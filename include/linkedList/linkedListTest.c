#include "linkedList.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int x;
	int y;
} vector;

boolean isEven(void* element) {
	int* num = (int*)element;
	return *num % 2 == 0;
}

boolean isGreaterThanThree(void* element) {
	int* num = (int*)element;
	return *num > 3;
}

void printIntElement(void* element) {
	int* num = (int*)element;
	printf("  Element: %d\n", *num);
}

int sum = 0;
void addToSum(void* element) {
	int* num = (int*)element;
	sum += *num;
}

void multiplyByTwo(void* element) {
	int* num = (int*)element;
	*num *= 2;
}

void addTwo(void* element) {
	int* num = (int*)element;
	*num += 2;
}

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

void testInsertAt() {
	printf("Test insert at\n");

	list* l = listNew(sizeof(int), intFree, intCompare);

	int* n1 = malloc(sizeof(int));
	int* n2 = malloc(sizeof(int));
	int* n3 = malloc(sizeof(int));
	int* n4 = malloc(sizeof(int));

	*n1 = 1; *n2 = 2; *n3 = 3; *n4 = 4;

	listInsertAt(l, 100, n1);
	assert(*(int*)listGetHead(l) -> data == 1);
	assert(*(int*)listGetTail(l) -> data == 1);
	printList(l);


	listInsertAt(l, 0, n2);
	assert(*(int*)listGetHead(l) -> data == 2);
	assert(*(int*)listGetTail(l) -> data == 1);
	printList(l);


	listInsertAt(l, 1, n3);
	assert(*(int*)listGetHead(l) -> data == 2);
	assert(*(int*)listGetTail(l) -> data == 1);
	printList(l);


	listInsertAt(l, 1, n4);
	assert(*(int*)listGetHead(l) -> data == 2);
	assert(*(int*)listGetTail(l) -> data == 1);
	printList(l);

	listFree(l);
}

void testRemoveElement() {
	printf("Testing remove element\n");
	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;

	assert(listRemoveElement(l, num1) == FALSE);

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);

	printList(l);
	assert(listRemoveElement(l, num5) == FALSE);
	assert(listGetSize(l) == 4);

	printList(l);

	assert(listRemoveElement(l, num1) == TRUE);
	assert(listGetSize(l) == 3);
	assert(*(int*)listGetTail(l) -> data == 2);
	printList(l);

	assert(listRemoveElement(l, num4) == TRUE);
	assert(listGetSize(l) == 2);
	assert(*(int*)listGetHead(l) -> data == 3);
	printList(l);

	listAppendHead(l, num5);

	assert(listRemoveElement(l, num3) == TRUE);
	assert(listGetSize(l) == 2);
	assert(*(int*)listGetTail(l) -> data == 2);
	assert(*(int*)listGetHead(l) -> data == 5);
	printList(l);

	assert(listRemoveElement(l, num5) == TRUE);
	assert(listGetSize(l) == 1);
	assert(*(int*)listGetTail(l) -> data == 2);
	assert(*(int*)listGetHead(l) -> data == 2);
	printList(l);

	assert(listRemoveElement(l, num2) == TRUE);
	assert(listGetSize(l) == 0);
	assert(!listGetTail(l));
	assert(!listGetHead(l));
	printList(l);

	listFree(l);
}

void testRemoveByIndex() {
	printf("Test remove by index\n");
	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;
	printList(l);

	assert(listRemoveElementByIndex(l, 0) == FALSE);
	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);
	assert(listRemoveElementByIndex(l, -1) == FALSE);
	assert(listRemoveElementByIndex(l, 5) == FALSE);
	assert(listGetSize(l) == 5);
	printList(l);

	assert(listRemoveElementByIndex(l, 4) == TRUE);
	assert(*(int*)listGetTail(l) -> data == 2);
	assert(listGetSize(l) == 4);
	printList(l);

	assert(listRemoveElementByIndex(l, 0) == TRUE);
	assert(*(int*)listGetHead(l) -> data == 4);
	assert(listGetSize(l) == 3);
	printList(l);

	assert(listRemoveElementByIndex(l, 1) == TRUE);
	assert(*(int*)listGetTail(l) -> data == 2);
	assert(*(int*)listGetHead(l) -> data == 4);
	assert(listGetSize(l) == 2);
	printList(l);

	assert(listRemoveElementByIndex(l, 1) == TRUE);
	assert(*(int*)listGetTail(l) -> data == 4);
	assert(*(int*)listGetHead(l) -> data == 4);
	assert(listGetSize(l) == 1);
	printList(l);

	assert(listRemoveElementByIndex(l, 0) == TRUE);
	assert(!listGetTail(l));
	assert(!listGetHead(l));
	assert(listGetSize(l) == 0);
	printList(l);

	listFree(l);
}

