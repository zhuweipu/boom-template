#include "dma-ext.h"

#define TEST_SIZE 600

int src_array[TEST_SIZE];
int dst_array[TEST_SIZE];

int main(void)
{
	int i;

	for (i = 0; i < TEST_SIZE; i++)
		src_array[i] = (i << 16) | i;

	dma_write_cr(SEGMENT_SIZE, TEST_SIZE * sizeof(int));
	dma_write_cr(NSEGMENTS, 1);
	dma_write_cr(SRC_STRIDE, 0);
	dma_write_cr(DST_STRIDE, 0);
	dma_transfer(dst_array, src_array);
	asm volatile ("fence");
	if (dma_read_cr(RESP_STATUS) != NO_ERROR) {
		return -1;
	}

	for (i = 0; i < TEST_SIZE; i++) {
		if (dst_array[i] != src_array[i]) {
			return -1;
		}
	}

	return 0;
}
