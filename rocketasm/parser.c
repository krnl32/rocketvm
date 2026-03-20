#include "rocketasm/parser.h"

#include <rocketvm/common/utility/logger.h>
#include <stdio.h>
#include <stdlib.h>

static void rsm_parser_advance(rsm_parser_t *parser, rsm_lexer_t *lexer);
static int rsm_parser_parse_opcode(rsm_parser_t *parser, rsm_lexer_t *lexer, rsm_program_t *program);
static rsm_operand_t rsm_parser_parse_operand(rsm_parser_t *parser, rsm_lexer_t *lexer);

rsm_parser_t *rsm_parser_create(void)
{
	rsm_parser_t *parser = malloc(sizeof(*parser));
	if (!parser) {
		perror("malloc");
		return NULL;
	}

	parser->token.type = RSM_TOKEN_INVALID;
	return parser;
}

void rsm_parser_destroy(rsm_parser_t *parser)
{
	if (parser) {
		free(parser);
	}
}

int rsm_parser_parse(rsm_parser_t *parser, rsm_lexer_t *lexer, rsm_program_t *program)
{
	rsm_parser_advance(parser, lexer);

	while (parser->token.type != RSM_TOKEN_EOF) {
		if (parser->token.type == RSM_TOKEN_INVALID) {
			rvm_error("rsm_lexer_token failed");
			return -1;
		}

		if (parser->token.type == RSM_TOKEN_IDENT) {
			const char *name = parser->token.value.ident;
			size_t name_len = parser->token.size;
			rsm_parser_advance(parser, lexer);

			if (parser->token.type == RSM_TOKEN_COLON) {
				if (program->instr_count > UINT16_MAX) {
					rvm_error("program too large for 16-bit address space");
					return -1;
				}

				if (rsm_program_push_label(program, (rsm_label_t){ .name = name, .name_len = name_len, .address = (uint16_t)program->instr_count }) == -1) {
					rvm_error("rsm_program_push_label failed");
					return -1;
				}

				parser->token = rsm_lexer_tokenize(lexer);
				continue;
			}

			rvm_error("rsm_parser_parse unexpected identifier: %*.s", name_len, name);
			return -1;
		}

		if (parser->token.type == RSM_TOKEN_OPCODE) {
			if (rsm_parser_parse_opcode(parser, lexer, program) == -1) {
				rvm_error("rsm_parser_parse_opcode failed");
				return -1;
			}
			continue;
		}

		if (parser->token.type == RSM_TOKEN_NEWLINE) {
			rsm_parser_advance(parser, lexer);
			continue;
		}

		rvm_info("unexpected token: %s", rsm_token_type_to_string(parser->token.type));
		return -1;
	}

	return 0;
}

static void rsm_parser_advance(rsm_parser_t *parser, rsm_lexer_t *lexer)
{
	parser->token = rsm_lexer_tokenize(lexer);
}

