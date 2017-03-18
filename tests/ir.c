#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ir-dataset.h"

uint32_t positions[NREADS];
uint8_t swaps[NREADS];

int main(void)
{
	for (int i = 0; i < NREADS; i++) {
		printf("ref %d: pos %x, swap %d\n", i, refPositions[i], refSwaps[i]);
	}

	asm volatile ("fence");
	ir_set_addr(ADDR_CONSENSUS, refConsensus);
	ir_set_addr(ADDR_READ, refReads);
	ir_set_addr(ADDR_QUAL, refQuality);
	ir_set_addr(ADDR_POS, positions);
	ir_set_addr(ADDR_SWAP, swaps);
	ir_set_target(refTarget);
	ir_set_len(NCONSENSUS, NREADS);
	ir_start(0);
	asm volatile ("fence");

	for (int i = 0; i < NREADS; i++) {
		if (positions[i] != refPositions[i] || swaps[i] != refSwaps[i]) {
			printf("mismatch %d: pos %x, swap %d\n", i, positions[i], swaps[i]);
			return 1;
		}
	}
	return 0;
}
