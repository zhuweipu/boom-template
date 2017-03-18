#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

unsigned short inputData[8] = {
	0x8800, 0x9911, 0xaa22, 0xbb33, 0xcc44, 0xdd55, 0xee66, 0xff77
};
unsigned char expectedData[16] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};
unsigned char outputData[16];

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	split_set_input_addr(inputData);
	split_set_output_addr(0, outputData);
	split_set_output_addr(1, outputData + 8);
	split_set_input_size(SIZE_16);
	split_set_output_size(0, SIZE_8);
	split_set_output_size(1, SIZE_8);
	split_set_offset(0, 0);
	split_set_offset(1, 1);
	split_start(8, 2);
	asm volatile ("fence");

	for (int i = 0; i < 16; i++) {
		checkEqualInt(expectedData[i], outputData[i]);
	}

	return 0;
}
