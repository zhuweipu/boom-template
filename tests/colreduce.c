#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"
#include "colreduce-dataset.h"

int outputSum[QUAL_LEN];
int outputMin[QUAL_LEN];
int outputMax[QUAL_LEN];

void compareArray(int *expected, int *actual, int n)
{
	for (int i = 0; i < n; i++) {
		if (expected[i] != actual[i])
			exit(EXIT_FAILURE);
	}
}

int main(void)
{
	reduce_set_size(QUAL_SIZE, 2);
	reduce_set_comparator(CMP_OP_GREATER, 0);

	reduce_set_addr(inputQuality, outputSum);
	reduce_set_operation(REDUCE_SUM);
	reduce_start(0, QUAL_LEN);

	reduce_set_addr(inputQuality, outputMin);
	reduce_set_operation(REDUCE_MIN);
	reduce_start(0, QUAL_LEN);

	reduce_set_addr(inputQuality, outputMax);
	reduce_set_operation(REDUCE_MAX);
	reduce_start(0, QUAL_LEN);

	asm volatile ("fence");

	printf("check sum\n");
	compareArray(expectedSum, outputSum, QUAL_LEN);
	printf("check min\n");
	compareArray(expectedMin, outputMin, QUAL_LEN);
	printf("check max\n");
	compareArray(expectedMax, outputMax, QUAL_LEN);

	return 0;
}
