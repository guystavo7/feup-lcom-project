#include "linkedList.h"
#include <stdlib.h>

/**
 * @brief Creates a new linked list
 * @param elementSize Size of each element in bytes
 * @param freeElement Function to free element data (can be NULL)
 * @param compareElement Function to compare two elements
 * @return Pointer to the newly created list
 */
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

/**
 * @brief Internal helper to free a node and its data
 * @param element The node to free
 * @param freeElement Function to free the element's data
 */
void _nodeFree(listNode* element, freeFunction freeElement) {
	if (freeElement) freeElement(element->data);
	free(element);
}

/**
 * @brief Frees the list and all its elements
 * @param list The list to free
 */
void listFree(list* list) {
	listNode* current = list->head;

	while (current) {
		listNode* next = current->next;
		_nodeFree(current, list->freeElement);
		current = next;
	}

	free(list);
}

/**
 * @brief Clears all elements from the list without freeing the list itself
 * @param list The list to clear
 */
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

/**
 * @brief Appends an element to the head of the list
 * @param list The list to modify
 * @param element Pointer to the element to add
 */
void listAppendHead(list* list, void* element) {
	listNode* newNode = malloc(sizeof(listNode));

	newNode->data = element;
	newNode->next = list->head;

	list->head = newNode;
	if (list->_length == 0) list->tail = list->head;

	list->_length++;
}

/**
 * @brief Appends an element to the tail of the list
 * @param list The list to modify
 * @param element Pointer to the element to add
 */
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

/**
 * @brief Gets the number of elements in the list
 * @param list The list to query
 * @return The number of elements
 */
int listGetSize(list* list) {
	return list->_length;
}

/**
 * @brief Gets the size of each element in bytes
 * @param list The list to query
 * @return The element size in bytes
 */
int listGetElementSize(list* list) {
	return list->_elementSize;
}

/**
 * @brief Gets the head node of the list
 * @param list The list to query
 * @return Pointer to the head node
 */
listNode* listGetHead(list* list) {
	return list->head;
}

/**
 * @brief Gets the tail node of the list
 * @param list The list to query
 * @return Pointer to the tail node
 */
listNode* listGetTail(list* list) {
	return list->tail;
}

/**
 * @brief Removes and frees the head element from the list
 * @param list The list to modify
 */
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

/**
 * @brief Removes and frees the tail element from the list
 * @param list The list to modify
 */
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

/**
 * @brief Checks if the list is empty
 * @param list The list to query
 * @return TRUE if empty, FALSE otherwise
 */
boolean listIsEmpty(list* list) {
	return list->_length == 0;
}

/**
 * @brief Checks if the list contains a specific element
 * @param list The list to search
 * @param element The element to find
 * @return TRUE if found, FALSE otherwise
 */
boolean listContains(list* list, void* element) {
	listNode* current = list->head;
	while (current) {
		if (list->compareElement(current->data, element)) return TRUE;
		current = current->next;
	}

	return FALSE;
}

/**
 * @brief Inserts an element at a specific index
 * @param list The list to modify
 * @param index The index at which to insert (0-based)
 * @param element Pointer to the element to insert
 */
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

/**
 * @brief Removes the first occurrence of an element from the list
 * @param list The list to modify
 * @param element The element to remove
 * @return TRUE if removed, FALSE if not found
 */
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

/**
 * @brief Removes an element at a specific index
 * @param list The list to modify
 * @param index The index of the element to remove (0-based)
 * @return TRUE if removed, FALSE if index is invalid
 */
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

/**
 * @brief Gets the node at a specific index
 * @param list The list to query
 * @param index The index to retrieve (0-based)
 * @return Pointer to the node, or NULL if index is invalid
 */
void* listGetAt(list* list, int index) {
	if (index < 0 || index >= list -> _length) return NULL;
	listNode* current = list -> head;

	while (index) {
		index--;
		current = current -> next;
	}

	return current;
}

/**
 * @brief Finds the index of an element in the list
 * @param list The list to search
 * @param element The element to find
 * @return The index of the element, or -1 if not found
 */
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

/**
 * @brief Finds the first element matching the predicate
 * @param list The list to search
 * @param predicate Function that returns TRUE when element matches
 * @return Pointer to the found element's data, or NULL if not found
 */
void* listFind(list* list, boolean (*predicate)(void* element)) {
	listNode* cur = list->head;
	while(cur) {
		if(predicate(cur->data)) return cur->data;
		cur = cur->next;
	}
	return NULL;
}

/**
 * @brief Finds the index of the first element matching the predicate
 * @param list The list to search
 * @param predicate Function that returns TRUE when element matches
 * @return The index of the element, or -1 if not found
 */
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

/**
 * @brief Executes a callback function for each element in the list
 * @param list The list to iterate
 * @param callback Function to execute for each element
 */
void listForEach(list* list, void (*callback)(void* element)) {
	listNode* cur = list->head;
	while(cur) {
		callback(cur->data);
		cur = cur->next;
	}
}
