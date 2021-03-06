/*
 * first_phase_parsing.c
 *
 *  Created on: 29 ????? 2017
 *      Author: yacov
 */




#include "first_phase_parsing.h"


const char *opcodes[] =
{
   "mov", /* 0 */
"cmp", /* 1 */
"add", /* 2 */
"sub", /* 3 */
"not", /* 4 */
"clr", /* 5 */
"lea", /* 6 */
"inc", /* 7 */
"dec", /* 8 */
"jmp", /* 9 */
"bne", /* 10 */
"red", /* 11 */
"prn", /* 12 */
"jsr", /* 13 */
"rts", /* 14 */
"stop", /* 15 */
};




static void parse_two_operands_instruction(char *pch, int opcode);
static void parse_one_operand_instruction(char *pch, int opcode);
static void parse_non_operand_instruction(char *pch, int opcode);



void parse_instruction(char *pch, instruction_type i_type) {
   //char *curr_word;
   int str_itr;

   if (INST_DATA == i_type) {

	   char *p1;
	   char *p2;
	   p1 = p2 = pch;

      while (*p2 != '\0') {
    	  if (*p2 == ' ' || *p2 == ',') {

    		  char tmp_c;

    		  tmp_c = *p2;

    		  *p2 = '\0';

    		  data_arr[DC++] = atoi(p1);

    		  *p2 = tmp_c;

    		  while (*p2 == ' ') {
    			  p2++;
    		  }

    		  if (*p2 != ',') {
    			  ERROR("Invalid data sentence!!!", pch);
    		  }

    		  p2++;

    		  while (*p2 == ' ') {
    			  p2++;
    		  }

    		  p1 = p2;

    	  }
    	  else {

    		  if (*p2 == '+' || *p2 == '-') {
    			  if (p1 != p2) {
    				  ERROR("Invalid data sentence!!! Misplace sign charachter", pch);
    			  }

    		  }
    		  else if (!(*p2 >= '0' && *p2 <= '9')) {
    			  ERROR("Invalid data sentence!!!", pch);
    		  }

    		  p2++;
    	  }
      }

      data_arr[DC++] = atoi(p1);
   }
   else if (INST_STRING == i_type){

	  if (pch[0] != '"') {
         ERROR("Invalid command", pch);
      }

      str_itr = 1;

      while (pch[str_itr] != '\0') {
         if (pch[str_itr] != '"') {
            data_arr[DC++] = pch[str_itr++];
         }
         else {
            break;
         }
      }

      if (pch[str_itr] == '\0') {
    	 ERROR("Invalid command, missing parentheses at the end of string...n", pch); /* Maybe add the line of the failure in the .as file */
      }

      data_arr[DC++] = 0;

   }
   else
   {
      /* TODO - return error... */
   }
}

bool is_register_name(char *pch) {
   bool result = FALSE;

   if (pch[0] != 'r')
   {
      result = FALSE;
   }
   else if (strlen(pch) > 2)
   {
      result = FALSE;
   }
   else if ((pch[1] - '0') > 7 ||
            (pch[1] - '0') < 0)
   {
      result = FALSE;
   }

   return result;
}

/*
 * This function returns TRUE if the string starting at pch is in dynamic addressing format. Otherwise, FALSE.
 * start bit - is the start bit to extract the value from the first operand
 * end_bit - is the end bit to extract the value from the first operand
 */
bool is_dynamic_addressing(char *pch, int *start_bit, int *end_bit){
   /*bool result = TRUE;*/
   char *p1, *p2;

   p1 = pch;

   /* Find the open bracket */
   while (*p1++ != '[') {}

   if (*p1 == '\0') {
	   return FALSE;
   }

   p1++;

   p2 = p1;

   /* Find the next comma */
   while (*p2++ != ',') {}

   if (*p2 == '\0') {
	   return FALSE;
   }

   *p2 = '\0';

   /* Extract the start bit of the indirect addressing */
   *start_bit = atoi(p1);

   *p2 = ',';

   p2++;

   p1 = p2;

   /* Find the closing bracket */
   while (*p2++ != ']') {}

   if (*p2 == '\0') {
	   return FALSE;
   }

   *p2 = '\0';

   /* Extract the end bit of the indirect addressing */
   *end_bit = atoi(p1);

   *p2 = ']';

   p2++;

   if (*p2 != ',') {
	   return FALSE;
   }

   return TRUE;
}


