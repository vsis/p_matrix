#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "crystal.h"
#include "list.h"

//******************************************************************************
node * new_node(crystal *new_element){
	node *result;
	result = (node *) malloc (sizeof(node));
	if (result != NULL){
		result->element = new_element;
	}else{
		printf ("Nodo nulo!!!\n");
	}
	return result;
}

//******************************************************************************
list * new_list(){
	list * result;
	result = (list *)malloc ( sizeof(list) );
	if (result != NULL){
		result->head = NULL;
		result->tail = NULL;
		result->i = NULL;
		result->j = NULL;
		result->size = 0;
	}else{
		printf ("lista nula!!!\n");
	}
	return result;
}

//******************************************************************************
void add_element (crystal *new_element, list *linked_list){
	node *added_node;
	added_node = new_node(new_element);
	if (linked_list->tail != NULL){
		linked_list->tail->next = added_node;
		linked_list->tail = added_node;
	}else{
		linked_list->head = added_node;
		linked_list->tail = added_node;
		linked_list->i = added_node;
		linked_list->j = added_node;
	}
	linked_list->size++;
}

//******************************************************************************
crystal * get_next_i_element(list *linked_list){
	crystal *current;
	if (linked_list->i != NULL){
		current = linked_list->i->element;
		linked_list->i = linked_list->i->next;
	}else{
		current = NULL;
	}
	return current;
}

//******************************************************************************
crystal * get_next_j_element(list *linked_list){
	crystal *current;
	if (linked_list->j != NULL){
		current = linked_list->j->element;
		linked_list->j = linked_list->j->next;
	}else{
		current = NULL;
	}
	return current;
}

//******************************************************************************
void set_ij(list *linked_list){
	if (linked_list->i != NULL){
		linked_list->j = linked_list->i;
		//linked_list->i = linked_list->i->next;
	}
}

