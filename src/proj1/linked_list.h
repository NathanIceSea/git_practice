#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_ 1

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef unsigned int ElementType;

#ifndef _USE_LIST_ADT_H_
#define _USE_LIST_ADT_H_ 1
#include "list_adt.h"
#endif

struct Node {
    ElementType elem;
    PtrToNode next;
};

#endif