#include <stdio.h>
#include <stdlib.h>

#include "crystal_reader.h"
#include "debug.h"

char message[64];

//******************************************************************************
int open_crystal_reader(char * path){	//carga los cristales del archivo path en memoria
	int error;
	crystal_input_file = fopen(path, "r");
	if (crystal_input_file == NULL){
		return CRYSTAL_READER_ERROR;
	}
	positions_x = (float *) calloc( NUMBER_OF_ELEMENTS, sizeof(float) );
	positions_y = (float *) calloc( NUMBER_OF_ELEMENTS, sizeof(float) );
	positions_z = (float *) calloc( NUMBER_OF_ELEMENTS, sizeof(float) );
	//si calloc devuelve NULL para algún array, se devuelve error.
	if ((positions_x == NULL) || (positions_y == NULL)  || (positions_z == NULL)){
		fclose(crystal_input_file);
		return CRYSTAL_READER_ERROR;
	}
	error = get_crystal_elements();	//se comienza a leer los cristales.
	fclose(crystal_input_file);
	return error;
}

//******************************************************************************
int get_crystal_elements(){
	int ring, block, crystal, current_element, index;
	float x0, y0, z0;
	for (current_element = 0; current_element < NUMBER_OF_ELEMENTS; current_element++){ 	//para cada uno de los elementos
												//se lee un cristal y se asigna a la posición 'index' de los arrays de ubicación
		if ( feof(crystal_input_file) ){
			sprintf(message, "crystal_get_elements(): fin inesperado del archivo. elementos leídos %i", current_element);
			error_msg(message);
			return CRYSTAL_READER_ERROR;
		}
		fscanf (crystal_input_file, "%i\t\t%i\t\t%i\t\t%f %f %f\n",&ring, &block, &crystal, &x0, &y0, &z0);
		index = get_crystal_index(ring,crystal);	//se calcula la posición que corresponde a ese cristal.
		positions_x[index] = x0;			//se almacenan la posición en los arrays.
		positions_y[index] = y0;
		positions_z[index] = z0;
	}
	return CRYSTAL_READER_SUCCESS;
}

//******************************************************************************
int get_crystal_index(int ring, int crystal){
	int index;
	index = crystal * NUMBER_OF_RINGS;	//los cristales están ordenados por número de cristal...
	index += ring;				//... pero para números de cristal iguales, se ordenan por número de anillo.
	if (index >= NUMBER_OF_ELEMENTS){
		//vamos a tener un error del tipo "out of array index"
		sprintf(message, "get_index(ring= %i, crystal= %i): se calculó un índice mayor que el permitido", ring, crystal);
		error_msg(message);
	}
	return index;
}

//******************************************************************************
void get_crystal_position (int ring, int crys, float *x0, float *y0, float *z0){
	int index = get_crystal_index(ring, crys);
	*x0 = positions_x[index];
	*y0 = positions_y[index];
	*z0 = positions_z[index];
}

