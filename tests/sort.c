#include <stdio.h>
#include <stdlib.h>
#include "adamacc.h"
#include "sort-dataset.h"

uint8_t output_keys1[NUM_ELEMENTS];
uint16_t output_keys2[NUM_ELEMENTS];
uint32_t output_payload[NUM_ELEMENTS];

void checkEqualInt(int i, int expected, int actual)
{
	if (expected == actual)
		return;
	printf("%d: Expected %x, got %x\n", i, expected, actual);
	exit(EXIT_FAILURE);
}

int main(void)
{
	//for (int i = 0; i < NUM_ELEMENTS; i++) {
	//	printf("%d - key1: %02x, key2: %04x, payload: %08x\n",
	//		i, expected_keys1[i], expected_keys2[i],
	//		expected_payload[i]);
	//}

	sort_set_columns(2, 1);
	sort_set_input_key_addr(0, input_keys1);
	sort_set_input_key_addr(1, input_keys2);
	sort_set_input_payload_addr(0, input_payload);
	sort_set_output_key_addr(0, output_keys1);
	sort_set_output_key_addr(1, output_keys2);
	sort_set_output_payload_addr(0, output_payload);
	sort_set_key_size(0, SIZE_8);
	sort_set_key_size(1, SIZE_16);
	sort_set_payload_size(0, SIZE_32);
	sort_start(0, NUM_ELEMENTS);
	asm volatile ("fence");

	for (int i = 0; i < NUM_ELEMENTS; i++) {
		checkEqualInt(i, expected_keys1[i], output_keys1[i]);
		checkEqualInt(i, expected_keys2[i], output_keys2[i]);
		checkEqualInt(i, expected_payload[i], output_payload[i]);
	}

	return 0;
}
