#ifndef __LIST_NODE_H
#define __LIST_NODE_H

typedef void (*freeFunction)(void *);

typedef struct _listNode {
  	void* data;
	struct _listNode* next;
} _listNode;

typedef struct {
  	int _elementSize;
	int _length;
	freeFunction freeElement;
	_listNode* head;
	_listNode* tail;
} list;

list* newList(int elementSize, freeFunction freeElement);
void freeList(list *list);

void listAppendHead(list* list, void* element);
void listAppendTail(list* list, void* element);
int getListSize(list *list);
int getElementSize(list *list);

void getOrRemoveListHead(list* list, void* element, bool remove);
void getOrRemoveListTail(list* list, void* element, bool remove);

void listRemoveElement(list* list, void* element);

void listRemoveElementByIndex(list* list, void* element, int index);

void listInsertAt(list* list, int index, void* element);

#endif
