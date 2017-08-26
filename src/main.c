#define _GNU_SOURCE

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/types.h"
#include "ctype.h"

#define NUM_OF_BYTES  (100)
typedef enum { FALSE, TRUE } bool;
typedef enum {LABEL_NONE, LABEL_DATA, LABEL_CODE} label_type;
typedef enum {INST_NONE, INST_DATA, INST_STRING, INST_ENTRY, INST_EXTERN} instruction_type;

#define DEBUG 1

#define ERROR(msg, cmd_ptr) \
if (1) { \
	printf("\n"); \
	printf("%s\n", msg); \
	printf("Assembly line: %s\n", cmd_ptr); \
	printf("At file %s: %d", __FILE__, __LINE__); \
	exit(EXIT_FAILURE); \
};

#define PRINT_DEBUG(...) \
if (DEBUG==1) {\
	printf(__VA_ARGS__); \
	printf("\n"); \
};

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


/* Array holding the code sec */
int code_arr[NUM_OF_BYTES];
int IC = 0;

/* Array holding the data */
int data_arr[NUM_OF_BYTES];
int DC = 0;
    


struct LABEL
{
   char *label;
   int value; 
   label_type type;   
} labels_table[NUM_OF_BYTES];

struct LABELS_MAP
{
   struct LABEL labels_table[NUM_OF_BYTES];
   int itr;   
} labels_map;


bool is_instruction(char *pch)
{
   bool result = FALSE;
   
   if (pch[0] == '.') {
      result = TRUE;
   }
   
   return result;   
}


instruction_type get_instruction_type(char *pch)
{
   instruction_type result = INST_NONE;
   
   if (0 == strcmp(&pch[1], "data"))
   {
      result = INST_DATA;
   }
   else if (0 == strcmp(&pch[1], "string"))
   {
      result = INST_STRING;
   }
   else if (0 == strcmp(&pch[1], "entry"))
   {
      result = INST_ENTRY;
   }
   else if (0 == strcmp(&pch[1], "extern"))
   {
      result = INST_EXTERN;
   }

      
   return result;
   
}

void store_label(char *label, label_type type) 
{
   int table_itr = 0;
   
   while (labels_map.labels_table[table_itr].label != NULL) 
   {
      if (0 == strncmp(labels_map.labels_table[table_itr].label, label, strlen(label)))
      {        
         printf("Label(%s) already exist\n", label);
         exit(EXIT_FAILURE);
      }   

      table_itr++;      
   }
   
   

   labels_map.labels_table[table_itr].label = malloc(strlen(label) * sizeof(char));
   memcpy(labels_map.labels_table[table_itr].label, label, strlen(label) * sizeof(char));
   
   if (type == LABEL_DATA) {
	   labels_map.labels_table[table_itr].value = DC;
   }
   else if (type == LABEL_CODE) {
	   labels_map.labels_table[table_itr].value = IC;
   }
   labels_map.labels_table[table_itr].type = type;

   labels_map.itr = table_itr;
   
}


void perform_7(char *pch, instruction_type i_type)
{
   char *curr_word;
   int str_itr;
   
   if (INST_DATA == i_type) {
      curr_word = strtok(pch, " ");

//      while (curr_word != NULL) {
//         if (curr_word[0] == '+' || curr_word[0] == '-') {
//            /* TODO - how can check that this is a valid number? */
//            /* Does atoi() function also recognize the +/- signs? */
//            data_arr[DC++] = atoi(&curr_word[1]);
//         }
//         else {
//            data_arr[DC++] = atoi(&curr_word[0]);
//         }
//
//
//         curr_word = strtok(pch, " ");
//      }
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

      if (pch[str_itr] == '\0')
      {
    	 ERROR("Invalid command, missing parentheses at the end of string...n", pch); /* Maybe add the line of the failure in the .as file */
      }      
      
      str_itr++;
      
      /* Check trailing characters are only blanking spaces */
      while (pch[str_itr] != '\0')
      {
         if (pch[str_itr] != ' ') {
            ERROR("Invalid command, non-space character after string command", pch);
         }      
      }      
   }
   else 
   {
      /* TODO - return error... */
   }   
}