int valid_operand_addressing[OPERAND_TYPE_END][1<<OPCODE_FIELD_WIDTH] = {
		{//source operand
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* mov */
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* cmp */
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* add */
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* sub */
				0, /* not */
				0, /* clr */
				SET_BIT(DIRECT), /* lea*/

		},
		{ //dest operand
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* mov */
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* cmp */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* add */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* sub */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* not */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* clr */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* lea*/
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* inc */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* dec */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* jmp */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* bne */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* red */
				SET_BIT(IMMEDIATE) | SET_BIT(DIRECT) | SET_BIT(DYNAMIC) | SET_BIT(DIRECT_REG), /* prn */
				SET_BIT(DIRECT) | SET_BIT(DIRECT_REG), /* jsr */

		}
};

void validate_command_addressing(int opcode, operand_type_t operand_type, int addr) {

	if (!(SET_BIT(addr) & (valid_operand_addressing[operand_type][opcode]))) {
		printf("\n"); \
		printf("Invalid addressing\n"); \
		printf("At file %s: %d", __FILE__, __LINE__); \
		exit(EXIT_FAILURE); \

	}
}

/*
 * pch - points to the start of the two operands
 * opcode - is the opcode value of the action sentence
 */
void parse_two_operands_instruction(char *pch, int opcode) {
   char *p1, *p2;

   /* command fields */
   /* TODO - we might use macros to set and get the fields */
   /*int era; TODO - need to implement the era logic
    basically, era is relevant only for addresses
   if the address is declared locally, era = '10'
   if the address is declared externally, era = '01'
   otherwisie, era = '00' */
   int dest_addr;
   int src_addr;
   int group = 2;


   /* The increment of the IC */
   /* In first iteration we only check for the validity of the operands
    * and their addressing type */

   p1 = pch;

   if (*p1 == '#') {
	   /* This branch for instant addressing */
	   src_addr = 0;
	   validate_command_addressing(opcode, SOURCE, src_addr);
	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {
		   if (*p2 == ',') {
			   break;
		   }

		   p2++;
	   }

	   if (p2 == p1) {
		   ERROR("Invalid source operand", pch);
	   }

	   if (*p2 != ',') {
		   ERROR("Invalid source operand", pch);
	   }

	   if (*p2 != ',') {
		   ERROR("Invalid source operand", pch);
	   }

	   /* if atoi fail - issue error */
   }
   else if (*p1 == 'r') {
	   /* direct register addressing */
	   src_addr = 3;
	   validate_command_addressing(opcode, SOURCE, src_addr);

	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {
		   if (*p2 == ',') {
			   break;
		   }
		   else {
			   if (!(*p2 >= '0' && *p2 <= '9')) {
				   ERROR("Invalid source operand", pch);
			   }
		   }

		   p2++;
	   }

	   if (p2 == p1) {
		   /* issue error - need to exclude the case where the label is r */
		   ERROR("Invalid source operand", pch);
	   }

	   if (*p2 != ',') {
		   /* issue error */
		   ERROR("Invalid source operand", pch);
	   }
   }
   else { /* either direct addressing or dynamic addressing */

	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {
		   if ((*p2 == '[' || *p2 == ',')) {
			   break;
		   }

		   p2++;
	   }

	   if (*p2 == '\0') {
		   /* issue error */
		   ERROR("Invalid source operand", pch);
	   }

	   if (*p2 == ',') {
		   /* direct addressing */
		   src_addr = 1;
		   validate_command_addressing(opcode, SOURCE, src_addr);
	   }
	   else {
		   /* dynamic addressing */
		   src_addr = 2;
		   validate_command_addressing(opcode, SOURCE, src_addr);

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == '-') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   //print_label_map();

					   ERROR("Invalid source operand", p1);
				   }
			   }

			   p2++;
		   }

		   if (*p2 == '\0') {
			   /* issue error */
			   ERROR("Invalid source operand", pch);
		   }

		   *p2 = '\0';

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid source operand", pch);
		   }

		   //*p2 = ',';

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == ']') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   ERROR("Invalid source operand", pch);
				   }
			   }

			   p2++;
		   }

		   if (*p2 == '\0') {
			   /* issue error */
			   ERROR("Invalid source operand", pch);
		   }

		   *p2 = '\0';

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid source operand", pch);
		   }

		   *p2 = ']';

		   p2++;

		   if (*p2 != ',') {
			   /* issue error */
			   ERROR("Invalid source operand", pch);
		   }
	   }
   }

   /* Up until now p2 should point to the comma between the operands or to NULL */

   if (*p2 == '\0') {
	   pch = strtok(NULL, " ");

	   if (pch == NULL) {
		   ERROR("Invalid destination operand", pch);
	   }

	   if (*pch != ',') {
		   ERROR("Invalid destination operand", pch);
	   }

	   p2 = pch;
   }

   /* Up until now p2 should point to the comma between the operands */
   p2++;

   if (*p2 == '\0') {
	   pch = strtok(NULL, " ");

	   if (pch == NULL) {
		   ERROR("Invalid destination operand", pch);
	   }

	   p2 = pch;
   }

   p1 = p2;

   if (*p1 == '#') {
	   /* This branch for instant addressing */
	   dest_addr = 0;
	   validate_command_addressing(opcode, DETSTINATION, dest_addr);

	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {

		   p2++;
	   }

	   if (p2 == p1) {
		   ERROR("Invalid destination operand", pch);
	   }

	   /* if atoi fail - issue error */
   }
   else if (*p1 == 'r') {
	   /* direct register addressing */
	   dest_addr = 3;
	   validate_command_addressing(opcode, DETSTINATION, dest_addr);

	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {
		   if (!(*p2 >= '0' && *p2 <= '9')) {
			   ERROR("Invalid destination operand", pch);
		   }

		   p2++;
	   }

	   if (p2 == p1) {
		   /* issue error - need to exclude the case where the label is r */
		   ERROR("Invalid destination operand", pch);
	   }
   }
   else { /* either direct addressing or dynamic addressing */

	   p1++;
	   p2 = p1;
	   while (*p2 != '\0') {
		   if (*p2 == '[') {
			   break;
		   }

		   p2++;
	   }

	   if (*p2 == '\0') {
		   /* direct addressing */
		   dest_addr = 1;
		   validate_command_addressing(opcode, DETSTINATION, dest_addr);
	   }
	   else {
		   /* dynamic addressing */
		   dest_addr = 2;
		   validate_command_addressing(opcode, DETSTINATION, dest_addr);

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == '-') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   ERROR("Invalid destination operand", p1);
				   }
			   }

			   p2++;
		   }

		   if (*p2 == '\0') {
			   /* issue error */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = '\0';

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = '-';

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == ']') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   ERROR("Invalid destination operand", pch);
				   }
			   }

			   p2++;
		   }

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = ']';

		   p2++;

	   }
   }




   printf("Parsed command line successfully\n");

   if (3 == src_addr && 3 == dest_addr) {
	   L = 2;
   }
   else {
	   L = 3;
   }

   code_arr[IC] =
   REGISTER_SET(/*reserved*/5, 12, 3) |
   REGISTER_SET(group, 10, 2) |
   REGISTER_SET(opcode, 6, 4) |
   REGISTER_SET(src_addr, 4, 2) |
   REGISTER_SET(dest_addr, 2, 2) |
   REGISTER_SET(0, 0, 2);
   IC += L;

   PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);
}

