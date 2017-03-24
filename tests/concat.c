#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char inputData[16] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};
unsigned short expectedData[8] = {
	0x8800, 0x9911, 0xaa22, 0xbb33, 0xcc44, 0xdd55, 0xee66, 0xff77
};
unsigned short outputData[8];

void checkEqualInt(int expected, int actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	concat_set_input_addr(0, inputData);
	concat_set_input_addr(1, inputData + 8);
	concat_set_output_addr(outputData);
	concat_set_size(0, SIZE_8);
	concat_set_size(1, SIZE_8);
	concat_set_ncolumns(2);
	concat_start(0, 8);
	asm volatile ("fence");

	for (int i = 0; i < 8; i++) {
		checkEqualInt(expectedData[i], outputData[i]);
	}

	return 0;
}
