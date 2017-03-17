#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

// We are only feeding the partitioner 10 inputs, but there are two
// extra spaces to keep inputColumn aligned
int inputKeys[12] = {43, 5, 24, 77, 56, 265, 122, 104, 91, 145, 0, 0};
// 16-byte column with 10 elements
unsigned long inputColumn[20] = {
	0xffaeL, 0xab35L, 0x8829L, 0xfe32L,
	0x5521L, 0x56b3L, 0x671cL, 0x44ddL,
	0x891aL, 0xbc32L, 0xab32L, 0x5321L,
	0xbaccL, 0x3333L, 0x1023L, 0xbfa4L,
	0xaaaaL, 0xddddL, 0x1026L, 0x3333L
};
// How does the input map to output
int inputIdx[10] = {1, 2, 0, 4, 3, 7, 8, 6, 9, 5};

int dividers[8] = {12, 32, 50, 60, 80, 110, 133, 160};

int outputKeys[12];
unsigned long outputColumn[20];
int outputIdx[9] = {0, 1, 2, 3, 4, 5, 7, 8, 9};



void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

void checkEqualLong(unsigned long expected, unsigned long actual)
{
	if (expected == actual)
		return;
	printf("Expected %x, got %x\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	int i;

	partition_set_columns(1);
	partition_set_size(0, SIZE_32);
	partition_set_size(1, SIZE_128);
	partition_set_input_addr(0, inputKeys);
	partition_set_input_addr(1, inputColumn);

	for (i = 0; i < 8; i++) {
		partition_set_key(i, dividers[i]);
	}
	for (i = 0; i < 9; i++) {
		partition_set_output_addr(i, 0, &outputKeys[outputIdx[i]]);
		partition_set_output_addr(i, 1, &outputColumn[2 * outputIdx[i]]);
	}
	partition_start(10);
	asm volatile ("fence");

	for (i = 0; i < 9; i++) {
		int actual, nextIdx, thisIdx;
		thisIdx = outputIdx[i];
		nextIdx = (i == 8) ? 10 : outputIdx[i+1];
		actual = partition_get_count(i);
		checkEqualInt(nextIdx - thisIdx, actual);
	}

	for (i = 0; i < 10; i++) {
		checkEqualInt(inputKeys[inputIdx[i]], outputKeys[i]);
		checkEqualLong(inputColumn[2 * inputIdx[i]], outputColumn[2 * i]);
		checkEqualLong(inputColumn[2 * inputIdx[i] + 1], outputColumn[2 * i + 1]);
	}

	return 0;
}
