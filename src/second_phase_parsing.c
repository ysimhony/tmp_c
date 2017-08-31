/*
 * second_phase_parsing.c
 *
 *  Created on: 30 ????? 2017
 *      Author: yacov
 */

#include "second_phase_parsing.h"

int second_phase_parsing() {

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char * pch;
	char * first_word;

	bool is_label;
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

	/* Read line */
	while ((read = getline(&line, &len, fp)) != -1) {
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

	//free(line);

	print_label_map();

	fclose(fp);
	if (line)
	  free(line);
	exit(EXIT_SUCCESS);



}