static int rsm_parser_parse_opcode(rsm_parser_t *parser, rsm_lexer_t *lexer, rsm_program_t *program)
{
	rsm_instr_t instr = { 0 };

	instr.opcode = parser->token.value.opcode;
	rsm_parser_advance(parser, lexer);

	if (instr.opcode != RVM_OP_RET) {
		// OPR1
		if (parser->token.type != RSM_TOKEN_NEWLINE && parser->token.type != RSM_TOKEN_EOF) {
			instr.opr1 = rsm_parser_parse_operand(parser, lexer);
			if (instr.opr1.type == RSM_OPERAND_INVALID) {
				return -1;
			}

			rsm_parser_advance(parser, lexer);

			// OPR2?
			if (parser->token.type == RSM_TOKEN_COMMA) {
				rsm_parser_advance(parser, lexer);

				instr.opr2 = rsm_parser_parse_operand(parser, lexer);
				if (instr.opr2.type == RSM_OPERAND_INVALID) {
					return -1;
				}

				rsm_parser_advance(parser, lexer);
			}
		}
	}

	if (parser->token.type != RSM_TOKEN_NEWLINE && parser->token.type != RSM_TOKEN_EOF) {
		rvm_error("unexpected token after instruction");
		return -1;
	}

	// RET
	if (instr.opcode == RVM_OP_RET) {
		if (instr.opr1.type != RSM_OPERAND_NONE || instr.opr2.type != RSM_OPERAND_NONE) {
			rvm_error("RET takes no operands");
			return -1;
		}
		goto finish;
	}

	// LOAD: reg, [mem]
	if (instr.opcode == RVM_OP_LOAD) {
		if (instr.opr1.type != RSM_OPERAND_REG || instr.opr2.type != RSM_OPERAND_MEM) {
			rvm_error("LOAD requires: LOAD reg, [addr]");
			return -1;
		}
	}

	// STORE: [mem], reg
	if (instr.opcode == RVM_OP_STORE) {
		if (instr.opr1.type != RSM_OPERAND_MEM || instr.opr2.type != RSM_OPERAND_REG) {
			rvm_error("STORE requires: STORE [addr], reg");
			return -1;
		}
	}

	// ALU + CMP
	if (instr.opcode == RVM_OP_ADD || instr.opcode == RVM_OP_SUB || instr.opcode == RVM_OP_MUL || instr.opcode == RVM_OP_DIV || instr.opcode == RVM_OP_MOD || instr.opcode == RVM_OP_SHL || instr.opcode == RVM_OP_SHR ||
		instr.opcode == RVM_OP_CMP) {
		if (instr.opr1.type != RSM_OPERAND_REG) {
			rvm_error("ALU ops require destination register");
			return -1;
		}

		if (!(instr.opr2.type == RSM_OPERAND_REG || instr.opr2.type == RSM_OPERAND_IMM)) {
			rvm_error("ALU ops require reg or imm operand");
			return -1;
		}
	}

	// MOV
	if (instr.opcode == RVM_OP_MOV) {
		if (instr.opr1.type != RSM_OPERAND_REG || !(instr.opr2.type == RSM_OPERAND_REG || instr.opr2.type == RSM_OPERAND_IMM)) {
			rvm_error("MOV requires: MOV reg, reg|imm");
			return -1;
		}
	}

	// CONTROL FLOW
	if (instr.opcode == RVM_OP_JMP || instr.opcode == RVM_OP_JZ || instr.opcode == RVM_OP_JNZ || instr.opcode == RVM_OP_CALL) {
		if (instr.opr1.type == RSM_OPERAND_NONE) {
			rvm_error("control flow requires operand");
			return -1;
		}

		if (!(instr.opr1.type == RSM_OPERAND_LABEL || instr.opr1.type == RSM_OPERAND_REG || instr.opr1.type == RSM_OPERAND_IMM)) {
			rvm_error("invalid control flow operand");
			return -1;
		}
	}

finish:
	if (rsm_program_push_instr(program, instr) == -1) {
		rvm_error("rsm_program_push_instr failed");
		return -1;
	}

	return 0;
}

static rsm_operand_t rsm_parser_parse_operand(rsm_parser_t *parser, rsm_lexer_t *lexer)
{
	rsm_operand_t operand = { 0 };

	if (parser->token.type == RSM_TOKEN_LBRACKET) {
		rsm_parser_advance(parser, lexer);

		operand.type = RSM_OPERAND_MEM;

		if (parser->token.type == RSM_TOKEN_NUMBER) {
			operand.mem_type = RSM_MEM_IMM;
			operand.value.imm = parser->token.value.number;
		} else if (parser->token.type == RSM_TOKEN_REG) {
			operand.mem_type = RSM_MEM_REG;
			operand.value.reg = parser->token.value.reg;
		} else if (parser->token.type == RSM_TOKEN_IDENT) {
			operand.mem_type = RSM_MEM_LABEL;
			operand.value.label = parser->token.value.ident;
			operand.label_size = parser->token.size;
		} else {
			rvm_error("invalid memory operand");
			operand.type = RSM_OPERAND_INVALID;
			return operand;
		}

		rsm_parser_advance(parser, lexer);

		if (parser->token.type != RSM_TOKEN_RBRACKET) {
			rvm_error("expected ']'");
			operand.type = RSM_OPERAND_INVALID;
			return operand;
		}

		return operand;
	}

	if (parser->token.type == RSM_TOKEN_NUMBER) {
		operand.type = RSM_OPERAND_IMM;
		operand.value.imm = parser->token.value.number;
		return operand;
	}

	if (parser->token.type == RSM_TOKEN_REG) {
		operand.type = RSM_OPERAND_REG;
		operand.value.reg = parser->token.value.reg;
		return operand;
	}

	if (parser->token.type == RSM_TOKEN_IDENT) {
		operand.type = RSM_OPERAND_LABEL;
		operand.value.label = parser->token.value.ident;
		operand.label_size = parser->token.size;
		return operand;
	}

	rvm_error("unexpected operand: %d", parser->token.type);
	operand.type = RSM_OPERAND_INVALID;
	return operand;
}
