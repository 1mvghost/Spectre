#include <alloc.h>
#include <assert.h>
#include <ll.h>

void llInit(LinkedList* ll) {
  ASSERT(ll != 0);

  ll->Head = 0;
  ll->Tail = 0;
}

void llAdd(LinkedList* ll, void* data) {
  ASSERT(ll != 0);

  LLNode* tail = malloc(sizeof(LLNode));

  ASSERT(tail != 0);

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
  ASSERT(ll != 0);

  LLNode* cur = ll->Head;
  while (cur) {
    LLNode* next = cur->Next;
    free(cur);
    cur = next;
  }

  ll->Head = 0;
  ll->Tail = 0;
}