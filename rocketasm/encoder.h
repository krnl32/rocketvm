#ifndef RSM_ENCODER_H
#define RSM_ENCODER_H

#include "rocketasm/program.h"

int rsm_resolve_labels(const rsm_program_t* program);
int rsm_resolve_label(const rsm_program_t* program, const rsm_operand_t* opr, size_t instr_idx);
int rsm_encode_instr(const rsm_program_t* program, const rsm_instr_t* instr, size_t instr_idx);

#endif
