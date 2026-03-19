#include "rocketasm/lexer.h"

#include <rocketvm/common/utility/logger.h>
#include <rocketvm/common/isa/utility.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define RSM_INVALID_OPCODE (rvm_opcode_t)(-1)
#define RSM_INVALID_REG (rvm_reg_t)(-1)

static inline char rsm_tolower(char ch)
{
	return ch | 0x20;
}

static rvm_opcode_t rsm_lexer_tokenize_opcode(const char *str, size_t n);
static rvm_reg_t rsm_lexer_tokenize_reg(const char *str, size_t n);

rsm_lexer_t *rsm_lexer_create(char *src, size_t size)
{
	rsm_lexer_t *lexer = malloc(sizeof(*lexer));
	if (!lexer) {
		perror("malloc");
		return NULL;
	}

	lexer->src = src;
	lexer->size = size;
	lexer->pos = 0;
	return lexer;
}

void rsm_lexer_destroy(rsm_lexer_t *lexer)
{
	if (lexer) {
		free(lexer->src);
		free(lexer);
	}
}

rsm_token_t rsm_lexer_tokenize(rsm_lexer_t *lexer)
{
	const char *src = lexer->src;

	while (1) {
		while (lexer->pos < lexer->size && isspace((unsigned char)src[lexer->pos]) && src[lexer->pos] != '\n') {
			lexer->pos++;
		}

		if (lexer->pos >= lexer->size) {
			return (rsm_token_t){ .type = RSM_TOKEN_EOF };
		}

		if (src[lexer->pos] == ';') {
			while (lexer->pos < lexer->size && src[lexer->pos] != '\n') {
				lexer->pos++;
			}
			continue;
		}

		break;
	}

	char ch = src[lexer->pos];
	if (ch == ',') {
		lexer->pos++;
		return (rsm_token_t){ .type = RSM_TOKEN_COMMA, .size = 1 };
	}
	if (ch == ':') {
		lexer->pos++;
		return (rsm_token_t){ .type = RSM_TOKEN_COLON, .size = 1 };
	}
	if (ch == '\n') {
		lexer->pos++;
		return (rsm_token_t){ .type = RSM_TOKEN_NEWLINE, .size = 1 };
	}

	if (isalpha(ch) || ch == '_') {
		size_t start = lexer->pos;

		while ((lexer->pos < lexer->size) && (isalnum(src[lexer->pos]) || src[lexer->pos] == '_')) {
			lexer->pos++;
		}

		size_t size = (lexer->pos - start);

		rvm_opcode_t opcode = rsm_lexer_tokenize_opcode(&src[start], size);
		if (opcode != RSM_INVALID_OPCODE) {
			return (rsm_token_t){ .type = RSM_TOKEN_OPCODE, .value.opcode = opcode, .size = size };
		}

		rvm_reg_t reg = rsm_lexer_tokenize_reg(&src[start], size);
		if (reg != RSM_INVALID_REG) {
			return (rsm_token_t){ .type = RSM_TOKEN_REG, .value.reg = reg, .size = size };
		}

		return (rsm_token_t){ .type = RSM_TOKEN_IDENT, .value.ident = &src[start], .size = size };
	}

	if (isdigit(ch)) {
		size_t start = lexer->pos;

		char *end;
		long value = strtol(&src[start], &end, 0);

		if (end == &src[start]) {
			rvm_error("invalid number");
			return (rsm_token_t){ .type = RSM_TOKEN_INVALID };
		}

		size_t size = (size_t)(end - &src[start]);
		lexer->pos += size;

		if (value < INT32_MIN || value > INT32_MAX) {
			rvm_error("number out of range");
			return (rsm_token_t){ .type = RSM_TOKEN_INVALID };
		}

		return (rsm_token_t){ .type = RSM_TOKEN_NUMBER, .value.number = (int32_t)value, .size = size };
	}

	rvm_error("unexpected character: %c", ch);
	lexer->pos++;
	return (rsm_token_t){ .type = RSM_TOKEN_INVALID };
}

void rsm_lexer_dump_token(const rsm_token_t *token)
{
	switch (token->type) {
		case RSM_TOKEN_INVALID: {
			break;
		}
		case RSM_TOKEN_OPCODE: {
			rvm_info("Token: OPCODE -> %s", rvm_opcode_to_string(token->value.opcode));
			break;
		}
		case RSM_TOKEN_REG: {
			rvm_info("Token: REG -> %s", rvm_reg_to_string(token->value.reg));
			break;
		}
		case RSM_TOKEN_IDENT: {
			rvm_info("Token: IDENT -> %.*s", token->size, token->value.ident);
			break;
		}
		case RSM_TOKEN_NUMBER: {
			rvm_info("Token: NUMBER -> %d", token->value.number);
			break;
		}
		case RSM_TOKEN_COMMA: {
			rvm_info("Token: COMMA");
			break;
		}
		case RSM_TOKEN_COLON: {
			rvm_info("Token: COLON");
			break;
		}
		case RSM_TOKEN_NEWLINE: {
			rvm_info("Token: NEWLINE");
			break;
		}
		case RSM_TOKEN_EOF: {
			rvm_info("Token: EOF");
			break;
		}
		default:
			break;
	}
}

