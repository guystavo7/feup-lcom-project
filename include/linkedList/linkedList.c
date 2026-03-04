#include "linkedList.h"
#include <stdlib.h>

list* listNew(int elementSize, freeFunction freeElement, compareFunction compareElement) {
	list* l = malloc(sizeof(list));
	l -> _elementSize = elementSize;
	l -> _length = 0;

	l -> freeElement = freeElement;
	l -> compareElement = compareElement;
	l -> head = NULL;
	l -> tail = NULL;

	return l;
}

void _nodeFree(listNode* element, freeFunction freeElement) {
	if (freeElement) freeElement(element -> data);
	free(element);
}


void listFree(list* list) {
	listNode* current = list -> head;

	while (current) {
		listNode* next = current -> next;
		_nodeFree(current, list -> freeElement);
		current = next;
	}

	free(list);
}

void listClear(list* list) {
	listNode* current = list -> head;
	while (current) {
		listNode* next = current -> next;
		_nodeFree(current, list -> freeElement);
		current = next;
	}

	list -> _length = 0;
	list -> head = NULL;
}

void listAppendHead(list* list, void* element) {
	listNode* newNode = malloc(sizeof(listNode));

	newNode -> data = element;
	newNode -> next = list -> head;

	if (list -> _length == 0) list -> tail = list -> head;
	list -> head = newNode;
	list -> _length++;
}

int listGetSize(list* list) {
	return list -> _length;
}

int listGetElementSize(list* list) {
	return list -> _elementSize;
}

listNode* listGetHead(list* list) {
	return list -> head;
}

listNode* listGetTail(list* list) {
	return list -> tail;
}

void listPopHead(list* list) {
	if (list -> _length == 0) return;

	listNode* current = list -> head;
	list -> head = current -> next;
	_nodeFree(current, list -> freeElement);

	if (list -> _length == 1) {
		list -> head = NULL;
		list -> tail = NULL;
	}

	list -> _length--;
}