void parse_one_operand_instruction(char *pch, int opcode) {

	char *p1, *p2;

	int dest_addr;
	int group = 1;

	p1 = pch;

	if (*p1 == '#') {
		/* This branch for instant addressing */
		dest_addr = 0;
		validate_command_addressing(opcode, DETSTINATION, dest_addr);

		p1++;
		p2 = p1;
		while (*p2 != '\0') {

		   p2++;
		}

		if (p2 == p1) {
		   ERROR("Invalid destination operand", pch);
		}

		/* if atoi fail - issue error */
	}
	else if (*p1 == 'r') {
		/* direct register addressing */
		dest_addr = 3;
		validate_command_addressing(opcode, DETSTINATION, dest_addr);

		p1++;
		p2 = p1;
		while (*p2 != '\0') {
		   if (!(*p2 >= '0' && *p2 <= '9')) {
			   ERROR("Invalid destination operand", pch);
		   }

		   p2++;
		}

		if (p2 == p1) {
		   /* issue error - need to exclude the case where the label is r */
		   ERROR("Invalid destination operand", pch);
		}
	}
	else { /* either direct addressing or dynamic addressing */

		p1++;
		p2 = p1;
		while (*p2 != '\0') {
		   if (*p2 == '[') {
			   break;
		   }

		   p2++;
		}

		if (*p2 == '\0') {
		   /* direct addressing */
		   dest_addr = 1;
		   validate_command_addressing(opcode, DETSTINATION, dest_addr);
		}
		else {
		   /* dynamic addressing */
		   dest_addr = 2;
		   validate_command_addressing(opcode, DETSTINATION, dest_addr);

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == '-') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   ERROR("Invalid destination operand", p1);
				   }
			   }

			   p2++;
		   }

		   if (*p2 == '\0') {
			   /* issue error */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = '\0';

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = '-';

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == ']') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   ERROR("Invalid destination operand", pch);
				   }
			   }

			   p2++;
		   }

		   if (atoi(p1) > 14) {
			   /* issue error - the valid range is 0-14 */
			   ERROR("Invalid destination operand", pch);
		   }

		   *p2 = ']';

		   p2++;
		}
	}


	code_arr[IC] =
		REGISTER_SET(/*reserved*/5, 12, 3) |
		REGISTER_SET(group, 10, 2) |
		REGISTER_SET(opcode, 6, 4) |
		REGISTER_SET(0, 4, 2) |
		REGISTER_SET(dest_addr, 2, 2) |
		REGISTER_SET(0, 0, 2);

	// It's a one operand instruction, we need only two words for the instruction
	L = 2;


	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);
}

