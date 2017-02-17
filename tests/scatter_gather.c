#include "dma-ext.h"

int src_array[10] = {3, 6, 1, 2, 6, 1, 0, 5, 9, 2};
int dst_array[10];
int idx[5] = {0, 3, 2, 6, 9};

int main(void)
{
	int tmp_array[5];
	void *addr_tab[5];
	int ref_array[10];

	for (int i = 0; i < 5; i++)
		addr_tab[i] = src_array + idx[i];
	dma_write_cr(NSEGMENTS, 5);
	dma_write_cr(SEGMENT_SIZE, sizeof(int));
	dma_gather(tmp_array, addr_tab);
	asm volatile ("fence");

	for (int i = 0; i < 5; i++)
		addr_tab[i] = dst_array + idx[i];
	dma_scatter(addr_tab, tmp_array);
	asm volatile ("fence");

	for (int i = 0; i < 10; i++)
		ref_array[i] = 0;
	for (int i = 0 ; i < 5; i++)
		ref_array[idx[i]] = src_array[idx[i]];

	for (int i = 0; i < 10; i++) {
		if (dst_array[i] != ref_array[i])
			return i + 1;
	}

	return 0;
}
