#include <stdio.h>
#include <stdlib.h>

#include "lor_reader.h"
#include "debug.h"

char message[128];

//******************************************************************************
lor * new_lor(int lor_value, int r1, int c1, int r2, int c2){
	lor * result = NULL;
	result = (lor *) malloc ( sizeof(lor) );
	if (result != NULL){
		result->lor_value = lor_value;
		result->r1 = r1;
		result->c1 = c1;
		result->r2 = r2;
		result->c2 = c2;
	}else{
		//si al sistema se le acaba la memoria al cargar los lors...
		//... es probable que en esta función, malloc no asigne memoria y retorne NULL
		sprintf (message, "new_lor(lor_value= %i, r1=%i, c1=%i, r2=%i, c2=%i) devolvió un valor NULL",lor_value, r1, c1, r2, c2);
		error_msg(message);
	}
	return result;
}

//******************************************************************************
int open_lor_reader(char * path){
	current_lor = NULL;
	lor_input_file = fopen(path, "r");
	if (lor_input_file == NULL){
		return LOR_READER_ERROR;
	} else {
		return LOR_READER_SUCCESS;
	}
}

//******************************************************************************
int get_next_lor(){
	int read_error = 0;
	free(current_lor);
	//leer los números desde el archivo
	read_error = read_error || (fread (&lor_value, sizeof(int), 1, lor_input_file) == 0);
	read_error = read_error || (fread (&r1, sizeof(int), 1, lor_input_file) == 0);
	read_error = read_error || (fread (&c1, sizeof(int), 1, lor_input_file) == 0);
	read_error = read_error || (fread (&r2, sizeof(int), 1, lor_input_file) == 0);
	read_error = read_error || (fread (&c2, sizeof(int), 1, lor_input_file) == 0);
	if ( ! read_error ){
		current_lor = new_lor(lor_value, r1, c1, r2, c2);
		if (current_lor == NULL){
			error_msg("get_next_lor(): new_lor() retornó NULL. Revisa tu memoria disponible");
			return LOR_READER_ERROR;
		}
	} else {
		warning_msg("get_next_lor(): alcanzado el final del archivo");
		current_lor = NULL;
		return LOR_READER_ERROR;
	}
	return LOR_READER_SUCCESS;
}

//******************************************************************************
void close_lor_reader(){
	fclose(lor_input_file);
}

