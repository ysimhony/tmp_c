/*
 * label_map.c
 *
 *  Created on: 4 ????? 2017
 *      Author: yacov
 */

#include "label_map.h"

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
void store_symbol(LABELS_MAP *labels_map, char *symbol, int value, label_type attr) {

	if (-1 != get_symbol_value(labels_map, symbol)) {
		print_label_map(labels_map);
		LAEBEL_MAP_ERROR("Symbol already exists in label map");
//		exit(EXIT_FAILURE);
	}

	// Allocate a new label entry
	labels_map->itr = (LABEL *)malloc(sizeof(LABEL));

	// Set the label entry fields
	labels_map->itr->label = (char *)malloc((strlen(symbol)+1) * sizeof(char));
	strncpy(labels_map->itr->label, symbol, strlen(symbol)+1);
	labels_map->itr->value = value;
	labels_map->itr->type = attr;

	// Add the new entry to the head of the label map
	labels_map->itr->next = labels_map->labels_table;
	labels_map->labels_table = labels_map->itr;

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

