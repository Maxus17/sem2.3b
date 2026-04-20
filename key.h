#ifndef KEY_H
#define KEY_H

#include "err.h"
#include "inp.h"

typedef char KeyType;
typedef int RealType;
typedef int IndexType;
typedef char InfoType;

typedef struct Node{
	RealType release;
	InfoType *info;
	struct Node *next;
}Node;

typedef struct KeySpace{
	KeyType *key;
	Node *node;
	struct KeySpace *next;
}KeySpace;


#endif
