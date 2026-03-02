#include <stdio.h>

void getBits(int num) {
	int size = 0, n = num;
	int bits[32] = { 0 };

	while (n > 0) {
		bits[size] = n % 2;
		n /= 2;
		size++;
	}

	for (int i = size - 1; i >= 0; i--) {
		printf("%d", bits[i]);
	}

	printf("\n");
}

int main() {
	getBits(5);
	getBits(8);
	getBits(15);

	return 0;
}
