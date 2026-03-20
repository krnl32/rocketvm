#include "rocketasm/encoder.h"

#include <rocketvm/common/isa/encoder.h>
#include <rocketvm/common/utility/logger.h>

int rsm_resolve_labels(const rsm_program_t *program)
{
	for (size_t i = 0; i < program->instr_count; i++) {
		rsm_instr_t *instr = &program->instrs[i];

		if (instr->opr1.type == RSM_OPERAND_LABEL) {
			int addr = rsm_program_find_label_address(program, instr->opr1.value.label, instr->opr1.label_size);
			if (addr < 0) {
				rvm_info("undefined label: %*.s\n", instr->opr1.label_size, instr->opr1.value.label);
				return -1;
			}
		}

		if (instr->opr2.type == RSM_OPERAND_LABEL) {
			int addr = rsm_program_find_label_address(program, instr->opr2.value.label, instr->opr2.label_size);
			if (addr < 0) {
				rvm_info("undefined label: %*.s\n", instr->opr2.label_size, instr->opr2.value.label);
				return -1;
			}
		}
	}

	return 0;
}

int rsm_resolve_label(const rsm_program_t *program, const rsm_operand_t *opr, size_t instr_idx)
{
	int addr = rsm_program_find_label_address(program, opr->value.label, opr->label_size);
	if (addr < 0) {
		rvm_info("undefined label: %*.s\n", opr->value.label, opr->label_size);
		return -1;
	}

	return (addr - (int)instr_idx - 1) * 2;
}

int rsm_encode_instr(const rsm_program_t *program, const rsm_instr_t *instr, size_t instr_idx)
{
	const rsm_operand_t *a = &instr->opr1;
	const rsm_operand_t *b = &instr->opr2;

	switch (instr->opcode) {
			// MOV
		case RVM_OP_MOV: {
			if (a->type == RSM_OPERAND_REG && b->type == RSM_OPERAND_IMM) {
				return rvm_mov_imm(a->value.reg, (uint8_t)b->value.imm);
			}
			if (a->type == RSM_OPERAND_REG && b->type == RSM_OPERAND_REG) {
				return rvm_mov_reg(a->value.reg, b->value.reg);
			}
			break;
		}

			// LOAD
		case RVM_OP_LOAD: {
			if (a->type == RSM_OPERAND_REG && b->type == RSM_OPERAND_MEM) {
				switch (b->mem_type) {
					case RSM_MEM_IMM:
						return rvm_load_addr(a->value.reg, (uint8_t)b->value.imm);

					case RSM_MEM_REG:
						return rvm_load_reg(a->value.reg, b->value.reg);

					case RSM_MEM_LABEL: {
						int addr = rsm_program_find_label_address(program, b->value.label, b->label_size);
						if (addr < 0) {
							rvm_info("undefined label: %*.s\n", b->value.label, b->label_size);
							return -1;
						}
						return rvm_load_addr(a->value.reg, (uint8_t)addr);
					}

					case RSM_MEM_NONE:
					default:
						break;
				}
			}
			break;
		}

			// STORE
		case RVM_OP_STORE: {
			if (a->type == RSM_OPERAND_MEM && b->type == RSM_OPERAND_REG) {
				switch (a->mem_type) {
					case RSM_MEM_IMM:
						return rvm_store_addr(b->value.reg, (uint8_t)a->value.imm);

					case RSM_MEM_REG:
						return rvm_store_reg(b->value.reg, a->value.reg);

					case RSM_MEM_LABEL: {
						int addr = rsm_program_find_label_address(program, a->value.label, a->label_size);
						if (addr < 0) {
							rvm_info("undefined label: %*.s\n", a->value.label, a->label_size);
							return -1;
						}
						return rvm_store_addr(b->value.reg, (uint8_t)addr);
					}

					case RSM_MEM_NONE:
					default:
						break;
				}
			}
			break;
		}

			// ALU + CMP
		case RVM_OP_ADD:
		case RVM_OP_SUB:
		case RVM_OP_MUL:
		case RVM_OP_DIV:
		case RVM_OP_MOD:
		case RVM_OP_SHL:
		case RVM_OP_SHR:
		case RVM_OP_CMP: {
			if (a->type != RSM_OPERAND_REG) {
				break;
			}

			rvm_reg_t rvd = a->value.reg;

			if (b->type == RSM_OPERAND_IMM) {
				uint8_t imm = (uint8_t)b->value.imm;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
				switch (instr->opcode) {
					case RVM_OP_ADD:
						return rvm_add_imm(rvd, imm);
					case RVM_OP_SUB:
						return rvm_sub_imm(rvd, imm);
					case RVM_OP_MUL:
						return rvm_mul_imm(rvd, imm);
					case RVM_OP_DIV:
						return rvm_div_imm(rvd, imm);
					case RVM_OP_MOD:
						return rvm_mod_imm(rvd, imm);
					case RVM_OP_SHL:
						return rvm_shl_imm(rvd, imm);
					case RVM_OP_SHR:
						return rvm_shr_imm(rvd, imm);
					case RVM_OP_CMP:
						return rvm_cmp_imm(rvd, imm);
					default:
						break;
				}
			}

			if (b->type == RSM_OPERAND_REG) {
				rvm_reg_t rvs = b->value.reg;

				switch (instr->opcode) {
					case RVM_OP_ADD:
						return rvm_add_reg(rvd, rvs);
					case RVM_OP_SUB:
						return rvm_sub_reg(rvd, rvs);
					case RVM_OP_MUL:
						return rvm_mul_reg(rvd, rvs);
					case RVM_OP_DIV:
						return rvm_div_reg(rvd, rvs);
					case RVM_OP_MOD:
						return rvm_mod_reg(rvd, rvs);
					case RVM_OP_SHL:
						return rvm_shl_reg(rvd, rvs);
					case RVM_OP_SHR:
						return rvm_shr_reg(rvd, rvs);
					case RVM_OP_CMP:
						return rvm_cmp_reg(rvd, rvs);
					default:
						break;
				}
			}

			break;
		}

			// CTRLF
		case RVM_OP_JMP:
		case RVM_OP_JZ:
		case RVM_OP_JNZ:
		case RVM_OP_CALL: {
			// LABEL -> Relative
			if (a->type == RSM_OPERAND_LABEL) {
				int simm = rsm_resolve_label(program, a, instr_idx);
				if (simm == -1) {
					rvm_error("rsm_resolver_resolve_label failed");
					return -1;
				}

				switch (instr->opcode) {
					case RVM_OP_JMP:
						return rvm_jmp_imm((int16_t)simm);
					case RVM_OP_JZ:
						return rvm_jz_imm((int16_t)simm);
					case RVM_OP_JNZ:
						return rvm_jnz_imm((int16_t)simm);
					case RVM_OP_CALL:
						return rvm_call_imm((int16_t)simm);
					default:
						break;
				}
			}

			// REG -> Absolute
			if (a->type == RSM_OPERAND_REG) {
				switch (instr->opcode) {
					case RVM_OP_JMP:
						return rvm_jmp_reg(a->value.reg);
					case RVM_OP_JZ:
						return rvm_jz_reg(a->value.reg);
					case RVM_OP_JNZ:
						return rvm_jnz_reg(a->value.reg);
					case RVM_OP_CALL:
						return rvm_call_reg(a->value.reg);
					default:
						break;
				}
			}
#pragma GCC diagnostic pop
			break;
		}

			// RET
		case RVM_OP_RET: {
			return rvm_ret();
		}

		default: {
			break;
		}
	}

	rvm_error("invalid instruction encoding (opcode=%d)", instr->opcode);
	return -1;
}
