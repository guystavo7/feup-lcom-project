#include "../../include/linkedList/linkedList.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int x;
	int y;
} vector;

void printList(list* l) {
	listNode* current = l -> head;
	int index = 0;

	printf("	Printing list\n");
	while (current) {
		printf(" 	- Index %d: %d\n", index, *(int*) current -> data);
		index++;
		current = current -> next;
	}
}

void intFree(void* n) {
	free(n);
}

void vectorFree(void* v) {
	free(v);
}

int intCompare(void* n1, void* n2) {
	return *(int*)n1 == *(int*)n2;
}

int vectorCompare(void* v1, void* v2) {
	vector* a = (vector*) v1;
	vector* b = (vector*) v2;

	return a -> x == b -> x && a -> y == b -> y;
}

void testNewFreeList1() {
	//Testing creating and freeing an integer list
	printf("Test new and free 1\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	listFree(l);
}

void _testNewFreeList2(list* l) {
	//Auxiliar function that adds an integer in list - checking if data is persistent after return
	assert(listGetSize(l) == 0);

	int* num = malloc(sizeof(int));
	*num = 5;

	listAppendHead(l, num);
	assert(listGetSize(l) == 1);
	printList(l);
}

void testNewFreeList2() {
	//Checking if list data is persistent after return
	printf("Test new and free 2\n");
	list* l = listNew(sizeof(int), intFree, intCompare);

	_testNewFreeList2(l);
	assert(listGetSize(l) == 1);
	printList(l);

	listFree(l);
}

void _testNewFreeList3(list* l) {
	//Checking new and free for list with a more complex type - checking for persistent data
	vector* v1 = malloc(sizeof(vector));
	v1 -> x = 5;
	v1 -> y = 10;

	listAppendHead(l, v1);
	assert(listGetSize(l) == 1);
}

void testNewFreeList3() {
	//Checking new and free for list with a more complex type
	printf("Test new and free 3\n");
	list* l = listNew(sizeof(vector), vectorFree, vectorCompare);

	assert(listGetSize(l) == 0);
	_testNewFreeList3(l);

	vector* head = listGetHead(l) -> data;
	assert(head -> x == 5);
	assert(head -> y == 10);

	printf("Vector: %d, %d\n", head -> x, head -> y);
	listFree(l);
}

void testAppendHead() {
	//Testing pushFront
	printf("Test append head\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
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
	//Testing all getters
	printf("Test getters\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	assert(listGetSize(l) == 0);
	assert(listIsEmpty(l));

	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	listAppendHead(l, num1);
	assert(listGetSize(l) == 1);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 1);
	assert(!listIsEmpty(l));

	*num2 = 2;
	*num3 = 3;
	listAppendHead(l, num2);
	listAppendHead(l, num3);

	assert(*(int*)listGetHead(l) -> data == 3);
	assert(listGetSize(l) == 3);

	listFree(l);
}

void testClear() {
	//Testing clearing empty and non-empty lists
	printf("Test clear\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	assert(listGetElementSize(l) == sizeof(int));

	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	listClear(l);

	*num1= 1;
	listAppendHead(l, num1);

	*num2 = 2;
	listAppendHead(l, num2);

	*num3 = 3;
	listAppendHead(l, num3);

	printList(l);
	assert(listGetSize(l) == 3);
	assert(listIsEmpty(l) == 0);

	listClear(l);
	printList(l);
	assert(listGetSize(l) == 0);
	assert(listIsEmpty(l));

	listFree(l);
}

void testPopHead() {
	//Testing pop_front - empty and non empty lists
	printf("Test pop head\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	listPopHead(l);

	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));

	*num1 = 1;
	*num2 = 2;

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	printList(l);
	assert(*(int*)listGetHead(l) -> data == 2);
	assert(*(int*)listGetTail(l) -> data == 1);

	listPopHead(l);
	printList(l);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 1);

	listPopHead(l);
	printList(l);
	assert(!listGetHead(l));
	assert(!listGetTail(l));

	listPopHead(l);
	printList(l);

	assert(!listGetHead(l));
	assert(!listGetTail(l));

	listFree(l);
}

void testPopTail() {
	//Testing pop back
	printf("Test pop tail\n");

	list* l = listNew(sizeof(list), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	*num2 = 2;
	*num3 = 3;

	listPopTail(l);
	printList(l);

	listAppendTail(l, num1);
	listAppendTail(l, num2);
	listAppendTail(l, num3);
	assert(listGetSize(l) == 3);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 3);

	printList(l);
	listPopTail(l);
	assert(listGetSize(l) == 2);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 2);

	printList(l);
	listPopTail(l);
	assert(listGetSize(l) == 1);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 1);

	printList(l);
	listPopTail(l);
	assert(listGetSize(l) == 0);
	assert(!listGetHead(l));
	assert(!listGetTail(l));

	listFree(l);
}

