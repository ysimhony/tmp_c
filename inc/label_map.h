/*
 * label_map.h
 *
 *  Created on: 4 ????? 2017
 *      Author: yacov
 */

#ifndef INC_LABEL_MAP_H_
#define INC_LABEL_MAP_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define LAEBEL_MAP_ERROR(msg) \
if (1) { \
	printf("\n"); \
	printf("%s\n", msg); \
	printf("At file %s: %d", __FILE__, __LINE__); \
	exit(EXIT_FAILURE); \
};

typedef enum {LABEL_NONE, LABEL_DATA, LABEL_CODE} label_type;

typedef struct _LABEL
{
   char *label;
   int value;
   label_type type;
   struct _LABEL *next;
} LABEL;

typedef struct _LABELS_MAP
{
   LABEL *labels_table;
   LABEL *itr;//points to the last label entry
} LABELS_MAP;

int get_symbol_value(LABELS_MAP *label_map, char *symbol);
void set_symbol_value(LABELS_MAP *label_map, char *symbol, int value);
void store_symbol(LABELS_MAP *labels_map, char *symbol, int value, label_type attr);
void update_labels_by_attr(LABELS_MAP *labels_map, int offset, label_type attr);
void print_label_map(LABELS_MAP *labels_map);

#endif /* INC_LABEL_MAP_H_ */
