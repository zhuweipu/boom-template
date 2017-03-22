#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"

unsigned int inputData[8] = {1, 2, 5, 3, 8, 6, 0, 10};
unsigned int pivot = 5;

unsigned char outputData[8];
unsigned char eqExpected[8] = {0, 0, 1, 0, 0, 0, 0, 0};
unsigned char ltExpected[8] = {1, 1, 0, 1, 0, 0, 1, 0};
unsigned char leExpected[8] = {1, 1, 1, 1, 0, 0, 1, 0};
unsigned char gtExpected[8] = {0, 0, 0, 0, 1, 1, 0, 1};
unsigned char geExpected[8] = {0, 0, 1, 0, 1, 1, 0, 1};

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

void checkEqualArr(unsigned char *expected, unsigned char *actual)
{
	for (int i = 0; i < 8; i++)
		checkEqualInt(expected[i], actual[i]);
}

int main(void)
{
	boolgen_set_addrs(inputData, outputData);
	boolgen_set_size(SIZE_32);

	printf("equal test\n");
	boolgen_set_op(CMP_OP_EQUAL, pivot);
	boolgen_start(0, 8);
	asm volatile ("fence");
	checkEqualArr(eqExpected, outputData);

	printf("less test\n");
	boolgen_set_op(CMP_OP_LESS, pivot);
	boolgen_start(0, 8);
	asm volatile ("fence");
	checkEqualArr(ltExpected, outputData);

	printf("less or equal test\n");
	boolgen_set_op(CMP_OP_LTE, pivot);
	boolgen_start(0, 8);
	asm volatile ("fence");
	checkEqualArr(leExpected, outputData);

	printf("greater test\n");
	boolgen_set_op(CMP_OP_GREATER, pivot);
	boolgen_start(0, 8);
	asm volatile ("fence");
	checkEqualArr(gtExpected, outputData);

	printf("greater or equal test\n");
	boolgen_set_op(CMP_OP_GTE, pivot);
	boolgen_start(0, 8);
	asm volatile ("fence");
	checkEqualArr(geExpected, outputData);

	return 0;
}
