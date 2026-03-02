#include "linkedList.h"
#include <stdlib.h>

void freeElement(_listNode* element, freeFunction freeElement) {
	if (freeElement) freeElement(element -> data);
	free(element);
}

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
		freeElement(current, list -> freeElement);
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

int getListSize(list* list) {
	return list -> _length;
}

int getElementSize(list* list) {
	return list -> _elementSize;
}

void* listGetHead(list* list) {
	return list -> head;
}

void* listGedTail(list* list) {
	return list -> tail;
}

void listPopHead(list* list) {
	if (list -> _length == 0) return;

	_listNode* current = list -> head;
	list -> head = current -> next;
	freeElement(current, list -> freeElement);

	if (list -> _length == 1) {
		list -> head = NULL;
		list -> tail = NULL;
	}

	list -> _length--;
}
