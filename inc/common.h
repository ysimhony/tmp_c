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
#include "label_map.h"

#define NUM_OF_BYTES  (100)
#define CODE_ARRAY_OFFSET  (100)

typedef enum { FALSE, TRUE } bool;
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

//ERA
#define  ERA_FIELD_OFFSET  (0)
#define  ERA_FIELD_WIDTH   (2)

//destination addressing
#define  DST_ADDR_FIELD_OFFSET  (2)
#define  DST_ADDR_FIELD_WIDTH   (2)

//source addressing
#define  SRC_ADDR_FIELD_OFFSET  (4)
#define  SRC_ADDR_FIELD_WIDTH   (2)

//opcode
#define  OPCODE_FIELD_OFFSET  (6)
#define  OPCODE_FIELD_WIDTH   (4)

//group
#define  GROUP_FIELD_OFFSET  (10)
#define  GROUP_FIELD_WIDTH   (2)

//reserved
#define  RSRVD_FIELD_OFFSET  (12)
#define  RSRVD_FIELD_WIDTH   (3)

//opcode set/get
#define REGISTER_SET(val, start, width) ((val & ((1<<width)-1))<<start)
#define REGISTER_GET(reg, start, width) ((reg>> start) & ((1<<width)-1))






/* Array holding the code sec */
extern int code_arr[NUM_OF_BYTES];
extern int IC;
extern int L;

/* Array holding the data */
extern int data_arr[NUM_OF_BYTES];
extern int DC;

extern LABELS_MAP data_code_labels;
extern LABELS_MAP external_labels;
extern LABELS_MAP entry_labels;


bool label_check(char* pch);
bool is_instruction(char *pch);
bool is_external_symbol(char *symbol);
void store_label(char *label, label_type type);
int find_label(char *label, label_type type);
instruction_type get_instruction_type(char *pch);
void print_code_arr();

void print_label_map();



#endif /* INC_COMMON_H_ */
