#include "adamacc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ir-dataset.h"

// 32 just to ensure block alignment
uint16_t positions[32];
uint16_t scores[32];

int main(void)
{
	asm volatile ("fence");
	ir_set_addr(ADDR_CONSENSUS, refConsensus);
	ir_set_addr(ADDR_READ, refReads);
	ir_set_addr(ADDR_QUAL, refQuality);
	ir_set_addr(ADDR_SCORE, scores);
	ir_set_addr(ADDR_POS, positions);
	ir_start(NCONSENSUS, NREADS);
	asm volatile ("fence");

	for (int i = 0; i < NCONSENSUS * NREADS; i++) {
		if (positions[i] != refPositions[i] || scores[i] != refScores[i]) {
			printf("mismatch %d: pos %d, score %d\n", i, positions[i], scores[i]);
			return 1;
		}
	}
	return 0;
}
