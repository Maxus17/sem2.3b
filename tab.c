#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tab.h"

Table *createTable(const IndexType cnt)
{
	Table *table = (Table *)malloc(sizeof(Table));
	if (!table)
	{
		return NULL;
	}
	table->msize = cnt;
	table->ks = (KeySpace **)calloc(table->msize, sizeof(KeySpace *));
	if (!table->ks)
	{
		return NULL;
	}
	table->csize = 0;
	return table;
}

IndexType hash(KeyType *key, IndexType msize)
{
	unsigned long h = 5381;
	int c;

	while ((c = *key++))
	{
		h = ((h << 5) + h) + c;
	}

	return (IndexType)(h % msize);
}

void freeNode(Node *node)
{
	if (node)
	{
		if (node->info)
		{
			free(node->info);
		}
		free(node);
	}
}

void freeElem(KeySpace *elem)
{
	if (elem)
	{
		if (elem->key)
		{
			free(elem->key);
		}
		if (elem->node)
		{
			Node *del_node = elem->node;
			while (del_node)
			{
				Node *tmp = del_node;
				del_node = del_node->next;
				freeNode(tmp);
			}
		}	
		
		free(elem);
	}
}

void freeNodes(Node **nodes, const int cnt)
{
	if (nodes)
	{
		for (int i = 0; i < cnt; i++)
		{
			if (nodes[i])
			{
				freeNode(nodes[i]);
			}
		}
		free(nodes);
	}
}

void freeTab(Table *table)
{
	if (table)
	{
		for (IndexType i = 0; i < table->msize; i++)
		{
			if (table->ks[i])
			{
				KeySpace *cur = table->ks[i];
				while (cur)
				{
					KeySpace *del = cur;
					cur = cur->next;			
					freeElem(del);
				}
			}
		}
		free(table->ks);
		free(table);
	}
}

bool isEmpty(Table *table)
{
	if (!table || !table->ks || table->csize == 0 || table->msize == 0)
	{
		return true;
	}
	return false;
}

bool checkElem(Table *table, KeyType *key)
{
	if (isEmpty(table) || !key)
	{
		return  false;
	}

	IndexType id = hash(key, table->msize);
	
	KeySpace *cur = table->ks[id];
	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
		{
			return true;
		}
		cur = cur->next;
	}
	return false;
}

Node **find(Table *table, KeyType *key, int *cnt)
{
	if (isEmpty(table) || !key)
	{
		return  NULL;
	}
	
	IndexType id = hash(key, table->msize);
	KeySpace *cur = table->ks[id];
	
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			*cnt = 0;
			Node *node = cur->node;
			while (node)
			{
				(*cnt)++;
				node = node->next;
			}

			Node **res = (Node **)malloc(*cnt * sizeof(Node*));
			if (!res)
			{
				return NULL;
			}

			node = cur->node;
			for (int i = 0; i < *cnt; i++)
			{
				res[i] = (Node *)malloc(sizeof(Node));
				if (!res[i])
				{
					freeNodes(res, i);
					return NULL;
				}
		
				res[i]->release = node->release;
				res[i]->info = strdup(node->info);
				res[i]->next = NULL;
		
				node = node->next;
			}
			return res;	
		}
		cur = cur->next;
	}
	
	return NULL;
}


KeySpace *findElem(Table *table, KeyType *key, const RealType release)
{
	if (isEmpty(table) || !key || release == 0)
	{
		return  NULL;
	}

	IndexType id = hash(key, table->msize);
	
	KeySpace *cur = table->ks[id];
	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
		{
			KeySpace *cpy = (KeySpace *)malloc(sizeof(KeySpace));
			if (cpy == NULL)
			{
				freeTab(table);
				return NULL;
			}
			
			cpy->key = strdup(cur->key);
			cpy->node = NULL;
			cpy->next = NULL;
			Node *node = cur->node;
			while (node != NULL)
			{
				if (node->release == release)
				{
					cpy->node = (Node *)malloc(sizeof(Node));
					if (cpy->node == NULL)
					{
						return NULL;
					}
					cpy->node->release = node->release;
					cpy->node->info = strdup(node->info);
					cpy->node->next = NULL;
					return cpy;
				}
				node = node->next;
			}
			freeElem(cpy);	
		}
		cur = cur->next;
	}
	return NULL;
}

