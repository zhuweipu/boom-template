#include "rocc.h"
#include <stdio.h>

char array[256];

static inline unsigned long translator_virt_to_phys(void *vaddr)
{
	unsigned long paddr;
	ROCC_INSTRUCTION_DS(1, paddr, vaddr, 0);
	return paddr;
}

int main(void)
{
	printf("%p -> %lx\n", array, translator_virt_to_phys(array));
	return 0;
}
