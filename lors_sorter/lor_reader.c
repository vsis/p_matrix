#include <stdio.h>
#include <stdlib.h>

#include "lor_reader.h"

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
		printf ("DEBUG\tnew_lor(lor_value= %i, r1=%i, c1=%i, r2=%i, c2=%i) devolvió un valor NULL\n",lor_value, r1, c1, r2, c2);
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
		printf ("DEBUG\t open_lor_reader(path= %s): calloc devolvió NULL para lor_packs (number_of_packs = %i)", path, number_of_packs);
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
		printf ("DEBUG\t get_next_lor_pack(): calloc() devolvió NULL para current_lor_pack\n");
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
			printf("DEBUG\tget_next_lor_pack(): fin inesperado del archivo de entrada. %i elementos leídos\n", i);
			for (j = i; j < NUMBER_OF_LORS_PER_PACK; j++){	//desde la posición i en adelante, se asigna NULL a todo el array current_lor_pack.
				current_lor_pack[j] = NULL;
			}
			break;
		}
		current_lor = new_lor(lor_value, r1, c1, r2, c2);
		current_lor_pack[i] = current_lor;
	}
	sort_lor_pack(0, i - 1);	//ordenar el pack, por lor_value de menor a mayor.
	return LOR_READER_SUCCESS;
}

//******************************************************************************
void swap_lor_in_current_pack(int i, int j){
	lor * aux;
	aux = current_lor_pack[i];
	current_lor_pack[i] = current_lor_pack[j];
	current_lor_pack[j] = aux;	
}


//******************************************************************************
void sort_lor_pack(int begin, int end){
	int i, pivot_value, pivot;
	if (begin < end){
		pivot = begin;	//el pivote siempre es el primer elemento del subarray.
		pivot_value = current_lor_pack[pivot]->lor_value;
		for (i = begin + 1; i <= end; i++){
			if ( current_lor_pack[i]->lor_value < pivot_value ){
				pivot++;
				swap_lor_in_current_pack(i, pivot);
			}
		}
		swap_lor_in_current_pack(begin, pivot);
		sort_lor_pack (begin, pivot - 1);
		sort_lor_pack (pivot + 1, end);
	}
}

//******************************************************************************
int get_all_lor_packs(){
	int i, error;
	for (i = 0; i < number_of_packs; i++){	//para cada posición de array de packs
		error = get_next_lor_pack();	//leer el nuevo pack
		if (error != LOR_READER_SUCCESS){
			printf ("DEBUG\tget_all_packs(): error al leer el pack número %i\n", i);
			return LOR_READER_ERROR;
		}
		lor_packs[i] = current_lor_pack;
	}
	return LOR_READER_SUCCESS;
}