void testAppendTail() {
	//Testing pushBack
	printf("Test append tail\n");

	list* l = listNew(sizeof(list), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	*num2 = 2;
	*num3 = 3;
	printList(l);

	listAppendTail(l, num1);
	printList(l);

	assert(*(int*)listGetTail(l) -> data == 1);
	assert(*(int*)listGetHead(l) -> data == 1);

	listAppendTail(l, num2);
	printList(l);

	assert(*(int*)listGetTail(l) -> data == 2);
	assert(*(int*)listGetHead(l) -> data == 1);

	listAppendTail(l, num3);
	printList(l);
	assert(*(int*)listGetTail(l) -> data == 3);
	assert(*(int*)listGetHead(l) -> data == 1);

	listFree(l);
}

void testAppend() {
	printf("Testing both appends\n");
	//Testing pushing back and front
	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4;
	printList(l);

	listAppendTail(l, num1);
	printList(l);

	listAppendTail(l, num2);
	printList(l);

	listAppendHead(l, num3);
	printList(l);

	listAppendTail(l, num4);
	printList(l);

	listFree(l);
}

void testContains1() {
	//Testing contains for integers
	printf("Test contains 1\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	*num2 = 2;
	*num3 = 3;

	assert(!listContains(l, num1));
	assert(!listContains(l, num2));
	assert(!listContains(l, num3));
	listAppendHead(l, num1);

	assert(listContains(l, num1));
	assert(!listContains(l, num2));
	assert(!listContains(l, num3));
	listAppendHead(l, num2);

	assert(listContains(l, num1));
	assert(listContains(l, num2));
	assert(!listContains(l, num3));
	listPopHead(l);

	num2 = malloc(sizeof(int));
	*num2 = 2;

	assert(listContains(l, num1));
	assert(!listContains(l, num2));
	assert(!listContains(l, num3));

	free(num2);
	free(num3);
	listFree(l);
}

void testContains2() {
	//Test contains for list of vectors
	printf("Test contains 2\n");

	list* l = listNew(sizeof(vector), vectorFree, vectorCompare);
	vector* v1 = malloc(sizeof(vector));
	vector* v2 = malloc(sizeof(vector));

	v1 -> x = 1; v1 -> y = 2;
	v2 -> x = 2; v2 -> y = 4;

	assert(!listContains(l, v1));
	assert(!listContains(l, v2));
	listAppendHead(l, v1);

	assert(listContains(l, v1));
	assert(!listContains(l, v2));
	listAppendHead(l, v2);

	assert(listContains(l, v1));
	assert(listContains(l, v2));

	listFree(l);
}

int main() {
	testNewFreeList1();
	testNewFreeList2();
	testNewFreeList3();

	testAppendHead();
	testAppendTail();
	testAppend();

	testPopHead();
	testPopTail();

	testGetters();
	testClear();

	testContains1();
	testContains2();
}
