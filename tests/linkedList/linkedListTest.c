#include "../../include/linkedList/linkedList.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void printList(list* l) {
	listNode* current = l -> head;
	int index = 0;

	printf("Printing list\n");
	while (current) {
		printf("Index %d: %d\n", index, *(int*) current -> data);
		index++;
		current = current -> next;
	}
}

void freeInt(void* n) {
	free(n);
}

int intCompare(void* n1, void* n2) {
	return *(int*)n1 < *(int*)n2;
}

void testNewFreeList1() {
	printf("Test new and free 1\n");

	list* l = listNew(sizeof(int), freeInt, intCompare);
	listFree(l);
}

void _testNewFreeList2(list* l) {
	assert(listGetSize(l) == 0);

	int* num = malloc(sizeof(int));
	*num = 5;

	listAppendHead(l, num);
	assert(listGetSize(l) == 1);
	printList(l);
}

void testNewFreeList2() {
	printf("Test new and free 2\n");
	list* l = listNew(sizeof(int), freeInt, intCompare);

	_testNewFreeList2(l);
	assert(listGetSize(l) == 1);
	printList(l);

	listFree(l);
}

void testAppendHead() {
	printf("Test append head\n");

	list* l = listNew(sizeof(int), freeInt, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	printList(l);

	*num1 = 5;
	listAppendHead(l, num1);
	printList(l);

	*num2 = 6;
	listAppendHead(l, num2);
	printList(l);

	*num3 = 1;
	listAppendHead(l, num3);

	*num4 = 2;
	listAppendHead(l, num4);
	printList(l);

	listFree(l);
}

void testGetters() {
	printf("Test getters\n");

	list* l = listNew(sizeof(int), freeInt, intCompare);
	assert(listGetSize(l) == 0);

	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	listAppendHead(l, num1);
	assert(listGetSize(l) == 1);
	assert(*(int*)listGetHead(l) -> data == 1);

	*num2 = 2;
	*num3 = 3;
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	assert(*(int*)listGetHead(l) -> data == 3);
	assert(listGetSize(l) == 3);

	listFree(l);
}

void testClear() {
	printf("Test clear\n");

	list* l = listNew(sizeof(int), freeInt, intCompare);

	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1= 1;
	listAppendHead(l, num1);

	*num2 = 2;
	listAppendHead(l, num2);

	*num3 = 3;
	listAppendHead(l, num3);

	printList(l);
	assert(listGetSize(l) == 3);

	listClear(l);
	printList(l);
	assert(listGetSize(l) == 0);

	listFree(l);
}

int main() {
	testNewFreeList1();
	testNewFreeList2();
	testAppendHead();
	testGetters();
	testClear();
}
