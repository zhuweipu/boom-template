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

#define NPARTITIONS 9
#define NCOLUMNS 3

#define SIZE_8 0
#define SIZE_16 1
#define SIZE_32 2
#define SIZE_64 3
#define SIZE_128 4
#define SIZE_256 5

#define ADDR_CONSENSUS 0
#define ADDR_READ 1
#define ADDR_QUAL 2
#define ADDR_POS 3
#define ADDR_SWAP 4

static inline void partition_start(unsigned long n)
{
	asm volatile ("custom3 0, %0, 0, 0" :: "r" (n));
}

static inline void partition_set_key(int idx, unsigned long key)
{
	asm volatile ("custom3 0, %[idx], %[key], 1" ::
			[idx] "r" (idx), [key] "r" (key));
}

static inline void partition_set_output_addr(int part, int col, void *addr)
{
	int idx = (part + 1) * (1 + NCOLUMNS) + col;
	asm volatile ("custom3 0, %[idx], %[addr], 2" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void partition_set_input_addr(int col, void *addr)
{
	asm volatile ("custom3 0, %[col], %[addr], 2" ::
			[col] "r" (col), [addr] "r" (addr));
}

static inline void partition_set_size(int col, unsigned long size)
{
	asm volatile ("custom3 0, %[col], %[size], 3" ::
			[col] "r" (col), [size] "r" (size));
}

static inline void partition_set_columns(int columns)
{
	asm volatile ("custom3 0, %[columns], 0, 4" ::
			[columns] "r" (columns));
}

static inline void stitch_start(unsigned long n)
{
	asm volatile("custom3 0, %0, 0, 8" :: "r" (n));
}

static inline void stitch_set_input_addr(int col, void *addr)
{
	asm volatile ("custom3 0, %[col], %[addr], 9" ::
			[col] "r" (col), [addr] "r" (addr));
}

static inline void stitch_set_output_addr(void *addr)
{
	int idx = NCOLUMNS;
	asm volatile ("custom3 0, %[idx], %[addr], 9" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void stitch_set_size(int col, unsigned long size)
{
	asm volatile ("custom3 0, %[col], %[size], 10" ::
			[col] "r" (col), [size] "r" (size));
}

static inline void stitch_set_columns(int columns)
{
	asm volatile ("custom3 0, %[columns], 0, 11" ::
			[columns] "r" (columns));
}

static inline void ir_set_target(unsigned int target)
{
	asm volatile ("custom3 0, %[target], 0, 18" :: [target] "r" (target));
}

static inline void ir_set_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 17" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void ir_start(unsigned long consLen, unsigned long readLen)
{
	asm volatile ("custom3 0, %[consLen], %[readLen], 16" ::
			[consLen] "r" (consLen), [readLen] "r" (readLen));
}

#endif
