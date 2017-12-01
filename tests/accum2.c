#include <stdio.h>
#include <stdint.h>

#include "rocc.h"

int main(void)
{
	unsigned long value, expected = 0;
	uint64_t data[5] = {2, 4, 1, 3, 5};

	for (int i = 0; i < 5; i++)
		expected += data[i];

	ROCC_INSTRUCTION_DSS(0, value, data, 5, 0);

	printf("value: %ld\n", value);
	if (value != expected) {
		printf("ERROR: value does not match expected\n");
		return 1;
	}

	return 0;
}
