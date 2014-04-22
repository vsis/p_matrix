#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lor_reader.h"
#include "writer.h"
#include "debug.h"

//******************************************************************************
int get_minor(int *indexes){
	int i, minor_index;
	lor * current_minor;
	current_minor = NULL;
	minor_index = -1;
	//se asigna a 'current_minor' el menor según los indices entregados
	for (i= 0; i<number_of_packs; i++){
		if ((indexes[i] < NUMBER_OF_LORS_PER_PACK) && ( lor_packs[i][ indexes[i] ] != NULL ) ){
			current_minor = lor_packs[i][ indexes[i] ];
			minor_index = i;
			break;
		}
	}
	//se busca el menor y almacenamos el índice donde está
	for (i++; i<number_of_packs; i++){
		if ( ( indexes[i] < NUMBER_OF_LORS_PER_PACK ) && ( lor_packs[i][ indexes[i] ] != NULL ) && ( current_minor->lor_value > lor_packs[i][ indexes[i] ]->lor_value ) ){
			current_minor = lor_packs[i][ indexes[i] ];
			minor_index = i;
		}
	}
	//se almacena el menor y se retorna el índices
	current_lor = current_minor;
	return minor_index;
}


//******************************************************************************
int open_lor_writer(char *path){
	writer_output = fopen(path, "w");
	if (writer_output == NULL){
		error_msg("open_lor_writer(): no se pudo abrir el archivo de salida");
		return WRITER_ERROR;
	}
	return WRITER_SUCCESS;
}

//******************************************************************************
void write_current_lor(){
	int lor_index, current_c1, current_r1, current_c2, current_r2;
	lor_index = current_lor->lor_value;
	current_c1 = current_lor->c1;
	current_r1 = current_lor->r1;
	current_c2 = current_lor->c2;
	current_r2 = current_lor->r2;
	fwrite(&lor_index, sizeof(int), 1, writer_output);
	fwrite(&current_r1, sizeof(int), 1, writer_output);
	fwrite(&current_c1, sizeof(int), 1, writer_output);
	fwrite(&current_r2, sizeof(int), 1, writer_output);
	fwrite(&current_c2, sizeof(int), 1, writer_output);
}

//******************************************************************************
int write_all_lors(){
	int i, minor_index;
	int * indexes;
	indexes = (int *) calloc (number_of_packs, sizeof(int));
	if (indexes == NULL){
		error_msg("write_all_lors(): calloc retornó NULL para la variable 'indexes'");
		return WRITER_ERROR;
	}
	//inicializar todos los índices en 0
	for (i= 0; i< number_of_packs; i++){
		indexes[i] = 0;
	}
	//escribir de forma ordenada los lors
	for (i= 0; i< NUMBER_OF_VALID_LORS; i++){
		minor_index = get_minor(indexes);
		if (minor_index != -1){
			write_current_lor();
			free(current_lor);
			indexes[minor_index]++;
		}else{
			//se notifica un mensaje de error
			char message[64];
			sprintf(message, "write_all_lors(): get_minor() retornó -1 en la iteración nº %i",i);
			error_msg(message);
			return WRITER_ERROR;
		}
	}
	fclose(writer_output);
	return WRITER_SUCCESS;
}