void parse_non_operand_instruction(char *pch, int opcode) {

	int group = 0;

	if (pch != NULL) {
	   ERROR("Invalid non operand instruction", pch);
	}

	code_arr[IC] =
		REGISTER_SET(/*reserved*/5, 12, 3) |
		REGISTER_SET(group, 10, 2) |
		REGISTER_SET(opcode, 6, 4) |
		REGISTER_SET(0, 4, 2) |
		REGISTER_SET(0, 2, 2) |
		REGISTER_SET(0, 0, 2);

	L = 1;

	IC += L;

	PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);

}

static void parse_action_instruction(char *pch){
   int itr;
   bool found;

   itr = 0;
   found = FALSE;

   while (itr <  16) {
      if ((strlen(opcodes[itr]) == strlen(pch)) &&
          (0 == strncmp(opcodes[itr], pch, strlen(opcodes[itr]))))
      {
         found = TRUE;
         break;
      }

      itr++;
   }

   if (FALSE == found)
   {
      ERROR("Invalid instruction", pch);
   }

   pch = strtok(NULL, " ");

   switch (itr){
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


bool data_or_string_check(char* pch)
{
   bool result = FALSE;


   if (pch[0] == '.')
   {
      if (0 == strncmp(&pch[1], "data", strlen("data")))
      {
         result = TRUE;
      }
      else if (0 == strncmp(&pch[1], "string", strlen("string")))
      {
         result = TRUE;
      }
   }

   return result;
}


void print_data_arr() {

/* Array holding the data */
/*int data_arr[NUM_OF_BYTES];
int DC = 0;
*/
}


void first_phase_parsing() {


   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;
   char * pch;
   char * first_word;

   bool is_label;
   instruction_type i_type;

   memset(&data_code_labels, 0, sizeof(LABELS_MAP));
   memset(&external_labels, 0, sizeof(LABELS_MAP));
   memset(&entry_labels, 0, sizeof(LABELS_MAP));

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

   printf("Starting first phase parsing...\n\n");

   IC = 0;
   DC = 0;
   /* Read line */
   while ((read = getline(&line, &len, fp)) != -1) {
/*        printf("Retrieved line of length %zu :\n", read); */
      /* printf("%s", line);*/

      /* printf ("Splitting string \"%s\" into tokens:\n", line); */

      /* Remove the traing CR/LF */
      line[strcspn(line, "\r\n")] = 0;

      /* Skip comment line or empty line */
      if ((line[0] == ';') || (line[0] == 0))
         continue;

      /* TODO - need to skip empty lines */

      PRINT_DEBUG("parsing line: %s", line);
      is_label = FALSE;

      /* Get first word */
      pch = strtok(line, " ");
      first_word = pch;

      /* Mark label flag */
      if (TRUE == label_check(pch)) {
         is_label = TRUE;
         first_word[strlen(first_word)-1] = '\0';
      }

      if (is_label) {
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
            if (is_label == TRUE) {

               store_symbol(&data_code_labels, first_word, DC, LABEL_DATA);
            }

            /* TODO - need to split into two cases of DATA and STRING labels */
            parse_instruction(strtok (NULL, " "), i_type);
         }
         else if (i_type == INST_ENTRY) {

         }
         else if (i_type == INST_EXTERN) {
            /* perform_9();            */

        	pch = strtok (NULL, " ");
        	store_symbol(&external_labels, pch, IC, LABEL_NONE);
         }
         else {
            /* TODO - need to add error here... */
         }
      }
      else {/* action instruction */

         if (is_label == TRUE) {

            store_symbol(&data_code_labels, first_word, IC, LABEL_CODE);
         }

         parse_action_instruction(pch);
      }

      PRINT_DEBUG("finished parsing line\n");
   }

   //free(line);

   fclose(fp);
   if (line)
      free(line);

   printf("Finished first phase parsing!!!\n\n");

   //exit(EXIT_SUCCESS);



}
