#include "../../include/linkedList/linkedList.h"
#include <assert.h>
#include <stdio.h>

void printList(list* l) {
	_listNode* current = l -> head;
	int index = 0;

	printf("Printing list\n");
	while (current) {
		printf("Index %d: %d\n", index, *(int*) current -> data);
		index++;
		current = current -> next;
	}
}

void notFree(void* aa) {
	return;
}
void testNewFreeList() {
	list* l = newList(sizeof(int), notFree);
	freeList(l);
}
void testAppendHead() {
	list* l = newList(sizeof(int), notFree);
	int num;
	printList(l);

	num = 5;
	listAppendHead(l, &num);
	printList(l);

	num = 6;
	listAppendHead(l, &num);
	printList(l);

	num = 1;
	listAppendHead(l, &num);
	num = 2;
	listAppendHead(l, &num);
	printList(l);

	freeList(l);
}

int main() {
	testNewFreeList();
	testAppendHead();
}
