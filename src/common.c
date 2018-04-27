/*
 * common.c
 *
 *  Created on: 31 ????? 2017
 *      Author: yacov
 */


#include "common.h"

int code_arr[NUM_OF_BYTES] = {0};
int IC = 0;
int L = 0;

/* Array holding the data */
int data_arr[NUM_OF_BYTES] = {0};
int DC = 0;


LABELS_MAP data_code_labels;
LABELS_MAP external_labels;
LABELS_MAP entry_labels;

static const char *special_base = "!@#$%^&*";




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

/*
 * get_symbol_value
 * set_symbol_value
 * store_symbol(symbol, value, attribute)
 *
 */


//int find_label(char *label, label_type type){
//   int table_itr = 0;
//   char *tmp_ch = 0;
//
//   while (data_code_labels_map.labels_table[table_itr].label != NULL)
//   {
//	  if (data_code_labels_map.labels_table[table_itr].type == type) {
//	      if (0 == strncmp(data_code_labels_map.labels_table[table_itr].label, label, strlen(label))){
//	    	  return data_code_labels_map.labels_table[table_itr].value;
//	      }
//	  }
//
//      table_itr++;
//   }
//
//   ERROR("Label was not found", tmp_ch);
//}

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



void print_word(int word, char *str) {
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
	sprintf(str, "%s\n", tmp_str);
}


void print_word_special_base(uint word, char *str) {
	int bit_index;
	int str_itr;

	//memset(&str, 0, sizeof(str));

	for (bit_index=12, str_itr=0;bit_index>=0;bit_index-=3) {

		str[str_itr++] = special_base[(word>>bit_index) & 0x7];

	}

	str[str_itr] = '\0';

	//printf("%s\n", str);
}

void print_code_arr() {


	/* Array holding the code sec */
	int ic;
	char str[80];

	for (ic=0;ic<IC;ic++) {
		printf("%-3d: ", CODE_ARRAY_OFFSET+ic);
		print_word(code_arr[ic], &str[0]);
	}

}


void check_result(bool decimal_format) {

	FILE *res_fp, *exp_fp;
	char * res_line = NULL, *exp_line = NULL;
	size_t res_len = 0, exp_len = 0;
	ssize_t res_read, exp_read;
	char *res_pch, *exp_pch;
	int line_idx, bit_idx;

	res_fp = fopen(
			"C:\\Users\\yacov\\Documents\\GitHub\\tmp_c\\output\\ps.ob",
			"r");

	if (NULL == res_fp) {
		printf("  err %d \n", errno);
		return;
	}

	exp_fp = fopen(
			"C:\\Users\\yacov\\Documents\\GitHub\\tmp_c\\files\\ps.ob.ref",
			"r");

	if (NULL == exp_fp) {
		printf("  err %d \n", errno);
		return;
	}

	line_idx = 0;

//	exp_line = (char *)malloc(exp_len * sizeof(char));
//    if( exp_line == NULL)
//    {
//        perror("Unable to allocate buffer");
//        exit(1);
//    }
//
//    res_line = (char *)malloc(res_len * sizeof(char));
//    if( res_line == NULL)
//    {
//        perror("Unable to allocate buffer");
//        exit(1);
//    }

	while ((res_read = getline(&res_line, &res_len, res_fp)) != -1) {

		if ((exp_read = getline(&exp_line, &exp_len, exp_fp)) == -1) {
			/* TODO - ERROR("Invalid command", pch); */
			printf("check_result error: too many lines in result file\n");
			printf("check_result error: line_idx %u\n", line_idx);
			exit(1);
		}

		/* Remove the trailing CR/LF */
		res_line[strcspn(res_line, "\r\n")] = 0;

		/* Remove the trailing CR/LF */
		exp_line[strcspn(exp_line, "\r\n")] = 0;

		res_pch = res_line;
		exp_pch = exp_line;

		bit_idx = 0;
		while (*res_pch) {
			if (FALSE == decimal_format || *res_pch == '1' || *res_pch == '0') {
				if (*res_pch != *exp_pch) {
					printf("check_result error: mismatch in bits\n");
					printf("check_result error: exp_line %s\n", exp_line);
					printf("check_result error: res_line %s\n", res_line);
					printf("check_result error: mismatch in bit %u\n", bit_idx);
					printf("check_result error: res_pch %c, exp_pch %c\n", *res_pch, *exp_pch);
					printf("check_result error: line_idx %u\n", line_idx);
					exit(1);
				}

				exp_pch++;
				bit_idx++;
			}

			res_pch++;
		}

		if (*exp_pch != '\0') {
			printf("check_result error: few char in result line %u\n", line_idx);
			exit(1);
		}

		line_idx++;
	}

}
