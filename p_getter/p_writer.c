#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"
#include "p_writer.h"

//**********************************************************************
int init_p(int max_lors, int num_of_voxels){
	number_of_total_lors = max_lors;
	number_of_voxels = num_of_voxels;
	number_of_nonzero_elems = 0;
	max_a2b = number_of_total_lors * 50;
	a2r = (int *) calloc((2 * number_of_total_lors) + 1, sizeof(int));
	if (a2r == NULL){
		error_msg("init_p(): calloc retornó NULL para a2r");
		return P_WRITER_ERROR;
	}
	a2b = (int *) calloc(max_a2b, sizeof(int));
	if (a2b == NULL){
		error_msg("init_p(): calloc retornó NULL para a2b");
		return P_WRITER_ERROR;
	}
	a2p = (float *) calloc(max_a2b, sizeof(int));
	if(a2p == NULL){
		error_msg("init_p(): calloc retornó NULL para a2p");
		return P_WRITER_ERROR;
	}
	return P_WRITER_SUCCESS;
}

//**********************************************************************
int open_p_file(char * path){
	P_FILE = fopen(path, "w");
	if (P_FILE == NULL){
		error_msg("open_p_file(): no se pudo abrir el archivo de salida");
		return P_WRITER_ERROR;
	}
	return P_WRITER_SUCCESS;
}

//**********************************************************************
int add_lor(int lor_index, float * segments){
	int column;
	float current_segment_voxel;
	a2r[lor_index * 2] = lor_index;
	if (segments == NULL){
		a2r[(lor_index * 2) + 1] = 0;
	} else {
		for (column = 0; column < number_of_voxels; column++){
			current_segment_voxel = segments[column];
			if (current_segment_voxel != 0){
				a2b[number_of_nonzero_elems] = column;
				a2p[number_of_nonzero_elems] = current_segment_voxel;
				number_of_nonzero_elems++;
				if (number_of_nonzero_elems >= max_a2b){
					error_msg("add_lor(): number_of_nonzero_elems es mayor o igual que max_a2b");
					return P_WRITER_ERROR;
				}
			}
		}
	}
	return P_WRITER_SUCCESS;
}

//**********************************************************************
int write_p_matrix(){
	int written_elements;
	written_elements = fwrite(a2r, sizeof(int), number_of_total_lors, P_FILE);
	if (written_elements != number_of_total_lors){
		error_msg("error al escribir el array a2r");
		return P_WRITER_ERROR;
	}
	written_elements = fwrite(a2b, sizeof(int), number_of_nonzero_elems, P_FILE);
	if (written_elements != number_of_nonzero_elems){
		error_msg("error al escribir el array a2b");
		return P_WRITER_ERROR;
	}
	written_elements = fwrite(a2p, sizeof(float), number_of_nonzero_elems, P_FILE);
	if (written_elements != number_of_nonzero_elems){
		error_msg("error al escribir el array a2p");
		return P_WRITER_ERROR;
	}
	return P_WRITER_SUCCESS;
}

//**********************************************************************
void close_p_file(){
	fclose(P_FILE);
}

