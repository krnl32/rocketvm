#ifndef RSM_PROGRAM_H
#define RSM_PROGRAM_H

#include <rocketvm/common/isa/opcode.h>
#include <rocketvm/common/isa/register.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
	const char *name;
	size_t name_len;
	uint16_t address;
} rsm_label_t;

typedef enum {
	RSM_OPERAND_NONE,
	RSM_OPERAND_IMM,
	RSM_OPERAND_REG,
	RSM_OPERAND_LABEL,
	RSM_OPERAND_MEM,
	RSM_OPERAND_INVALID,
} rsm_operand_type_t;

typedef enum {
	RSM_MEM_NONE,
	RSM_MEM_IMM,
	RSM_MEM_REG,
	RSM_MEM_LABEL,
} rsm_mem_type_t;

typedef struct {
	rsm_operand_type_t type;
	rsm_mem_type_t mem_type;
	union {
		int32_t imm;
		rvm_reg_t reg;
		const char *label;
	} value;
	size_t label_size;
} rsm_operand_t;

typedef struct {
	rvm_opcode_t opcode;
	rsm_operand_t opr1;
	rsm_operand_t opr2;
} rsm_instr_t;

typedef struct {
	rsm_instr_t *instrs;
	size_t instr_count;
	size_t instr_capacity;

	rsm_label_t *labels;
	size_t label_count;
	size_t label_capacity;
} rsm_program_t;

rsm_program_t *rsm_program_create(void);
void rsm_program_destroy(rsm_program_t *prog);
int rsm_program_push_instr(rsm_program_t *prog, rsm_instr_t instr);
int rsm_program_push_label(rsm_program_t *prog, rsm_label_t label);
int rsm_program_find_label_address(const rsm_program_t *prog, const char *name, size_t name_len);

void rsm_program_dump_instrs(const rsm_program_t *prog);
void rsm_program_dump_labels(const rsm_program_t *prog);

#endif
