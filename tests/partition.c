#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

void checkEqual(uint32_t expected, uint32_t actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int inputKeys[10] = {43, 5, 24, 77, 56, 265, 122, 104, 91, 145};
unsigned long inputColumn[10] = {
	0xffaeL, 0xab35L, 0x8829L, 0xfe32L, 0x5521L,
	0x56b3L, 0x671cL, 0x44ddL, 0x891aL, 0xbc32L
};
int inputIdx[10] = {1, 2, 0, 4, 3, 7, 8, 6, 9, 5};

int dividers[8] = {12, 32, 50, 60, 80, 110, 133, 160};

int outputKeys[10];
unsigned long outputColumn[10];
int outputIdx[9] = {0, 1, 2, 3, 4, 5, 7, 8, 9};

int main(void)
{
	int i;

	partition_set_columns(1);
	partition_set_size(0, SIZE_32);
	partition_set_size(1, SIZE_64);
	partition_set_input_addr(0, inputKeys);
	partition_set_input_addr(1, inputColumn);

	for (i = 0; i < 8; i++) {
		partition_set_key(i, dividers[i]);
	}
	for (i = 0; i < 9; i++) {
		partition_set_output_addr(i, 0, &outputKeys[outputIdx[i]]);
		partition_set_output_addr(i, 1, &outputColumn[outputIdx[i]]);
	}
	partition_start(10);
	asm volatile ("fence");

	for (i = 0; i < 10; i++) {
		checkEqual(inputKeys[inputIdx[i]], outputKeys[i]);
		checkEqual(inputColumn[inputIdx[i]], outputColumn[i]);
	}

	return 0;
}