bool is_register_name(char *pch)
{
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
bool is_dynamic_addressing(char *pch, int *start_bit, int *end_bit)
{
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
   int group;


   /* The increment of the IC */
   /* In first iteration we only check for the validity of the operands
    * and their addressing type */
   int L = 0;

   group = 2;

   p1 = pch;

   if (*p1 == '#') {
	   /* This branch for instant addressing */
	   src_addr = 0;

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
	   }
	   else {
		   /* dynamic addressing */
		   src_addr = 2;

		   p2++;

		   p1 = p2;

		   while (*p2 != '\0') {
			   if (*p2 == '-') {
				   break;
			   }
			   else {
				   if (!(*p2 >= '0' && *p2 <= '9')) {
					   /* issue error */
					   print_label_map();

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

		   *p2 = ',';

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
   
   /* Up until now p2 should point to the comma between the operands */


   p2++;
   p1 = p2;

   if (*p1 == '#') {
	   /* This branch for instant addressing */
	   dest_addr = 0;

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
	   src_addr = 3;

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
		   src_addr = 1;
	   }
	   else {
		   /* dynamic addressing */
		   src_addr = 2;

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

   IC += L;

   PRINT_DEBUG ("updating IC to be %d where L is %d", IC, L);
}

void parse_one_operand_instruction(char *pch, int opcode)
{
}

void parse_non_operand_instruction(char *pch, int opcode)
{
}

void parse_action_instruction(char *pch) 
{
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

/* This function return TRUE if the string is LABEL. Otherwise, FALSE */
bool label_check(char* pch) 
{
   bool result = TRUE;
   
   size_t len = strlen(pch);
   
   if (pch[len-1] != ':') 
   {
      result = FALSE;      
   }
   
   return result;
   
   /* TODO - add all the condition for valid label */

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

void print_code_arr() {


	/* Array holding the code sec */
	int code_arr[NUM_OF_BYTES];
	int IC = 0;


}

void print_data_arr() {

/* Array holding the data */
/*int data_arr[NUM_OF_BYTES];
int DC = 0;
*/
}

void print_label_map() {

	int table_itr = 0;

	struct LABEL *label_cell;
   while (labels_map.labels_table[table_itr].label != NULL)
   {

	   label_cell = &labels_map.labels_table[table_itr];

	   printf("%s %d %d\n", label_cell->label, label_cell->value, label_cell->type);
	   table_itr++;
   }

}

int main() {
   

   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;
   char * pch;
   char * first_word;
    
   bool is_label;
   instruction_type i_type;
    
   printf("hellow world!!!\n");


   memset(&labels_map, 0, sizeof(struct LABELS_MAP));
   

    
   fp = fopen("../files/ps.as", "r");
   if (fp == NULL)
      exit(EXIT_FAILURE);

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

      PRINT_DEBUG("parsing line: %s\n", line);
      is_label = FALSE;

      /* Get first word */
      pch = strtok(line, " ");
      first_word = pch;

      /* Mark label flag */
      if (TRUE == label_check(pch))
      {
         is_label = TRUE;
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
            if (is_label == TRUE)
            {

               store_label(first_word, LABEL_DATA);
            }

            /* TODO - need to split into two cases of DATA and STRING labels */
            perform_7(strtok (NULL, " "), i_type);

         }
         else if (i_type == INST_ENTRY) {

         }
         else if (i_type == INST_EXTERN) {
            /* perform_9();            */
         }
         else {
            /* TODO - need to add error here... */
         }
      }
      else {/* action instruction */

         if (is_label == TRUE) {

            store_label(first_word, LABEL_CODE);
         }

         parse_action_instruction(pch);
      }

      PRINT_DEBUG("finished parsing line\n");
   }
   
   //free(line);
   
   print_label_map();

   fclose(fp);
   if (line)
      free(line);
   exit(EXIT_SUCCESS);
   
   

}
