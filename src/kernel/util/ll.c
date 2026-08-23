#include <alloc.h>
#include <ll.h>

void llAlloc(struct LinkedList* ll) {
  struct LLNode* head = malloc(sizeof(struct LLNode));
  memset(head, 0, sizeof(struct LLNode));

  ll->Head = head;
  ll->Tail = head;
}

void llAppend(struct LinkedList* ll, void* data) {
  struct LLNode* tail = malloc(sizeof(struct LLNode));
  memset(tail, 0, sizeof(struct LLNode));

  tail->Data = data;

  ll->Tail->Next = tail;
  ll->Tail = ll->Tail->Next;
}

void llFree(struct LinkedList* ll) {
  struct LLNode* cur = ll->Head;
  while (cur) {
    struct LLNode* next = cur->Next;
    free(cur);
    cur = next;
  }
}