void testGetAt() {
	printf("Test get at\n");
	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;
	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);

	for (int i = 0; i < 5; i++) {
		listNode* node = listGetAt(l, i);
		printf("Index %d: %d\n", i, *(int*)node -> data);
	}

	listFree(l);
}

void testIndexOf() {
	//Testing indexOf for integers
	printf("Test indexOf\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));

	*num1 = 1;
	*num2 = 2;
	*num3 = 3;

	assert(listIndexOf(l, num1) == -1);
	assert(listIndexOf(l, num2) == -1);
	assert(listIndexOf(l, num3) == -1);
	listAppendHead(l, num1);

	assert(listIndexOf(l, num1) == 0);
	assert(listIndexOf(l, num2) == -1);
	assert(listIndexOf(l, num3) == -1);
	listAppendHead(l, num2);

	assert(listIndexOf(l, num1) == 1);
	assert(listIndexOf(l, num2) == 0);
	assert(listIndexOf(l, num3) == -1);
	listPopHead(l);

	assert(listIndexOf(l, num1) == 0);
	assert(listIndexOf(l, num3) == -1);

	free(num3);
	listFree(l);
}

void testFind() {
	//Testing find for integers
	printf("Test find\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;

	assert(listFind(l, isEven) == NULL);
	assert(listFind(l, isGreaterThanThree) == NULL);

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);

	int* found = (int*)listFind(l, isEven);
	assert(found != NULL);
	assert(*found == 4);
	printf("Found first even: %d\n", *found);

	found = (int*)listFind(l, isGreaterThanThree);
	assert(found != NULL);
	assert(*found == 5);
	printf("Found first > 3: %d\n", *found);

	listFree(l);
}

void testFindIndex() {
	//Testing findIndex for integers
	printf("Test findIndex\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;

	assert(listFindIndex(l, isEven) == -1);
	assert(listFindIndex(l, isGreaterThanThree) == -1);

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);

	int found = listFindIndex(l, isEven);
	assert(found != -1);
	assert(found == 1);
	printf("Found first even: %d\n", found);

	found = listFindIndex(l, isGreaterThanThree);
	assert(found != -1);
	assert(found == 0);
	printf("Found first > 3: %d\n", found);

	listFree(l);
}

void testForEach1() {
	//Testing forEach for integers
	printf("Test forEach1\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);

	printf("Using forEach to print:\n");
	listForEach(l, printIntElement);

	sum = 0;
	listForEach(l, addToSum);
	assert(sum == 15);
	printf("Sum of all elements: %d\n", sum);

	listFree(l);
}

void testForEach2() {
	//Testing forEach to modify integer elements
	printf("Test forEach2\n");

	list* l = listNew(sizeof(int), intFree, intCompare);
	int* num1 = malloc(sizeof(int));
	int* num2 = malloc(sizeof(int));
	int* num3 = malloc(sizeof(int));
	int* num4 = malloc(sizeof(int));
	int* num5 = malloc(sizeof(int));

	*num1 = 1; *num2 = 2; *num3 = 3; *num4 = 4; *num5 = 5;

	listAppendHead(l, num1);
	listAppendHead(l, num2);
	listAppendHead(l, num3);
	listAppendHead(l, num4);
	listAppendHead(l, num5);

	printList(l);

	listForEach(l, multiplyByTwo);
	printf("After multiplying by 2:\n");
	printList(l);

	assert(*(int*)listGetHead(l)->data == 10);
	assert(*(int*)listGetTail(l)->data == 2);

	listForEach(l, addTwo);
	printf("After adding 2:\n");
	printList(l);
	assert(*(int*)listGetHead(l)->data == 12);
	assert(*(int*)listGetTail(l)->data == 4);

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

	testInsertAt();
	testRemoveElement();
	testRemoveByIndex();
	testGetAt();

	testIndexOf();
	testFind();
	testFindIndex();
	testForEach1();
	testForEach2();
}
