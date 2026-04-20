#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "dialog.h"

Err dialogInput(Table **table)
{
	KeyType *key = NULL;
	InfoType *info = NULL;
		
	Err code = ERR_OK;
	
	printf("Введите ключ нового элемента\n");
	key = readline("> ");
	if (key == NULL)
	{
		return ERR_EOF;
	}

	printf("Введите информацию элемента\n");
	info = readline("> ");
	if (info == NULL)
	{
		free(key);
		return ERR_EOF;
	}

	code = put(*table, key, info);
	if (code != ERR_OK)
	{	
		free(info);
		free(key);
		return code;
	}
	
	free(info);
	free(key);
	return ERR_OK;
}

Err dialogGet(Table *table)
{
	KeyType *key;
		
	Err code = ERR_OK;
	
	printf("Введите ключ удаления\n");
	key = readline("> ");
	if (key == NULL)
	{
		return ERR_EOF;
	}

	code = get(table, key);
	if (code != ERR_OK)
	{	
		free(key);
		return code;
	}

	free(key);
	return ERR_OK;
}

Err dialogFindKey(Table *table)
{	
	int cnt = 0;;

	printf("Введите ключ для поиска\n");
	KeyType *key = readline("> ");
	if (key == NULL)
	{
		return ERR_EOF;
	}

	Node **nodes = find(table, key, &cnt);
	if (nodes == NULL)
	{
		free(key);
		return ERR_ELEMENT;
	}

	printf("Ключ элемента: %s\n", key);
	for (int i = 0; i< cnt; i++)
	{
		printf("   Версия: %d; Информация: %s\n", nodes[i]->release, nodes[i]->info);
	}

	free(key);
	freeNodes(nodes, cnt);
	return ERR_OK;
}

Err dialogImport(Table **table)
{
	Err code = ERR_OK;
	
	printf("введите название файла\n");
	char *fname = readline("> ");
	if (fname == NULL)
	{
		return ERR_EOF;
	}

	if((code = bin(fname)) != ERR_OK)
	{
		free(fname);
		return code;
	}
	
	code = importBin(fname, table);
	if (code != ERR_OK)
	{
		free(fname);
		return code;
	}
	free(fname);
	return ERR_OK;
}

Err dialogExport(Table *table)
{
	Err code = ERR_OK;
	
	printf("введите название файла\n");
	char *fname = readline("> ");
	if (fname == NULL)
	{
		return ERR_EOF;
	}

	if((code = bin(fname)) != ERR_OK)
	{
		free(fname);
		return code;
	}
	
	code = exportBin(fname, table);
	if (code != ERR_OK)
	{
		free(fname);
		return code;
	}
	free(fname);
	return ERR_OK;
}

Err dialogFindVerse(Table *table)
{
	int inp = 2;
	
	printf("Введите ключ для поиска\n");
	KeyType *key = readline("> ");
	if (key == NULL)
	{
		return ERR_EOF;
	}
	
 	RealType release = 0;
	while (inp == 2)
	{
		printf("Введите версию для поиска\n> ");
		inp = input(&release);
		if (inp == EOF)
		{
			free(key);
			return ERR_EOF;
		}
	}
	inp = 2;

	KeySpace *elem = findElem(table, key, release);
	if (elem == NULL)
	{
		free(key);
		return ERR_ELEMENT;
	}

	printf("ключ: %s, версии:\n", elem->key);
	printf("         версия: %d; инфо: %s\n", elem->node->release, elem->node->info);

	free(key);
	freeElem(elem);
	return ERR_OK;
}

Err dialogGetRelease(Table *table)
{
	int inp = 2;
	
	printf("Введите ключ для поиска\n");
	KeyType *key = readline("> ");
	if (key == NULL)
	{
		return ERR_EOF;
	}
	
 	RealType release = 0;
	while (inp == 2)
	{
		printf("Введите версию для поиска\n> ");
		inp = input(&release);
		if (inp == EOF)
		{
			free(key);
			return ERR_EOF;
		}
	}

	Err code = getRelease(table, key, release);
	if (code != ERR_OK)
	{
		free(key);
		return code;
	}

	free(key);
	return ERR_OK;
}
