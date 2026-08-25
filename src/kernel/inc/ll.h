#ifndef LL_H
#define LL_H

#include <util.h>

typedef struct LLNode {
  void* Data;
  struct LLNode* Next;
} LLNode;

typedef struct {
  LLNode* Head;
  LLNode* Tail;
} LinkedList;

void llInit(LinkedList* ll);
void llAdd(LinkedList* ll, void* data);
void llFree(LinkedList* ll);

#endif