#ifndef __ADAMACC_H__
#define __ADAMACC_H__

#include <stdint.h>

#define SEQUENCE_MAX_LEN 120
#define RECORD_PADDING (256 - 3 * 4 - 2 * 120 - 1)

typedef char sequence_t[SEQUENCE_MAX_LEN];

typedef struct read_record {
	uint32_t chromKey;
	uint32_t position;
	uint32_t matePosition;
	sequence_t base;
	sequence_t quality;
	char duplicate;
	char padding[RECORD_PADDING];
} read_record_t;

#define NPARTITIONS 256

static inline void partition_start(unsigned long n)
{
	asm volatile ("custom3 0, %0, 0, 0" :: "r" (n));
}

static inline void partition_set_key(int idx, unsigned long key)
{
	asm volatile ("custom3 0, %[idx], %[key], 1" ::
			[idx] "r" (idx), [key] "r" (key));
}

static inline void partition_set_output_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 2" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void partition_set_input_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 2" ::
			[idx] "r" (NPARTITIONS + idx), [addr] "r" (addr));
}

#endif
