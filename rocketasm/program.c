#include "rocketasm/program.h"

#include <rocketvm/common/isa/utility.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void rsm_dump_operand(const rsm_operand_t *op);

rsm_program_t *rsm_program_create(void)
{
	rsm_program_t *prog = malloc(sizeof(*prog));
	if (!prog) {
		perror("malloc");
		return NULL;
	}

	prog->instr_count = 0;
	prog->instr_capacity = 1024;
	prog->label_count = 0;
	prog->label_capacity = 1024;

	prog->instrs = malloc(sizeof(*prog->instrs) * prog->instr_capacity);
	if (!prog->instrs) {
		perror("malloc");
		free(prog);
		return NULL;
	}

	prog->labels = malloc(sizeof(*prog->labels) * prog->label_capacity);
	if (!prog->labels) {
		perror("malloc");
		free(prog->instrs);
		free(prog);
		return NULL;
	}

	return prog;
}

void rsm_program_destroy(rsm_program_t *prog)
{
	if (prog) {
		free(prog->labels);
		free(prog->instrs);
		free(prog);
	}
}

int rsm_program_push_instr(rsm_program_t *prog, rsm_instr_t instr)
{
	if (prog->instr_count >= prog->instr_capacity) {
		size_t new_cap = prog->instr_capacity * 2;
		void *tmp = realloc(prog->instrs, sizeof(*prog->instrs) * new_cap);
		if (!tmp) {
			perror("realloc");
			return -1;
		}

		prog->instr_capacity = new_cap;
		prog->instrs = tmp;
	}

	prog->instrs[prog->instr_count++] = instr;
	return 0;
}

int rsm_program_push_label(rsm_program_t *prog, rsm_label_t label)
{
	if (prog->label_count >= prog->label_capacity) {
		size_t new_cap = prog->label_capacity * 2;
		void *tmp = realloc(prog->labels, sizeof(*prog->labels) * new_cap);
		if (!tmp) {
			perror("realloc");
			return -1;
		}

		prog->label_capacity = new_cap;
		prog->labels = tmp;
	}

	prog->labels[prog->label_count++] = label;
	return 0;
}

void rsm_program_dump_instrs(const rsm_program_t *prog)
{
	printf("=== PROGRAM INSTRUCTIONS (%zu) ===\n", prog->instr_count);

	for (size_t i = 0; i < prog->instr_count; i++) {
		rsm_instr_t *instr = &prog->instrs[i];

		printf("[%04zu] ", i);
		printf("%s ", rvm_opcode_to_string(instr->opcode));

		if (instr->opr1.type != RSM_OPERAND_NONE) {
			rsm_dump_operand(&instr->opr1);
		}

		if (instr->opr2.type != RSM_OPERAND_NONE) {
			printf(", ");
			rsm_dump_operand(&instr->opr2);
		}

		printf("\n");
	}
}

void rsm_program_dump_labels(const rsm_program_t *prog)
{
	printf("=== PROGRAM LABELS (%zu) ===\n", prog->label_count);

	for (size_t i = 0; i < prog->label_count; i++) {
		rsm_label_t *label = &prog->labels[i];
		printf("[%04zu] %.*s -> 0x%04x (%u)\n", i, (int)label->name_len, label->name, label->address, label->address);
	}
}

static void rsm_dump_operand(const rsm_operand_t *op)
{
	switch (op->type) {
		case RSM_OPERAND_NONE:
			printf("none");
			break;

		case RSM_OPERAND_IMM:
			printf("%d", op->value.imm);
			break;

		case RSM_OPERAND_REG:
			printf("%s", rvm_reg_to_string(op->value.reg));
			break;

		case RSM_OPERAND_LABEL:
			printf("%.*s", (int)op->label_size, op->value.label);
			break;

		case RSM_OPERAND_INVALID:
		default:
			printf("invalid");
			break;
	}
}
