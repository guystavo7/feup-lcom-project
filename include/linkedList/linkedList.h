#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef void (*freeFunction)(void *);
typedef int (*compareFunction)(void *, void *);

typedef enum { FALSE, TRUE } boolean;

struct _listNode {
  void* data;
  struct _listNode* next;
};

typedef struct _listNode listNode;

typedef struct {
  int _elementSize;
  int _length;
  freeFunction freeElement;
  compareFunction compareElement;
  listNode* head;
  listNode* tail;
} list;

list* listNew(int elementSize, freeFunction freeElement, compareFunction compareElement);
void listFree(list *list);
void listClear(list *list);

void listAppendHead(list* list, void* element);
void listAppendTail(list* list, void* element);
int listGetSize(list *list);
int listGetElementSize(list *list);

//boolean listPeekHead(list* list, void* element);
//boolean listPeekTail(list* list, void* element);

void listPopHead(list* list);
//boolean listPopHead(list* list, void* element);
void listPopTail(list* list);
//boolean listPopTail(list* list, void* element);

//boolean listRemoveElement(list* list, void* element);
//boolean listRemoveElementByIndex(list* list, int index);

//boolean listInsertAt(list* list, int index, void* element);

//boolean listGetAt(list* list, int index, void* element);
//void* listGetDataAt(list* list, int index);

listNode* listGetHead(list* list);
listNode* listGetTail(list* list);

//listNode* listGetNext(listNode* node);

//void* listNodeGetData(listNode* node);

//boolean listIsEmpty(list* list);
//boolean listContains(list* list, void* element);

//int listIndexOf(list* list, void* element);
//void* listFind(list* list, boolean (*predicate)(void* element));

//int listFindIndex(list* list, boolean (*predicate)(void* element));
//void listForEach(list* list, void (*callback)(void* element));

#endif
