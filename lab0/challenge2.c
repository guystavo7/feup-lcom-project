#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void and(uint32_t n1, uint32_t n2) {
	printf("%d\n", n1 & n2);
}

void or(uint32_t n1, uint32_t n2) {
	printf("%d\n", n1 | n2);
}

void set(uint32_t n, uint32_t pos) {
	printf("%d\n", n | (1 << pos));
}

void unset(uint32_t n, uint32_t pos) {
	printf("%d\n", n & ~(1 << pos));
}

void byte(uint32_t n, uint32_t pos) {
	printf("%d\n", (n >> (8 * pos)) & 0xFF);
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Not enough arguments\n");
		exit(EXIT_FAILURE);
	}

	uint32_t n1 = atoi(argv[2]), n2 = atoi(argv[3]);

	if (strcmp("and", argv[1]) == 0) {
		and(n1, n2);
	} else if (strcmp("or", argv[1]) == 0) {
		or(n1, n2);
	} else if (strcmp("set", argv[1]) == 0) {
		set(n1, n2);
	} else if (strcmp("unset", argv[1]) == 0) {
		unset(n1, n2);
	} else {
		byte(n1, n2);
	}

	return 0;
}
