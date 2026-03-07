#include "linkedList.h"
#include <stdlib.h>

list* listNew(int elementSize, freeFunction freeElement, compareFunction compareElement) {
	list* l = malloc(sizeof(list));
	l->_elementSize = elementSize;
	l->_length = 0;

	l->freeElement = freeElement;
	l->compareElement = compareElement;
	l->head = NULL;
	l->tail = NULL;

	return l;
}

void _nodeFree(listNode* element, freeFunction freeElement) {
	if (freeElement) freeElement(element->data);
	free(element);
}


void listFree(list* list) {
	listNode* current = list->head;

	while (current) {
		listNode* next = current->next;
		_nodeFree(current, list->freeElement);
		current = next;
	}

	free(list);
}

void listClear(list* list) {
	listNode* current = list->head;
	while (current) {
		listNode* next = current->next;
		_nodeFree(current, list->freeElement);
		current = next;
	}

	list->_length = 0;
	list->head = NULL;
	list->tail = NULL;
}

void listAppendHead(list* list, void* element) {
	listNode* newNode = malloc(sizeof(listNode));

	newNode->data = element;
	newNode->next = list->head;

	list->head = newNode;
	if (list->_length == 0) list->tail = list->head;

	list->_length++;
}

void listAppendTail(list* list, void* element) {
	listNode* newNode = malloc(sizeof(listNode));

	newNode->data = element;
	newNode->next = NULL;

	if (list->_length == 0) {
		list->head = newNode;
	} else {
		list->tail->next = newNode;

	}

	list->tail = newNode;
	list->_length++;
}

int listGetSize(list* list) {
	return list->_length;
}

int listGetElementSize(list* list) {
	return list->_elementSize;
}

listNode* listGetHead(list* list) {
	return list->head;
}

listNode* listGetTail(list* list) {
	return list->tail;
}

void listPopHead(list* list) {
	if (list->_length == 0) return;

	listNode* current = list->head;
	if (list->_length == 1) {
		_nodeFree(list->head, list->freeElement);
		list->head = NULL;
		list->tail = NULL;
		list->_length = 0;
		return;
	}
	list->head = current->next;
	_nodeFree(current, list->freeElement);

	if (list->_length == 1) list->tail = NULL;
	list->_length--;
}

void listPopTail(list *list) {
	if (list->_length == 0) return;
	if (list->_length == 1) {
		_nodeFree(list->head, list->freeElement);
		list->head = NULL;
		list->tail = NULL;
		list->_length--;
		return;
	}

	listNode* current = list->head;
	while (current->next->next) current = current->next;

	list->tail = current;
	_nodeFree(current->next, list->freeElement);
	current->next = NULL;
	list->_length--;
}

boolean listIsEmpty(list* list) {
	return list->_length == 0;
}

boolean listContains(list* list, void* element) {
	listNode* current = list->head;
	while (current) {
		if (list->compareElement(current->data, element)) return TRUE;
		current = current->next;
	}

	return FALSE;
}

void listInsertAt(list *list, int index, void *element) {
	if (index == 0) {
		listAppendHead(list, element);
		return;
	}

	if (index >= list -> _length) {
		listAppendTail(list, element);
		return;
	}

	listNode* current = list -> head;
	listNode* newNode = malloc(sizeof(listNode));
	newNode -> data = element;

	while (index > 1) {
		current = current -> next;
		index--;
	}

	newNode -> next = current -> next;
	current -> next = newNode;
	list -> _length++;
}


boolean listRemoveElement(list *list, void *element) {
	if (list -> _length == 0) return FALSE;
	if (list -> _length == 1) {
		if (list -> head -> data == element) {
			_nodeFree(list -> head, list -> freeElement);
			list -> _length--;
			list -> head = NULL;
			list -> tail = NULL;
			return TRUE;
		}

		return FALSE;
	}

	listNode* current = list -> head;
	if (current -> data == element) {
		list -> _length--;
		list -> head = current -> next;
		_nodeFree(current, list -> freeElement);

		return TRUE;
	}

	while (current -> next) {
		if (current -> next -> data == element) {
			list -> _length--;
			listNode* next = current -> next -> next;
			if (!next) list -> tail = current;

			_nodeFree(current -> next, list -> freeElement);
			current -> next = next;
			return TRUE;
		}

		current = current -> next;
	}

	return FALSE;
}

boolean listRemoveElementByIndex(list* list, int index) {
	if (list -> _length == 0 || index < 0 || list -> _length <= index) return FALSE;

	listNode* current = list -> head;

	if (list -> _length == 1) {
		_nodeFree(current, list -> freeElement);
		list -> head = NULL;
		list -> tail = NULL;

	} else if (index == 0) {
		list -> head = current -> next;
		_nodeFree(current, list -> freeElement);

	} else {
		while (index != 1) {
			current = current -> next;
			index--;
		}

		if (current -> next == list -> tail) list -> tail = current;
		listNode* next = current -> next -> next;

		_nodeFree(current -> next, list -> freeElement);
		current -> next = next;
	}

	list -> _length--;
	return TRUE;
}

void* listGetAt(list* list, int index) {
	if (index < 0 || index >= list -> _length) return NULL;
	listNode* current = list -> head;

	while (index) {
		index--;
		current = current -> next;
	}

	return current;
}

int listIndexOf(list* list, void* element) {
	listNode* cur = list->head;
	int index = 0;
	while(cur) {
		if(list->compareElement(cur->data, element)) return index;
		index++;
		cur = cur->next;
	}
	return -1;
}

void* listFind(list* list, boolean (*predicate)(void* element)) {
	listNode* cur = list->head;
	while(cur) {
		if(predicate(cur->data)) return cur->data;
		cur = cur->next;
	}
	return NULL;
}

int listFindIndex(list* list, boolean (*predicate)(void* element)) {
	listNode* cur = list->head;
	int index = 0;
	while(cur) {
		if(predicate(cur->data)) return index;
		index++;
		cur = cur->next;
	}
	return -1;
}

void listForEach(list* list, void (*callback)(void* element)) {
	listNode* cur = list->head;
	while(cur) {
		callback(cur->data);
		cur = cur->next;
	}
}
