/*
 * second_phase_parsing.c
 *
 *  Created on: 30 ????? 2017
 *      Author: yacov
 */

#include "second_phase_parsing.h"

static int size_of_code_arr = 0;
static void parse_two_operands_instruction(char *pch, int opcode);
static void parse_one_operand_instruction(char *pch, int opcode);
static void parse_non_operand_instruction(char *pch, int opcode);



static char *parse_operand(char *pch, operand_type_t operand_type, bool two_operands) {
	char *p1;
	char *p2;
	int operand_val;
	addressing_type_t addressing_type;
	int operand_offset = 0;
	bool two_operands_registers = FALSE;

	p1 = p2 = pch;


	if (SOURCE == operand_type) {
		addressing_type = REGISTER_GET(code_arr[IC], SRC_ADDR_FIELD_OFFSET, SRC_ADDR_FIELD_WIDTH);

		operand_offset = 1;

//		if (p2 == NULL) {
//			fflush(stdout);
//		}
		// find the comma which the delimeter between source and destination operands
//		while (*p2 != ',') {
//			p2++;
//		}
//
//		*p2 = '\0';
	}
	else {
		addressing_type = REGISTER_GET(code_arr[IC], DST_ADDR_FIELD_OFFSET, DST_ADDR_FIELD_WIDTH);

		if (TRUE == two_operands) {
			operand_offset = 2;

			if (DIRECT_REG == addressing_type) {
				if (DIRECT_REG == REGISTER_GET(code_arr[IC], SRC_ADDR_FIELD_OFFSET, SRC_ADDR_FIELD_WIDTH)) {
					operand_offset = 1;
					two_operands_registers = TRUE;

				}
			}
		}
		else {
			operand_offset = 1;
		}

	}

	switch (addressing_type) {
		case IMMEDIATE: {
			p1++;
			operand_val = atoi(p1);
			operand_val <<= ERA_FIELD_WIDTH;
			operand_val = operand_val | REGISTER_SET(0, ERA_FIELD_OFFSET, ERA_FIELD_WIDTH);
		}
		break;

		case DIRECT: {
			operand_val = get_symbol_value(&external_labels, p1);

			if (-1 != operand_val) {
				// We found that the symbol exist in the external symbol map
				operand_val = 1;

				// update the external label map to store the reference to the external symbol
				// in the current assembly file
				set_symbol_value(&external_labels, p1, CODE_ARRAY_OFFSET+IC+1);
			}
			else {
				operand_val = get_symbol_value(&data_code_labels, p1);
				operand_val += CODE_ARRAY_OFFSET + size_of_code_arr;
				operand_val <<= ERA_FIELD_WIDTH;
				operand_val = operand_val | REGISTER_SET(2, ERA_FIELD_OFFSET, ERA_FIELD_WIDTH);

//				// If this symbol exist in the entry label map, then we update
//				if (-1 != get_symbol_value(&entry_labels, p1)) {
//					set_symbol_value(&entry_labels, p1, CODE_ARRAY_OFFSET+IC);
//				}
			}
		}
		break;
		case DYNAMIC: {
			char *p3;

			p3 = p1;
			while (*p3 != '[') {
				p3++;
			}

			*p3 = '\0';

			operand_val = get_symbol_value(&external_labels, p1);

			if (-1 != operand_val) {
				// We found that the symbol exist in the external symbol map
				operand_val = 1;

				// update the external label map to store the reference to the external symbol
				// in the current assembly file
				set_symbol_value(&external_labels, p1, CODE_ARRAY_OFFSET+IC+1);
			}
			else {
				operand_val = get_symbol_value(&data_code_labels, p1);

				if (-1 != operand_val) {
					// symbol exists in the data_code_array
					label_type attr;
					attr = get_symbol_attr(&data_code_labels, p1);

					int arr_val;
					int start_index;
					int end_index;

					// find out whether it's a data array  or code array
					if (LABEL_DATA == attr) {

						arr_val = data_arr[operand_val];
					}
					else {
						arr_val = code_arr[operand_val];
					}

					// find the start and end indices
					*p3 = '[';

					p3++;
					p1 = p3;


					while (*p3 != '-') {
						p3++;
					}
					*p3 = '\0';

					start_index = atoi(p1);
					*p3 = '-';

					p3++;
					p1 = p3;

					while (*p3 != ']') {
						p3++;
					}
					*p3 = '\0';

					end_index = atoi(p1);
					*p3 = ']';

					int field_width = end_index - start_index + 1;
					int sign_externsion = REGISTER_SET(0xFFFF, (ERA_FIELD_WIDTH+field_width), (15-ERA_FIELD_WIDTH-field_width));

					operand_val = REGISTER_GET(arr_val, start_index, field_width);
					operand_val = operand_val << ERA_FIELD_WIDTH;
					operand_val = operand_val | REGISTER_SET(0, ERA_FIELD_OFFSET, ERA_FIELD_WIDTH);
					operand_val = operand_val | ((operand_val & (1 << (field_width-1)))?sign_externsion:0);

				}
				else {
					// If the symbol does not exist in the data code label map, and not in th external label map
					// then, issue an error
				}

			}
		}
		break;
		case DIRECT_REG: {
			while (!((*p1 >= '0') && (*p1 <= '9'))) {
				p1++;
			}

			operand_val = atoi(p1);

			if (SOURCE == operand_type) {
				operand_val = REGISTER_SET(operand_val, SOURCE_REGISTER_OFFSET, SOURCE_REGISTER_WIDTH);
			}
			else {
				operand_val = REGISTER_SET(operand_val, DEST_REGISTER_OFFSET, DEST_REGISTER_WIDTH);

				if (TRUE == two_operands_registers) {
					// two_operands_registers means the this is the second registers of two
					// operads command, therefore the two registers are encoded in the same word
					operand_val |= code_arr[IC+operand_offset];
				}
			}

		}
		break;
		default:
			break;
	}

	code_arr[IC+operand_offset] = operand_val;

//	L = 2;
//	IC += L;

//	p2++;
//
//	while (*p2 == ' ') {
//		p2++;
//	}


	if (SOURCE == operand_type) {
		pch = strtok(NULL, ",");

		if (pch == NULL || pch == '\0') {
			ERROR("Could not find start of the destination opernads in two operands", pch);
		}
	}
	return pch;
}

