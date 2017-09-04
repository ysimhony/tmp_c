/*
 * common.c
 *
 *  Created on: 31 ????? 2017
 *      Author: yacov
 */


#include "common.h"


static void print_word(int word);


/* This function return TRUE if the string is LABEL. Otherwise, FALSE */
bool label_check(char* pch){
   bool result = TRUE;

   size_t len = strlen(pch);

   if (pch[len-1] != ':')
   {
      result = FALSE;
   }

   return result;

   /* TODO - add all the condition for valid label */

}

void store_label(char *label, label_type type){
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

int find_label(char *label, label_type type){
   int table_itr = 0;
   char *tmp_ch = 0;

   while (labels_map.labels_table[table_itr].label != NULL)
   {
	  if (labels_map.labels_table[table_itr].type == type) {
	      if (0 == strncmp(labels_map.labels_table[table_itr].label, label, strlen(label))){
	    	  return labels_map.labels_table[table_itr].value;
	      }
	  }

      table_itr++;
   }

   ERROR("Label was not found", tmp_ch);
}

bool is_instruction(char *pch){
   bool result = FALSE;

   if (pch[0] == '.') {
      result = TRUE;
   }

   return result;
}

instruction_type get_instruction_type(char *pch){
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


void print_label_map() {

	int table_itr = 0;

	struct LABEL *label_cell;
	printf("*************************\n");
	printf("Label Map:\n");
	printf("*************************\n");
	printf("%-10s%-10s%-s\n", "Label", "Value", "Type");
	while (labels_map.labels_table[table_itr].label != NULL)
	{

	   label_cell = &labels_map.labels_table[table_itr];

	   printf("%-10s %-10d %d\n", label_cell->label, label_cell->value, label_cell->type);
	   table_itr++;
	}

}


static void print_word(int word) {
	int bit_index;
	char tmp_str[30];
	int str_itr;

	memset(&tmp_str, 0, sizeof(tmp_str));


	for (bit_index=14, str_itr=0;bit_index>=0;bit_index--) {

		if (bit_index == 11 ||
			bit_index == 9 ||
			bit_index == 5 ||
			bit_index == 3 ||
			bit_index == 1) {
			tmp_str[str_itr++] = '-';
		}
		if ((1<<bit_index) & word) {
			tmp_str[str_itr++] = '1';
		}
		else {
			tmp_str[str_itr++] = '0';
		}


	}

	printf("%s\n", tmp_str);
}

void print_code_arr() {


	/* Array holding the code sec */
	int ic;

	for (ic=0;ic<IC;ic++) {
		printf("%-3d: ", ic);
		print_word(code_arr[ic]);
	}

}

