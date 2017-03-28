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

#define NPARTITIONS 8
#define NPARTCOLUMNS 3
#define NSTITCHCOLUMNS 3
#define NBRAIDCOLUMNS 2
#define NSORTKEYS 2
#define NSORTCOLUMNS 5
#define NAGGAUX 1

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

#define CMP_OP_EQUAL 0
#define CMP_OP_LESS 1
#define CMP_OP_LTE 2
#define CMP_OP_GREATER 3
#define CMP_OP_GTE 4

#define REDUCE_SUM 0
#define REDUCE_MIN 1
#define REDUCE_MAX 2

#define LOGALU_AND 0
#define LOGALU_OR 1

static inline void partition_start(int id, unsigned long n)
{
	asm volatile ("custom3 0, %[id], %[n], 0" ::
			[id] "r" (id), [n] "r" (n));
}

static inline void partition_set_key(int idx, unsigned long key)
{
	asm volatile ("custom3 0, %[idx], %[key], 1" ::
			[idx] "r" (idx), [key] "r" (key));
}

static inline void partition_set_output_addr(int part, int col, void *addr)
{
	int idx = (part + 1) * (1 + NPARTCOLUMNS) + col;
	asm volatile ("custom3 0, %[idx], %[addr], 2" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void partition_set_input_addr(int col, void *addr)
{
	asm volatile ("custom3 0, %[col], %[addr], 2" ::
			[col] "r" (col), [addr] "r" (addr));
}

static inline void partition_set_size(int col, int size)
{
	asm volatile ("custom3 0, %[col], %[size], 3" ::
			[col] "r" (col), [size] "r" (size));
}

static inline void partition_set_columns(int columns)
{
	asm volatile ("custom3 0, %[columns], 0, 4" ::
			[columns] "r" (columns));
}

static inline unsigned long partition_get_count(int id, int part)
{
	unsigned long count;
	asm volatile ("custom3 %[count], %[id], %[part], 5" :
			[count] "=r" (count) :
			[id] "r" (id),
			[part] "r" (part));
	return count;
}

static inline void concat_start(int id, unsigned long ninput)
{
	asm volatile("custom3 0, %[id], %[ninput], 8" ::
			[id] "r" (id), [ninput] "r" (ninput));
}

static inline void concat_set_input_addr(int col, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 9" ::
			[idx] "r" (col + 1), [addr] "r" (addr));
}

static inline void concat_set_output_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 9" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void concat_set_size(int col, int size)
{
	asm volatile ("custom3 0, %[col], %[size], 10" ::
			[col] "r" (col), [size] "r" (size));
}

static inline void concat_set_ncolumns(unsigned long ncolumns)
{
	asm volatile ("custom3 0, %[ncolumns], 0, 11" ::
			[ncolumns] "r" (ncolumns));
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

static inline void ir_set_len(unsigned long consLen, unsigned long readLen)
{
	asm volatile ("custom3 0, %[consLen], %[readLen], 19" ::
			[consLen] "r" (consLen), [readLen] "r" (readLen));
}

static inline void ir_start(int id)
{
	asm volatile ("custom3 0, %[id], 0, 16" :: [id] "r" (id));
}

static inline void cond_update_set_select_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 25" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void cond_update_set_input_addr(int column, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 25" ::
			[idx] "r" (1 + column), [addr] "r" (addr));
}

static inline void cond_update_set_output_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 25" ::
			[idx] "r" (1 + NBRAIDCOLUMNS), [addr] "r" (addr));
}

static inline void cond_update_set_size(int size)
{
	asm volatile ("custom3 0, %[size], 0, 26" :: [size] "r" (size));
}

static inline void cond_update_start(int idx, unsigned long len)
{
	asm volatile ("custom3 0, %[idx], %[len], 24" ::
			[idx] "r" (idx), [len] "r" (len));
}

static inline void append_start(int id)
{
	asm volatile ("custom3 0, %[id], 0, 32" :: [id] "r" (id));
}

static inline void append_set_output_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 33" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void append_set_input_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 33" ::
			[idx] "r" (idx + 1), [addr] "r" (addr));
}

static inline void append_set_len(int idx, unsigned long len)
{
	asm volatile ("custom3 0, %[idx], %[len], 34" ::
			[idx] "r" (idx), [len] "r" (len));
}

static inline void append_set_options(int npieces, int size)
{
	asm volatile ("custom3 0, %[npieces], %[size], 35" ::
			[npieces] "r" (npieces), [size] "r" (size));
}


static inline void split_set_input_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 41" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void split_set_output_addr(int col, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 41" ::
			[idx] "r" (col + 1), [addr] "r" (addr));
}

static inline void split_set_input_size(int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 42" ::
			[idx] "r" (0), [size] "r" (size));
}

static inline void split_set_output_size(int col, int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 42" ::
			[idx] "r" (col + 1), [size] "r" (size));
}

static inline void split_set_offset(int col, int offset)
{
	asm volatile ("custom3 0, %[col], %[offset], 43" ::
			[col] "r" (col), [offset] "r" (offset));
}

static inline void split_set_ncolumns(int ncolumns)
{
	asm volatile ("custom3 0, %[ncolumns], 0, 44" ::
			[ncolumns] "r" (ncolumns));
}

static inline void split_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 40" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void sort_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 48" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void sort_set_input_key_addr(int idx, void* addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 49" ::
			[idx] "r" (idx), [addr] "r" (addr));
}

static inline void sort_set_input_payload_addr(int idx, void* addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 49" ::
			[idx] "r" (NSORTKEYS + idx), [addr] "r" (addr));
}

