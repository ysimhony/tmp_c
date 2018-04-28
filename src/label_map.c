/*
 * label_map.c
 *
 *  Created on: 4 ????? 2017
 *      Author: yacov
 */

#include "label_map.h"

#define LAEBEL_MAP_ERROR(msg) \
if (1) { \
	printf("\n"); \
	printf("%s\n", msg); \
	printf("At file %s: %d", __FILE__, __LINE__); \
	exit(EXIT_FAILURE); \
};


void set_symbol_value(LABELS_MAP *label_map, char *symbol, int value) {
	int symbol_len = strlen(symbol);
	LABEL *label_entry;

	label_entry = label_map->labels_table;
	while (label_entry != NULL) {

		if (0 == strncmp(label_entry->label, symbol, symbol_len)) {
			label_entry->value = value;
			break;
		}

		label_entry = label_entry->next;
	}

	if (label_entry == NULL) {
		print_label_map(label_map);
		LAEBEL_MAP_ERROR("label was not found in label map");
	}
}

int get_symbol_value(LABELS_MAP *labels_map, char *symbol) {
	int result = -1;
	int symbol_len = strlen(symbol);
	LABEL *label_entry;

	label_entry = labels_map->labels_table;

	while (label_entry != NULL) {

		if (0 == strncmp(label_entry->label, symbol, symbol_len)) {
			result = label_entry->value;
			break;
		}
		label_entry = label_entry->next;
	}

	return result;
}

label_type get_symbol_attr(LABELS_MAP *labels_map, char *symbol) {
	label_type result = LABEL_NONE;
	int symbol_len = strlen(symbol);
	LABEL *label_entry;

	label_entry = labels_map->labels_table;

	while (label_entry != NULL) {

		if (0 == strncmp(label_entry->label, symbol, symbol_len)) {
			result = label_entry->type;
			break;
		}
		label_entry = label_entry->next;
	}

	return result;
}

void store_symbol(LABELS_MAP *labels_map, char *symbol, int value, label_type attr) {

	LABEL *label_entry;

	if (-1 != get_symbol_value(labels_map, symbol)) {
		print_label_map(labels_map);
		LAEBEL_MAP_ERROR("Symbol already exists in label map");
//		exit(EXIT_FAILURE);
	}

	// Allocate a new label entry
	label_entry = (LABEL *)malloc(sizeof(LABEL));

	if (label_entry == NULL) {
		LAEBEL_MAP_ERROR("Alocation failed");
	}

	// Set the label entry fields
	label_entry->label = (char *)malloc((strlen(symbol)+1) * sizeof(char));
	strncpy(label_entry->label, symbol, strlen(symbol)+1);
	label_entry->value = value;
	label_entry->type = attr;
	label_entry->next = NULL;

	if (labels_map->labels_table == NULL) {
		labels_map->labels_table = label_entry;
		labels_map->itr = label_entry;
	}
	else {
		// Add the new entry to the head of the label map
		labels_map->itr->next = label_entry;
		labels_map->itr = label_entry;
	}
}


void update_labels_by_attr(LABELS_MAP *labels_map, int offset, label_type attr) {
	LABEL *label_entry;

	label_entry = labels_map->labels_table;

	while (label_entry != NULL){
		if (label_entry->type == LABEL_DATA){
			label_entry->value += offset;
		}

		label_entry = label_entry->next;
	}

}


void print_label_map(LABELS_MAP *labels_map) {

	LABEL *label_entry;

	label_entry = labels_map->labels_table;

	printf("*************************\n");
	printf("Label Map:\n");
	printf("*************************\n");
	printf("%-10s%-10s%-s\n", "Label", "Value", "Type");

	while (label_entry != NULL){

	   printf("%-10s %-10d %d\n", label_entry->label, label_entry->value, label_entry->type);
	   label_entry = label_entry->next;
	}

}

void mark_symbol_as_entry(LABELS_MAP *label_map, char *symbol) {
	int symbol_len = strlen(symbol);
	LABEL *label_entry;

	label_entry = label_map->labels_table;
	while (label_entry != NULL) {

		if (0 == strncmp(label_entry->label, symbol, symbol_len)) {
			break;
		}

		label_entry = label_entry->next;
	}

	if (label_entry == NULL) {
		print_label_map(label_map);
		LAEBEL_MAP_ERROR("label was not found in label map");
	}

	label_entry->is_entry = TRUE;
}


bool is_symbol_entry(LABELS_MAP *label_map, char *symbol){
	int symbol_len = strlen(symbol);
	LABEL *label_entry;

	label_entry = label_map->labels_table;
	while (label_entry != NULL) {

		if (0 == strncmp(label_entry->label, symbol, symbol_len)) {
			break;
		}

		label_entry = label_entry->next;
	}

	if (label_entry == NULL) {
		print_label_map(label_map);
		LAEBEL_MAP_ERROR("label was not found in label map");
	}

	return label_entry->is_entry;

}

void get_next_label_as_entry_reset(LABELS_MAP *labels_map){
	if (NULL == labels_map) {
		LAEBEL_MAP_ERROR("label map is NULL!!!");
	}

	labels_map->label_as_entry_itr = labels_map->labels_table;
}

char *get_next_label_as_entry(LABELS_MAP *labels_map, uint *value, label_type *type){
	LABEL *label_entry;
	char *tmp_str;
	uint tmp_uint;
	label_type tmp_type;

	if (NULL == labels_map) {
		LAEBEL_MAP_ERROR("label map is NULL!!!");
	}

	label_entry = labels_map->label_as_entry_itr;

	while (label_entry != NULL && label_entry->is_entry == FALSE) {
		label_entry = label_entry->next;
	}

	if (NULL == label_entry) {
		return NULL;
	}

	tmp_str = label_entry->label;
	tmp_uint = label_entry->value;
	tmp_type = label_entry->type;

	labels_map->label_as_entry_itr = label_entry->next;

	*value = tmp_uint;
	*type = tmp_type;
	return tmp_str;

}
