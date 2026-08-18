#ifndef LL_H
#define LL_H

#include <util.h>

struct LLNode {
  void* Data;
  struct LLNode* Next;
};

struct LinkedList {
  struct LLNode* Head;
  struct LLNode* Tail;
};

void llAlloc(struct LinkedList* ll);

void llAppend(struct LinkedList* ll, void* data);

void llFree(struct LinkedList* ll);

#endif