static void parse_two_operands_instruction(char *pch, int opcode) {

	int dest_addr;
	int src_addr;

	dest_addr = REGISTER_GET(code_arr[IC], DST_ADDR_FIELD_OFFSET, DST_ADDR_FIELD_WIDTH);
	src_addr = REGISTER_GET(code_arr[IC], SRC_ADDR_FIELD_OFFSET, SRC_ADDR_FIELD_WIDTH);


	if (3 == src_addr && 3 == dest_addr) {
	   L = 2;
	}
	else {
	   L = 3;
	}

	//pch = strtok(pch, ",");
	pch = parse_operand(pch, SOURCE, TRUE);
	parse_operand(pch, DETSTINATION, TRUE);


	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);


}
static void parse_one_operand_instruction(char *pch, int opcode) {

	L = 2;

	parse_operand(pch, DETSTINATION, FALSE);

	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);

}

static void parse_non_operand_instruction(char *pch, int opcode) {

	L = 1;

	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);

}


static void parse_action_instruction(char *pch){
   int itr;

   itr = 0;

   pch = strtok(NULL, ",");

   int opcode = REGISTER_GET(code_arr[IC], 6, 4);
   switch (opcode) {
      case 0: /* mov */
      case 1: /* cmp */
      case 2: /* add */
      case 3: /* sub */
      case 6: /* lea */
         parse_two_operands_instruction(pch, itr);
         break;
      case 4: /* not */
      case 5: /* clr */
      case 7: /* inc */
      case 8: /* dec */
      case 9: /* jmp */
      case 10: /* bne */
      case 11: /* red */
      case 12: /* prn */
      case 13: /* jsr */
         parse_one_operand_instruction(pch, itr);
         break;
      case 14: /* rts */
      case 15: /* stop */
         parse_non_operand_instruction(pch, itr);
         break;
   }


}

void second_phase_parsing() {

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char * pch;

	instruction_type i_type;

	errno = 0;
	#ifdef __CYGWIN__
	fp = fopen("C:\\Users\\yacov\\Documents\\GitHub\\tmp_c\\files\\ps.as", "r");
	#elif __UNIX__
	fp = fopen("../files/ps.as", "r");
	#endif
	if (fp == NULL) {
	   printf ("  err %d \n", errno);
	   exit(EXIT_FAILURE);
	}

	//update_labels_by_attr(&data_code_labels, CODE_ARRAY_OFFSET+IC, LABEL_DATA);

	// Store the size of the code array, in order to know what is the offset for the data array
	size_of_code_arr = IC;

	IC = 0;
	DC = 0;

	/* Read line */
	while ((read = getline(&line, &len, fp)) != -1) {
	  /* Remove the traing CR/LF */
	  line[strcspn(line, "\r\n")] = 0;

	  /* Skip comment line or empty line */
	  if ((line[0] == ';') || (line[0] == 0))
		 continue;

	  /* TODO - need to skip empty lines */
	  PRINT_DEBUG("parsing line: %s", line);

	  /* Get first word */
	  pch = strtok(line, " ");

	  /* Mark label flag */
	  if (TRUE == label_check(pch)) {

		 /* Get second word */
		 pch = strtok (NULL, " ");

		 /* Check validity */
		 if (pch == NULL)
		 {

			/* TODO - what shall we do here? */
		 }
	  }

	  if (is_instruction(pch)) {/* pch holds the string that is suspect to be the instruction type */

		 i_type = get_instruction_type(pch);

		 if (i_type == INST_DATA || i_type == INST_STRING) {
		 }
		 else if (i_type == INST_ENTRY) {
		 }
		 else if (i_type == INST_EXTERN) {
		 }
		 else {
			/* TODO - need to add error here... */
		 }
	  }
	  else {/* action instruction */

		 parse_action_instruction(pch);
	  }

	  PRINT_DEBUG("finished parsing line\n");
	}

	fclose(fp);
	if (line)
	  free(line);

	write_arr_to_file("yacov");
}
