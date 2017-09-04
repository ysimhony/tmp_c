/*
 * first_phase_parsing.h
 *
 *  Created on: 29 ????? 2017
 *      Author: yacov
 */

#ifndef INC_FIRST_PHASE_PARSING_H_
#define INC_FIRST_PHASE_PARSING_H_



#include "common.h"


bool is_instruction(char *pch);
void store_label(char *label, label_type type);
void perform_7(char *pch, instruction_type i_type);
bool is_register_name(char *pch);
bool is_dynamic_addressing(char *pch, int *start_bit, int *end_bit);
void first_phase_parsing();

#endif /* INC_FIRST_PHASE_PARSING_H_ */
