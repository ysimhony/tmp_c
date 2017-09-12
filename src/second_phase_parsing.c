/*
 * second_phase_parsing.c
 *
 *  Created on: 30 ????? 2017
 *      Author: yacov
 */

#include "second_phase_parsing.h"

static void parse_two_operands_instruction(char *pch, int opcode);
static void parse_one_operand_instruction(char *pch, int opcode);
static void parse_non_operand_instruction(char *pch, int opcode);



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

	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);


}

static void parse_one_operand_instruction(char *pch, int opcode) {
	char *p1;
	char *p2;
	int operand_val;

	p1 = p2 = pch;

	switch (REGISTER_GET(code_arr[IC], DST_ADDR_FIELD_OFFSET, DST_ADDR_FIELD_WIDTH)) {
		case 0: {
			p1++;
			operand_val = atoi(p1);
		}
		break;

		case 1: {
			int operand_val = get_symbol_value(&external_labels, p1);

			if (-1 != operand_val) {
				// We found that the symbol exist in the external symbol map
				operand_val = 1;

				// update the external label map to store the reference to the external symbol
				// in the current assembly file
				set_symbol_value(&external_labels, p1, CODE_ARRAY_OFFSET+IC+1);
			}
			else {
				operand_val = get_symbol_value(&data_code_labels, p1);
				operand_val <<= ERA_FIELD_WIDTH;
				operand_val = operand_val | REGISTER_SET(2, ERA_FIELD_OFFSET, ERA_FIELD_WIDTH);

//				// If this symbol exist in the entry label map, then we update
//				if (-1 != get_symbol_value(&entry_labels, p1)) {
//					set_symbol_value(&entry_labels, p1, CODE_ARRAY_OFFSET+IC);
//				}
			}
		}
		break;
		case 2: {
			while (*p2 != '[') {
				p2++;
			}

			*p2 = '\0';

			int operand_val = get_symbol_value(&external_labels, p1);

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
					// symbol exist in the data_code_array
					label_type attr;
					attr = get_symbol_attr(&data_code_labels, p1);

					int arr_val;
					int start_index;
					int end_index;
					if (LABEL_DATA == attr) {

						arr_val = data_arr[operand_val];
					}
					else {
						arr_val = code_arr[operand_val];
					}

					*p2 = '[';

					p2++;
					p1 = p2;


					while (*p2 != '-') {
						p2++;
					}
					*p2 = '\0';

					start_index = atoi(p1);
					*p2 = '-';

					p2++;
					p1 = p2;

					while (*p2 != ']') {
						p2++;
					}
					*p2 = '\0';

					end_index = atoi(p1);
					*p2 = ']';

					operand_val = REGISTER_GET(arr_val, start_index, end_index);
					operand_val <<= ERA_FIELD_WIDTH;
					operand_val = operand_val | REGISTER_SET(0, ERA_FIELD_OFFSET, ERA_FIELD_WIDTH);
				}
				else {
					// If the symbol does not exist in the data code label map, and not in th external label map
					// then, issue an error
				}

			}

		}
		break;
	}

	code_arr[IC+1] = operand_val;
	L = 2;
	IC += L;
}

static void parse_non_operand_instruction(char *pch, int opcode) {

	L = 1;

	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);

}


static void parse_action_instruction(char *pch){
   int itr;

   itr = 0;

   pch = strtok(NULL, " ");

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

	update_labels_by_attr(&data_code_labels, IC, LABEL_DATA);

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
	  PRINT_DEBUG("parsing line: %s\n", line);

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

}