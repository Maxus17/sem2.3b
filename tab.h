#ifndef TAB_H
#define TAB_H

#include "key.h"

typedef struct Table{
	KeySpace **ks;// указательна начало списка - head
	IndexType msize;
	IndexType csize;
}Table;

Table *createTable(const IndexType);
IndexType hash(KeyType *, IndexType msize);
void freeElem(KeySpace *);
void freeNode(Node *);
void freeNodes(Node **, const int);
void freeTab(Table *);
bool isEmpty(Table *);
Node **find(Table *, KeyType *, int *);
KeySpace *findElem(Table *, KeyType *, const RealType);
bool checkElem(Table *, KeyType *);
Err put(Table *, KeyType *, const InfoType *);
Err get(Table *, KeyType *);
Err printTab(Table *);
Err bin(const char *);
Err exportBin(const char *, Table *);
Err importBin(const char *, Table **);
Err getRelease(Table *, KeyType *, RealType);

#endif