const char *rsm_token_type_to_string(rsm_token_type_t type)
{
	switch (type) {
		case RSM_TOKEN_INVALID: {
			return "INVALID_TOKEN";
		}
		case RSM_TOKEN_OPCODE: {
			return "OPCODE";
		}
		case RSM_TOKEN_REG: {
			return "REG";
		}
		case RSM_TOKEN_IDENT: {
			return "IDENT";
		}
		case RSM_TOKEN_NUMBER: {
			return "NUMBER";
		}
		case RSM_TOKEN_COMMA: {
			return "COMMA";
		}
		case RSM_TOKEN_COLON: {
			return "COLON";
		}
		case RSM_TOKEN_NEWLINE: {
			return "NEWLINE";
		}
		case RSM_TOKEN_EOF: {
			return "EOF";
		}
		default:
			return "UNKNOWN_TOKEN";
	}
}

static rvm_opcode_t rsm_lexer_tokenize_opcode(const char *str, size_t n)
{
	switch (n) {
			// JZ
		case 2: {
			if (rsm_tolower(str[0]) == 'j' && rsm_tolower(str[1]) == 'z') {
				return RVM_OP_JZ;
			}
			break;
		}

			// MOV, ADD, SUB, MUL, DIV, MOD, SHL, SHR, CMP, JMP, JNZ, RET
		case 3: {
			switch (rsm_tolower(str[0])) {
				case 'm': {
					if (rsm_tolower(str[1]) == 'o' && rsm_tolower(str[2]) == 'v') {
						return RVM_OP_MOV;
					}
					if (rsm_tolower(str[1]) == 'u' && rsm_tolower(str[2]) == 'l') {
						return RVM_OP_MUL;
					}
					if (rsm_tolower(str[1]) == 'o' && rsm_tolower(str[2]) == 'd') {
						return RVM_OP_MOD;
					}
					break;
				}

				case 'a': {
					if (rsm_tolower(str[1]) == 'd' && rsm_tolower(str[2]) == 'd') {
						return RVM_OP_ADD;
					}
					break;
				}

				case 's': {
					if (rsm_tolower(str[1]) == 'u' && rsm_tolower(str[2]) == 'b') {
						return RVM_OP_SUB;
					}
					if (rsm_tolower(str[1]) == 'h' && rsm_tolower(str[2]) == 'l') {
						return RVM_OP_SHL;
					}
					if (rsm_tolower(str[1]) == 'h' && rsm_tolower(str[2]) == 'r') {
						return RVM_OP_SHR;
					}
					break;
				}

				case 'd': {
					if (rsm_tolower(str[1]) == 'i' && rsm_tolower(str[2]) == 'v') {
						return RVM_OP_DIV;
					}
					break;
				}

				case 'c': {
					if (rsm_tolower(str[1]) == 'm' && rsm_tolower(str[2]) == 'p') {
						return RVM_OP_CMP;
					}
					break;
				}

				case 'j': {
					if (rsm_tolower(str[1]) == 'm' && rsm_tolower(str[2]) == 'p') {
						return RVM_OP_JMP;
					}
					if (rsm_tolower(str[1]) == 'n' && rsm_tolower(str[2]) == 'z') {
						return RVM_OP_JNZ;
					}
					break;
				}

				case 'r': {
					if (rsm_tolower(str[1]) == 'e' && rsm_tolower(str[2]) == 't') {
						return RVM_OP_RET;
					}
					break;
				}
				default:
					break;
			}
			break;
		}

			// LOAD, CALL
		case 4: {
			if (rsm_tolower(str[0]) == 'l' && rsm_tolower(str[1]) == 'o' && rsm_tolower(str[2]) == 'a' && rsm_tolower(str[3]) == 'd') {
				return RVM_OP_LOAD;
			}

			if (rsm_tolower(str[0]) == 'c' && rsm_tolower(str[1]) == 'a' && rsm_tolower(str[2]) == 'l' && rsm_tolower(str[3]) == 'l') {
				return RVM_OP_CALL;
			}
			break;
		}

			// STORE
		case 5: {
			if (rsm_tolower(str[0]) == 's' && rsm_tolower(str[1]) == 't' && rsm_tolower(str[2]) == 'o' && rsm_tolower(str[3]) == 'r' && rsm_tolower(str[4]) == 'e') {
				return RVM_OP_STORE;
			}
			break;
		}

		default:
			break;
	}

	return RSM_INVALID_OPCODE;
}

static rvm_reg_t rsm_lexer_tokenize_reg(const char *str, size_t n)
{
	// rv0...rv7
	if (n == 3 && rsm_tolower(str[0]) == 'r' && rsm_tolower(str[1]) == 'v') {
		if (str[2] >= '0' && str[2] <= '7') {
			return (rvm_reg_t)(str[2] - '0');
		}
	}
	return RSM_INVALID_REG;
}
