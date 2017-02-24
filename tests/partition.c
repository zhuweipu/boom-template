#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>

read_record_t records[4];

uint32_t chromKey[4];
uint32_t position[4];
uint32_t matePosition[4];
sequence_t base[4];
sequence_t quality[4];
char duplicate[4];

void checkEqual(uint32_t expected, uint32_t actual)
{
	if (expected == actual)
		return;
	printf("Expected %d, got %d\n", expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	partition_set_key(0, 1L << 32);
	partition_set_key(1, 2L << 32);

	partition_set_output_addr(0, records);
	partition_set_output_addr(1, records + 2);

	partition_set_input_addr(0, chromKey);
	partition_set_input_addr(1, position);
	partition_set_input_addr(2, matePosition);
	partition_set_input_addr(3, base);
	partition_set_input_addr(4, quality);
	partition_set_input_addr(5, duplicate);

	chromKey[0] = chromKey[2] = 0;
	chromKey[1] = chromKey[3] = 1;

	position[0] = 43;
	position[1] = 61;
	position[2] = 78;
	position[3] = 23;

	matePosition[0] = 51;
	matePosition[1] = 91;
	matePosition[2] = 25;
	matePosition[3] = 12;

	duplicate[0] = 1;
	duplicate[1] = 0;
	duplicate[2] = 0;
	duplicate[3] = 0;

	partition_start(4);
	asm volatile ("fence");

	for (int i = 0; i < 4; i++) {
		int j = ((i & 2) >> 1) | ((i & 1) << 1);
		checkEqual(chromKey[i], records[j].chromKey);
		checkEqual(position[i], records[j].position);
		checkEqual(matePosition[i], records[j].matePosition);
		checkEqual(duplicate[i], records[j].duplicate);
	}

	return 0;
}
