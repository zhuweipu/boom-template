#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"

uint8_t inputa[4] = {0, 1, 0, 1};
uint8_t inputb[4] = {0, 0, 1, 1};

uint8_t expectedAnd[4] = {0, 0, 0, 1};
uint8_t expectedOr[4] = {0, 1, 1, 1};

uint8_t outputAnd[4];
uint8_t outputOr[4];

int main(void)
{
	logalu_set_input(inputa, inputb);

	logalu_set_output(LOGALU_AND, outputAnd);
	logalu_start(0, 4);

	logalu_set_output(LOGALU_OR, outputOr);
	logalu_start(0, 4);

	asm volatile ("fence");

	for (int i = 0; i < 4; i++) {
		if (expectedAnd[i] != outputAnd[i]) {
			printf("Error for AND %d\n", i);
			return 1;
		}
		if (expectedOr[i] != outputOr[i]) {
			printf("Error for OR %d\n", i);
			return 1;
		}
	}

	return 0;
}