static inline void sort_set_output_key_addr(int idx, void* addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 49" ::
			[idx] "r" (NSORTCOLUMNS + idx), [addr] "r" (addr));
}

static inline void sort_set_output_payload_addr(int idx, void* addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 49" ::
			[idx] "r" (NSORTCOLUMNS + NSORTKEYS + idx),
			[addr] "r" (addr));
}

static inline void sort_set_key_size(int idx, int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 50" ::
			[idx] "r" (idx), [size] "r" (size));
}

static inline void sort_set_payload_size(int idx, int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 50" ::
			[idx] "r" (NSORTKEYS + idx), [size] "r" (size));
}

static inline void sort_set_columns(int nkeys, int npayload)
{
	asm volatile ("custom3 0, %[nkeys], %[npayload], 51" ::
			[nkeys] "r" (nkeys), [npayload] "r" (npayload));
}

static inline void boolgen_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 56" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void boolgen_set_addrs(void *inAddr, void *outAddr)
{
	asm volatile ("custom3 0, %[inAddr], %[outAddr], 57" ::
			[inAddr] "r" (inAddr), [outAddr] "r" (outAddr));
}

static inline void boolgen_set_size(int size)
{
	asm volatile ("custom3 0, %[size], 0, 58" :: [size] "r" (size));
}

static inline void boolgen_set_op(int typ, unsigned long value)
{
	asm volatile ("custom3 0, %[typ], %[value], 59" ::
			[typ] "r" (typ), [value] "r" (value));
}

static inline unsigned long filter_start(int id, unsigned long len)
{
	unsigned long count;
	asm volatile ("custom3 %[count], %[id], %[len], 64" :
			[count] "=r" (count) :
			[id] "r" (id), [len] "r" (len));
	return count;
}

static inline void filter_set_input_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 65" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void filter_set_select_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 65" ::
			[idx] "r" (1), [addr] "r" (addr));
}

static inline void filter_set_output_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 65" ::
			[idx] "r" (2), [addr] "r" (addr));
}

static inline void filter_set_size(int size)
{
	asm volatile ("custom3 0, %[size], 0, 66" :: [size] "r" (size));
}

static inline void reduce_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 72" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void reduce_set_addr(void *inAddr, void *outAddr)
{
	asm volatile ("custom3 0, %[inAddr], %[outAddr], 73" ::
			[inAddr] "r" (inAddr), [outAddr] "r" (outAddr));
}

static inline void reduce_set_size(int inSize, int outSize)
{
	asm volatile ("custom3 0, %[inSize], %[outSize], 74" ::
			[inSize] "r" (inSize), [outSize] "r" (outSize));
}

static inline void reduce_set_comparator(int op, int value)
{
	asm volatile ("custom3 0, %[op], %[value], 75" ::
			[op] "r" (op), [value] "r" (value));
}

static inline void reduce_set_operation(int op)
{
	asm volatile ("custom3 0, %[op], 0, 76" :: [op] "r" (op));
}

static inline void aggregate_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 80" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void aggregate_set_input_group_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (0), [addr] "r" (addr));
}

static inline void aggregate_set_input_key_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (1), [addr] "r" (addr));
}

static inline void aggregate_set_select_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (2), [addr] "r" (addr));
}

static inline void aggregate_set_input_aux_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (3 + idx), [addr] "r" (addr));
}

static inline void aggregate_set_output_group_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (3 + NAGGAUX), [addr] "r" (addr));
}

static inline void aggregate_set_output_key_addr(void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (4 + NAGGAUX), [addr] "r" (addr));
}

static inline void aggregate_set_output_aux_addr(int idx, void *addr)
{
	asm volatile ("custom3 0, %[idx], %[addr], 81" ::
			[idx] "r" (5 + NAGGAUX + idx), [addr] "r" (addr));
}

static inline void aggregate_set_group_size(int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 82" ::
			[idx] "r" (0), [size] "r" (size));
}

static inline void aggregate_set_key_size(int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 82" ::
			[idx] "r" (1), [size] "r" (size));
}

static inline void aggregate_set_aux_size(int idx, int size)
{
	asm volatile ("custom3 0, %[idx], %[size], 82" ::
			[idx] "r" (2 + idx), [size] "r" (size));
}

static inline void aggregate_set_options(int up, int naux)
{
	asm volatile ("custom3 0, %[up], %[naux], 83" ::
			[up] "r" (up), [naux] "r" (naux));
}

static inline unsigned long aggregate_get_count(int id)
{
	unsigned long count;
	asm volatile ("custom3 %[count], %[id], 0, 84" :
			[count] "=r" (count) :
			[id] "r" (id));
	return count;
}

static inline void logalu_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 88" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void logalu_set_input(void *addr1, void *addr2)
{
	asm volatile ("custom3 0, %[addr1], %[addr2], 89" ::
			[addr1] "r" (addr1), [addr2] "r" (addr2));
}

static inline void logalu_set_output(int op, void *addr)
{
	asm volatile ("custom3 0, %[op], %[addr], 90" ::
			[op] "r" (op), [addr] "r" (addr));
}

static inline void column_select_start(int id, unsigned long len)
{
	asm volatile ("custom3 0, %[id], %[len], 96" ::
			[id] "r" (id), [len] "r" (len));
}

static inline void column_select_set_addr(void *inAddr, void *outAddr)
{
	asm volatile ("custom3 0, %[inAddr], %[outAddr], 97" ::
			[inAddr] "r" (inAddr), [outAddr] "r" (outAddr));
}

static inline void column_select_set_options(int size, unsigned long stride)
{
	asm volatile ("custom3 0, %[size], %[stride], 98" ::
			[size] "r" (size), [stride] "r" (stride));
}

#endif
