#include "linkedList.h"
#include <stdlib.h>

list* newList(int elementSize, freeFunction freeElement) {
	list* l = malloc(sizeof(list));
	l -> _elementSize = elementSize;
	l -> _length = 0;

	l -> freeElement = freeElement;
	l -> head = NULL;
	l -> tail = NULL;

	return l;
}

void freeList(list* list) {
	_listNode* current = list -> head;

	while (current) {
		if (list -> freeElement) {
			list -> freeElement(current -> data);
		}

		_listNode* next = current -> next;
		free(current);
		current = next;
	}

	free(list);
}

void listAppendHead(list* list, void* element) {
	_listNode* newNode = malloc(sizeof(_listNode));

	newNode -> data = element;
	newNode -> next = list -> head;

	if (!list -> tail) list -> tail = list -> head;
	list -> head = newNode;
	list -> _length++;
}
