#ifndef RSM_LEXER_H
#define RSM_LEXER_H

#include <rocketvm/common/isa/opcode.h>
#include <rocketvm/common/isa/register.h>

#include <stddef.h>
#include <stdint.h>

typedef enum {
	RSM_TOKEN_INVALID,
	RSM_TOKEN_OPCODE,
	RSM_TOKEN_REG,
	RSM_TOKEN_IDENT,
	RSM_TOKEN_NUMBER,
	RSM_TOKEN_COMMA,
	RSM_TOKEN_COLON,
	RSM_TOKEN_NEWLINE,
	RSM_TOKEN_EOF,
} rsm_token_type_t;

typedef struct {
	rsm_token_type_t type;
	union {
		rvm_opcode_t opcode;
		rvm_reg_t reg;
		const char *ident;
		int32_t number;
	} value;

	size_t size;
} rsm_token_t;

typedef struct {
	char *src;
	size_t size;
	size_t pos;
} rsm_lexer_t;

rsm_lexer_t *rsm_lexer_create(char *src, size_t size);
void rsm_lexer_destroy(rsm_lexer_t *lexer);
rsm_token_t rsm_lexer_tokenize(rsm_lexer_t *lexer);

void rsm_lexer_dump_token(const rsm_token_t *token);
const char *rsm_token_type_to_string(rsm_token_type_t type);

#endif
