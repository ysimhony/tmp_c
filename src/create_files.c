/*
 * create_files.c
 *
 *  Created on: 27 ????? 2018
 *      Author: yacov
 */

#include "create_files.h"

static void write_ob_file(bool decimal_format);
static void write_ent_file();


static void write_ob_file(bool decimal_format) {
	FILE *fp;
	/* Array holding the code sec */
	int ic, dc;
	char str[80];

	fp = fopen("C:\\Users\\yacov\\Documents\\GitHub\\tmp_c\\output\\ps.ob",
			"w");

	if (NULL == fp) {
		printf("  err %d \n", errno);
		return;
	}

	for (ic = 0; ic < IC; ic++) {

		if (TRUE == decimal_format) {
			printf("%-3d: ", ic);
			print_word(code_arr[ic], &str[0]);
		} else {
			print_word_special_base((CODE_ARRAY_OFFSET + ic), str);
			sprintf(str, "%s :", str);
			str[strlen(str)] = ' ';
			print_word_special_base(code_arr[ic], str + 8);
		}
		fprintf(fp, "%s\n", str);
	}

	/* Array holding the data sec */
	for (dc = 0; dc < DC; dc++) {

		if (TRUE == decimal_format) {
			printf("%-3d: ", dc);
			print_word(data_arr[dc], &str[0]);
		} else {
			print_word_special_base(CODE_ARRAY_OFFSET + IC + dc, str);
			sprintf(str, "%s :", str);
			str[strlen(str)] = ' ';
			print_word_special_base(data_arr[dc], str + 8);
		}
		fprintf(fp, "%s\n", str);
	}

	//fprintf(fp, "Testing...\n");
	fclose(fp);
}

static void write_ent_file() {
	FILE *fp;
	/* Array holding the code sec */
	char *label;
	uint value;
	label_type type;
	char str[80];

	fp = fopen("C:\\Users\\yacov\\Documents\\GitHub\\tmp_c\\output\\ps.ent",
			"w");

	if (NULL == fp) {
		printf("  err %d \n", errno);
		return;
	}

	get_next_label_as_entry_reset(&data_code_labels);

	while ((label = get_next_label_as_entry(&data_code_labels, &value, &type)) != NULL) {

		sprintf(str, "%-10s", label);

		value += CODE_ARRAY_OFFSET;

		if (type == LABEL_DATA) {
			value += IC;
		}

		print_word_special_base(value, str + strlen(str));
		fprintf(fp, "%s\n", str);
	}

	fclose(fp);
}


void create_files() {
	write_ob_file(FALSE);
	write_ent_file();
}
