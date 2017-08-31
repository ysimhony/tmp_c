/*
 * common.h
 *
 *  Created on: 31 ????? 2017
 *      Author: yacov
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#define _GNU_SOURCE

#include "errno.h"
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
	if (cmd_ptr != NULL) { \
	printf("Assembly line: %s\n", cmd_ptr); \
	} \
	printf("At file %s: %d", __FILE__, __LINE__); \
	exit(EXIT_FAILURE); \
};

#define PRINT_DEBUG(...) \
if (DEBUG==1) {\
	printf(__VA_ARGS__); \
	printf("\n"); \
};

//opcode set/get
#define REGISTER_SET(val, start, width) ((val & ((1<<width)-1))<<start)


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



#endif /* INC_COMMON_H_ */
