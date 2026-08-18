#include <alloc.h>
#include <ll.h>

void llAlloc(struct LinkedList* ll) {
  ll->Head = malloc(sizeof(struct LLNode));
  ll->Tail = ll->Head;
}

void llAppend(struct LinkedList* ll, void* data) {
  ll->Tail->Next = malloc(sizeof(struct LLNode));
  ll->Tail = ll->Tail->Next;

  ll->Tail->Data = data;
  ll->Tail->Next = 0;
}

void llFree(struct LinkedList* ll) {
  struct LLNode* cur = ll->Head;
  while (cur) {
    struct LLNode* next = cur->Next;
    free(cur);
    cur = next;
  }
}