#ifndef RVM_MEMORY_H
#define RVM_MEMORY_H

#include <stdint.h>
#include <assert.h>

#define RVM_MEMORY_SIZE 0x10000
#define RVM_MEMORY_PROGRAM_START 0x0000
#define RVM_MEMORY_PROGRAM_END 0x7FFF
#define RVM_MEMORY_HEAP_START 0x8000
#define RVM_MEMORY_HEAP_END 0xEFFF
#define RVM_MEMORY_STACK_START 0xF000
#define RVM_MEMORY_STACK_END 0xFEFF

typedef struct {
	uint8_t data[RVM_MEMORY_SIZE];
} rvm_memory_t;

static inline uint8_t rvm_memory_read_uint8(const rvm_memory_t *mem, uint16_t offset)
{
	return mem->data[offset];
}

static inline void rvm_memory_write_uint8(rvm_memory_t *mem, uint16_t offset, uint8_t data)
{
	mem->data[offset] = data;
}

static inline uint16_t rvm_memory_read_uint16(const rvm_memory_t *mem, uint16_t offset)
{
	assert(offset < RVM_MEMORY_SIZE - 1 && "rvm_memory_read_uint16 offset out of bounds");
	uint8_t low = mem->data[offset];
	uint8_t high = mem->data[offset + 1];
	return (uint16_t)(high << 8) | low;
}

static inline void rvm_memory_write_uint16(rvm_memory_t *mem, uint16_t offset, uint16_t data)
{
	assert(offset < RVM_MEMORY_SIZE - 1 && "rvm_memory_write_uint16 offset out of bounds");
	uint8_t low = (uint8_t)(data & 0xFF);
	uint8_t high = (uint8_t)((data >> 8) & 0xFF);
	mem->data[offset] = low;
	mem->data[offset + 1] = high;
}

#endif
