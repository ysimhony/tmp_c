/*
 * label_map.h
 *
 *  Created on: 4 ????? 2017
 *      Author: yacov
 */

#ifndef INC_LABEL_MAP_H_
#define INC_LABEL_MAP_H_

#define _GNU_SOURCE

#include "stdio.h"
#include "stdlib.h"
#include "string.h"



typedef enum { FALSE, TRUE } bool;
typedef enum {LABEL_NONE, LABEL_DATA, LABEL_CODE} label_type;

typedef struct _LABEL
{
   char *label;
   int value;
   label_type type;
   bool is_entry;
   struct _LABEL *next;
} LABEL;

typedef struct _LABELS_MAP
{
   LABEL *labels_table;
   LABEL *itr;//points to the last label entry
   LABEL *label_as_entry_itr;
} LABELS_MAP;

void store_symbol(LABELS_MAP *labels_map, char *symbol, int value, label_type attr);
void set_symbol_value(LABELS_MAP *label_map, char *symbol, int value);
int get_symbol_value(LABELS_MAP *label_map, char *symbol);
label_type get_symbol_attr(LABELS_MAP *labels_map, char *symbol);
void update_labels_by_attr(LABELS_MAP *labels_map, int offset, label_type attr);
void mark_symbol_as_entry(LABELS_MAP *label_map, char *symbol);
bool is_symbol_entry(LABELS_MAP *label_map, char *symbol);
void print_label_map(LABELS_MAP *labels_map);
void get_next_label_as_entry_reset(LABELS_MAP *labels_map);
char *get_next_label_as_entry(LABELS_MAP *labels_map, uint *value, label_type *type);

#endif /* INC_LABEL_MAP_H_ */
