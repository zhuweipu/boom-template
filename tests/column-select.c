#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"

int inputData[50] = {
	0xFFAA, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0xF235, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0xC135, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0xA623, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x5910, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int outputData[5];

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	column_select_set_addr(inputData, outputData);
	column_select_set_options(SIZE_32, sizeof(int) * 9);
	column_select_start(0, 5);
	asm volatile ("fence");

	for (int i = 0; i < 5; i++) {
		checkEqualInt(inputData[i * 10], outputData[i]);
	}
	return 0;
}
