#include <stdio.h>
#include <stdlib.h>

#include "lor_reader.h"
#include "debug.h"

char message[128];

//******************************************************************************
lor * new_lor(int lor_value, int r1, int c1, int r2, int c2){
	lor * result;
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
	int error;
	lor_input_file = fopen(path, "r");
	if (lor_input_file == NULL){
		return 	LOR_READER_ERROR;
	}
	number_of_packs = NUMBER_OF_VALID_LORS / NUMBER_OF_LORS_PER_PACK;
	number_of_packs++;	//El último pack va a tener menos LORs que NUMBER_OF_LORS_PER_PACK.
	lor_packs = (lor ***) calloc (number_of_packs, sizeof(lor **));
	if (lor_packs == NULL){
		sprintf (message, "open_lor_reader(path= %s): calloc devolvió NULL para lor_packs (number_of_packs = %i)", path, number_of_packs);
		error_msg(message);
		return LOR_READER_ERROR;
	}
	error = get_all_lor_packs();	//se comienza a leer los packs de lors.
	fclose(lor_input_file);
	return error;
}


//******************************************************************************
int get_next_lor_pack(){
	int i, j, read_error = 0;
	current_lor_pack = (lor **) calloc( NUMBER_OF_LORS_PER_PACK, sizeof(lor *) );
	if (current_lor_pack == NULL){
		sprintf (message, "get_next_lor_pack(): calloc() devolvió NULL para current_lor_pack");
		error_msg(message);
		return LOR_READER_ERROR;
	}
	for (i = 0; i < NUMBER_OF_LORS_PER_PACK; i++){	//para cada nuevo ítem de este pack
		//leer los números desde el archivo
		read_error = read_error || (fread (&lor_value, sizeof(int), 1, lor_input_file) == 0);
		read_error = read_error || (fread (&r1, sizeof(int), 1, lor_input_file) == 0);
		read_error = read_error || (fread (&c1, sizeof(int), 1, lor_input_file) == 0);
		read_error = read_error || (fread (&r2, sizeof(int), 1, lor_input_file) == 0);
		read_error = read_error || (fread (&c2, sizeof(int), 1, lor_input_file) == 0);
		if (read_error){
			//printf("DEBUG\tget_next_lor_pack(): fin inesperado del archivo de entrada. %i elementos leídos\n", i);
			for (j = i; j < NUMBER_OF_LORS_PER_PACK; j++){	//desde la posición i en adelante, se asigna NULL a todo el array current_lor_pack.
				current_lor_pack[j] = NULL;
			}
			break;
		}
		current_lor = new_lor(lor_value, r1, c1, r2, c2);
		current_lor_pack[i] = current_lor;
	}
	return LOR_READER_SUCCESS;
}

//******************************************************************************
int get_all_lor_packs(){
	int i, error;
	for (i = 0; i < number_of_packs; i++){	//para cada posición de array de packs
		error = get_next_lor_pack();	//leer el nuevo pack
		if (error != LOR_READER_SUCCESS){
			sprintf (message, "get_all_packs(): error al leer el pack número %i", i);
			error_msg(message);
			return LOR_READER_ERROR;
		}
		lor_packs[i] = current_lor_pack;
	}
	return LOR_READER_SUCCESS;
}

//******************************************************************************
lor * search(int index){
	int i;
	for (i= 0; i< number_of_packs; i++){
		if ( (pack_min[i] <= index) && (pack_max[i] >= index) ){
			current_lor_pack = lor_packs[i];
			return binary_search(index, 0, NUMBER_OF_LORS_PER_PACK);
		}
	}
	sprintf(message, "search(index= %i): el índice buscado no está en ningún pack", index);
	warning_msg(message);
	return NULL;
}

//******************************************************************************
lor * binary_search(int index, int begin, int end){
	int current_value, current_position;
	current_position = (begin + end) / 2;
	if ((begin > end) || (current_position >= NUMBER_OF_LORS_PER_PACK)){	//si no está en el pack se retorna NULL
		return NULL;
	}
	current_value = current_lor_pack[current_position]->lor_value;
	if (current_value > index){						//si el encontrado es mayor, se busca en la mitad superior
		return binary_search(index, begin, current_position - 1);
	}else if(current_value < index){					//si el encontrado es menor, se busca en la mitad inferior
		return binary_search(index, current_position + 1, end);
	}else{ 									//si no, significa que son iguales (current_value == index)
		return current_lor_pack[current_position];
	}
}

