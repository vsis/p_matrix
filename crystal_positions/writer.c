#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "writer.h"
#include "debug.h"

//******************************************************************************
int start_logger (char *logger_path, FILE **log_file){
	*log_file = fopen (logger_path, "w");
	if (*log_file != NULL){
		//fprintf (*log_file,"RING NUM\tBLOCK NUM\tCRYSTAL NUM\t--- POSITION ---\n");
		return WRITER_SUCCESS;
	}
	else{
		return WRITER_ERROR;
	}
}

//******************************************************************************
void log_crystal(int ring_num, int block_num, int crystal_num, vector *crystal_position, FILE *log_file){
	if (crystal_position != NULL){
		fprintf(log_file, 
			"%i\t\t%i\t\t%i\t\t%.4f %.4f %.4f\n",
			ring_num, 
			block_num,
			crystal_num,
			crystal_position->x,
			crystal_position->y,
			crystal_position->z);
	} else {
		error_msg("Se intentó escribir un vector nulo sobre el archivo de salida");
	}
}

