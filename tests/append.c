#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

int inputData[16] = {
	0xab, 0xdd, 0x24, 0x83, 0x23, 0x18, 0x91, 0x25,
	0xee, 0xf2, 0x56, 0xa2, 0xf2, 0xaa, 0x21, 0x92
};

int expectedData[8] = {
	0xab, 0x24, 0x83, 0x91, 0xee, 0xf2, 0x21, 0x92
};

unsigned long chunks[5][2] = {
	{0, 1},
	{2, 2},
	{6, 1},
	{8, 2},
	{14, 2}
};

int outputData[8];

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	append_set_output_addr(outputData);
	for (int i = 0; i < 5; i++) {
		append_set_input_addr(i, inputData + chunks[i][0]);
		append_set_len(i, chunks[i][1]);
	}
	append_set_options(5, SIZE_32);
	append_start(0);
	asm volatile ("fence");

	for (int i = 0; i < 8; i++) {
		checkEqualInt(expectedData[i], outputData[i]);
	}

	return 0;
}
