#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "reader.h"

//******************************************************************************
int start_reader (char *reader_path, FILE **crys_file){
	char buffer[LINE_SIZE];
	*crys_file = fopen (reader_path, "r");
	if (*crys_file != NULL){
		fgets(buffer, LINE_SIZE, *crys_file);		//descartar la primera línea del archivo
		return READER_SUCCESS;
	}	
	return READER_ERROR;
}

//******************************************************************************
int read_crys (vector **crystal, int *crys_id, int *block, int *ring, FILE *crys_file){
	char buffer[LINE_SIZE];
	float x,y,z;
	if ( fgets (buffer, LINE_SIZE, crys_file) ){
		sscanf (buffer, "%i\t\t%i\t\t%i\t\t%f %f %f", 
			ring, block, crys_id, &x, &y, &z);
		*crystal = new_vector (x, y, z);
		return READER_SUCCESS;
	}
	return READER_EOF;
	
}
