#include <alloc.h>
#include <ll.h>

void llInit(LinkedList* ll) {
  ll->Head = 0;
  ll->Tail = 0;
}

void llAdd(LinkedList* ll, void* data) {
  LLNode* tail = malloc(sizeof(LLNode));

  tail->Next = 0;
  tail->Data = data;

  if (ll->Head == 0) {
    ll->Head = tail;
    ll->Tail = tail;
  } else {
    ll->Tail->Next = tail;
    ll->Tail = ll->Tail->Next;
  }
}

void llFree(LinkedList* ll) {
  LLNode* cur = ll->Head;
  while (cur) {
    LLNode* next = cur->Next;
    free(cur);
    cur = next;
  }

  ll->Head = 0;
  ll->Tail = 0;
}