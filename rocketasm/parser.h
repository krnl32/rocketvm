#ifndef RSM_PARSER_H
#define RSM_PARSER_H

#include "rocketasm/lexer.h"
#include "rocketasm/program.h"

typedef struct {
	rsm_token_t token;
} rsm_parser_t;

rsm_parser_t *rsm_parser_create(void);
void rsm_parser_destroy(rsm_parser_t *parser);
int rsm_parser_parse(rsm_parser_t *parser, rsm_lexer_t *lexer, rsm_program_t *program);

#endif
