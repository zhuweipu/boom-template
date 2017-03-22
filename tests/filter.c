#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"

unsigned int inputData[8] = {2, 24, 92, 13, 5444, 36, 1256, 78};
unsigned char select[8] = {0, 0, 1, 0, 1, 1, 0, 1};
unsigned int outputData[4];
unsigned int expectedData[4] = {92, 5444, 36, 78};

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	int count;

	filter_set_input_addr(inputData);
	filter_set_select_addr(select);
	filter_set_output_addr(outputData);
	filter_set_size(SIZE_32);
	count = filter_start(0, 8);
	asm volatile ("fence");

	if (count != 4) {
		printf("Expected 4 items, got %d\n", count);
		return 1;
	}

	for (int i = 0; i < 4; i++)
		checkEqualInt(expectedData[i], outputData[i]);

	return 0;
}
