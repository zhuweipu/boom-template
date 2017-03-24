#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"

uint8_t select[8] = {1, 1, 1, 0, 1, 0, 1, 0};
uint32_t inputGroups[8] = {0, 0, 0, 1, 1, 2, 3, 3};
uint32_t inputKeys[8] = {5, 2, 5, 7, 3, 1, 6, 9};
uint64_t inputAux[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

uint32_t expectedGroups[3] = {0, 1, 3};
uint32_t expectedKeys[3] = {5, 3, 6};
uint64_t expectedAux[6] = {0, 1, 8, 9, 12, 13};

uint32_t outputGroups[3];
uint32_t outputKeys[3];
uint64_t outputAux[6];

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

	aggregate_set_select_addr(select);
	aggregate_set_input_group_addr(inputGroups);
	aggregate_set_group_size(SIZE_32);
	aggregate_set_output_group_addr(outputGroups);
	aggregate_set_input_key_addr(inputKeys);
	aggregate_set_key_size(SIZE_32);
	aggregate_set_output_key_addr(outputKeys);
	aggregate_set_input_aux_addr(0, inputAux);
	aggregate_set_aux_size(0, SIZE_128);
	aggregate_set_output_aux_addr(0, outputAux);
	aggregate_set_options(1, 1);
	aggregate_start(0, 8);
	asm volatile ("fence");
	count = aggregate_get_count(0);

	if (count != 3) {
		printf("Incorrect count: %d\n", count);
		return 1;
	}

	for (int i = 0; i < 3; i++) {
		checkEqualInt(expectedGroups[i], outputGroups[i]);
		checkEqualInt(expectedKeys[i], outputKeys[i]);
	}
	for (int i = 0; i < 6; i++) {
		checkEqualInt(expectedAux[i], outputAux[i]);
	}

	return 0;
}