Err put(Table *table, KeyType *key, const InfoType *info)
{
	if (!key || !info)
	{
		return ERR_FORMAT;
	}

	IndexType id = hash(key, table->msize);

	KeySpace *cur = table->ks[id];
	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
		{
			Node *node = (Node *)malloc(sizeof(Node));
			if (!node)
			{
			 	return ERR_MEM;
			}

			node->info = strdup(info);
			node->release = cur->node->release + 1;
			node->next = cur->node;
			cur->node = node;

			table->csize++;
			return ERR_OK;
		}

		cur = cur->next;
	}	

	KeySpace *new = (KeySpace *)calloc(1, sizeof(KeySpace));
	if (!new)
	{
		freeTab(table);
	 	return ERR_MEM;
	}

	new->key = strdup(key);

	Node *node = (Node *)malloc(sizeof(Node));
	if (!node)
	{
		freeTab(table);
		freeElem(new);
	 	return ERR_MEM;
	}

	node->info = strdup(info);
	node->release = 1;
	node->next = NULL;

	new->node = node;
	new->next = table->ks[id];
	table->ks[id] = new;

	table->csize++;
	return ERR_OK;
}

Err get(Table *table, KeyType *key)
{
	if (isEmpty(table) || !key)
	{
		return ERR_EMPTY;
	}
	
	if (!checkElem(table, key))
	{
		return ERR_ELEMENT;
	}
		
	IndexType id = hash(key, table->msize);
	
	KeySpace *cur = table->ks[id];
	KeySpace *prev = NULL;
	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
		{			
			KeySpace *tmp = cur;
			cur = cur->next;
			freeElem(tmp);
			if (prev)
			{
				prev->next = cur;
			}
			else
			{
				table->ks[id] = cur;
			}

			table->csize--;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	return ERR_OK;
}

Err printTab(Table *table)
{
	if (isEmpty(table))
	{
		return ERR_EMPTY;
	}
	
	for (IndexType i = 0; i < table->msize; i++)
	{
		KeySpace *cur = table->ks[i];
		printf(" %d: ", i);
		if (!cur)
		{
			printf("NULL\n");
			continue;
		}
		
		while (cur)
		{
			printf("ключ: %s, версии:\n", cur->key);
			
			Node *node = cur->node;
			while (node)
			{
				printf("         версия: %d; инфо: %s\n", node->release, node->info);
				node = node->next;
			}
			cur = cur->next;
		}
		printf("\n");
	}
	
}

Err bin(const char *fname)
{
	char *p = strrchr(fname, '.');
	if (p == NULL || p == fname)
	{
		return ERR_FILE_NAME;
	}
	int n = strcmp(p + 1, "bin");
	if (n == 0)
	{
		return ERR_OK;
	}

	return ERR_FILE_NAME;
}

Err exportBin(const char *fname, Table *table)
{
	FILE *fp = fopen(fname, "wb");
	if (fp == NULL)
	{
		return ERR_FILE;
	}

	if (fwrite(&table->msize, sizeof(IndexType), 1, fp) != 1)
    {
		fclose(fp);
		return ERR_FILE;
	}

	for (IndexType i = 0; i < table->msize; i++)
	{
		KeySpace *cur = table->ks[i];
		int key_flag = 0;
		if (cur)
		{
			key_flag = -1;
		}
		else
		{
			key_flag = -2;
		}

		if (fwrite(&key_flag, sizeof(int), 1, fp) != 1)
	    {
			fclose(fp);
			return ERR_FILE;
		}

		while (cur)
		{
			size_t key_len = strlen(cur->key);
			if (fwrite(&key_len, sizeof(size_t), 1, fp) != 1)
		    {
				fclose(fp);
				return ERR_FILE;
			}
			if (fwrite(cur->key, sizeof(KeyType), key_len, fp) != key_len)
		    {
				fclose(fp);
				return ERR_FILE;
			}

			Node *node = cur->node;
			int node_flag = 0;
			if (node)
			{
				node_flag = -1;
			}
			else
			{
				node_flag = -2;
			}

			if (fwrite(&node_flag, sizeof(int), 1, fp) != 1)
		    {
				fclose(fp);
				return ERR_FILE;
			}

			while (node)
			{
				size_t info_len = strlen(node->info);
				if (fwrite(&info_len, sizeof(size_t), 1, fp) != 1)
			    {
					fclose(fp);
					return ERR_FILE;
				}
				if (fwrite(node->info, sizeof(InfoType), info_len, fp) != info_len)
			    {
					fclose(fp);
					return ERR_FILE;
				}

				if (node->next)
				{
					node_flag = -1;
				}
				else
				{
					node_flag = -2;
				}
	
				if (fwrite(&node_flag, sizeof(int), 1, fp) != 1)
			    {
					fclose(fp);
					return ERR_FILE;
				}
				node = node->next;
			}
			if (cur->next)
			{
				key_flag = -1;
			}
			else
			{
				key_flag = -2;
			}

			if (fwrite(&key_flag, sizeof(int), 1, fp) != 1)
		    {
				fclose(fp);
				return ERR_FILE;
			}

			cur = cur->next;
		}
	}
	fclose(fp);
	return ERR_OK;
}

Err importBin(const char *fname, Table **table)
{
	Err code = ERR_OK;

	FILE *fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		return ERR_FILE;
	}

	IndexType msize = 0;
	if (fread(&msize, sizeof(IndexType), 1, fp) != 1)
	{
		fclose(fp);
		return ERR_FORMAT;
	}

	Table *res = createTable(msize);

	for (IndexType i = 0; i < msize; i++)
	{
		int key_flag = 0;
		if (fread(&key_flag, sizeof(int), 1, fp) != 1)
		{
			freeTab(res);
			fclose(fp);
			return ERR_FORMAT;
		}

		while (key_flag == -1)
		{
			size_t key_len = 0;
			if (fread(&key_len, sizeof(size_t), 1, fp) != 1)
			{
				freeTab(res);
				fclose(fp);
				return ERR_FORMAT;	
			}

			KeyType *key = (KeyType *)calloc(key_len + 1, sizeof(KeyType));
			if (!key)
			{
				freeTab(res);
				fclose(fp);
				return ERR_MEM;
			}

			if (fread(key, sizeof(KeyType), key_len, fp) != key_len)
			{
				freeTab(res);
				free(key);
				fclose(fp);
				return ERR_FORMAT;	
			}

			int node_flag = 0;
			if (fread(&node_flag, sizeof(int), 1, fp) != 1)
			{
				freeTab(res);
				free(key);
				fclose(fp);
				return ERR_FORMAT;
			}
			
			while (node_flag == -1)
			{
				size_t len_info = 0;
				if (fread(&len_info, sizeof(size_t), 1, fp) != 1)
				{
					freeTab(res);
					free(key);
					fclose(fp);
					return ERR_FORMAT;	
				}

				InfoType *info = (KeyType *)calloc(len_info + 1, sizeof(KeyType));
				if (!info)
				{
					freeTab(res);
					free(key);
					fclose(fp);
					return ERR_MEM;
				}
	
				if (fread(info, sizeof(InfoType), len_info, fp) != len_info)
				{
					freeTab(res);
					free(key);
					free(info);
					fclose(fp);
					return ERR_FORMAT;	
				}

				code = put(res, key, info);
				if (code != ERR_OK)
				{
					freeTab(res);
					free(key);
					free(info);
					fclose(fp);
					return code;
				}

				free(info);

				if (fread(&node_flag, sizeof(int), 1, fp) != 1)
				{
					freeTab(res);
					free(key);
					fclose(fp);
					return ERR_FORMAT;
				}
			}
			free(key);

			if (fread(&key_flag, sizeof(int), 1, fp) != 1)
			{
				freeTab(res);
				fclose(fp);
				return ERR_FORMAT;
			}
		}
	}

	freeTab(*table);
	*table = res;
	fclose(fp);
	return ERR_OK;
}

Err getRelease(Table *table, KeyType *key, RealType release)
{
	if (isEmpty(table) || !key || release == 0)
	{
		return ERR_EMPTY;
	}

	if (!checkElem(table, key))
	{
		return ERR_ELEMENT;
	}

	IndexType id = hash(key, table->msize);
	KeySpace *cur = table->ks[id];
	KeySpace *prev = NULL;

	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
		{
			Node *node = cur->node;
			Node *prev_node = NULL;
			
			while (node != NULL)
			{
				if (node->release == release)
				{		
					if (prev_node == NULL)
					{
						cur->node = node->next;				
					}
					else
					{
						prev_node->next = node->next;
					}
					
					freeNode(node);
					
					if (!cur->node)
					{
						KeySpace *tmp = cur;
						cur = cur->next;
						freeElem(tmp);
						if (prev)
						{
							prev->next = cur;
						}
						else
						{
							table->ks[id] = cur;
						}
						
					}
					return ERR_OK;
				}
				else
				{
					prev_node = node;
					node = node->next;
				}
			}
			return ERR_ELEMENT;
		}
		else
		{
			prev = cur; 
			cur = cur->next;
		}
	}
	return ERR_ELEMENT;
}
