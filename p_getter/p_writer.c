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
	max_a2b = number_of_total_lors * 92;
	a2r = (int *) calloc(2 * number_of_total_lors, sizeof(int));
	if (a2r == NULL){
		error_msg("init_p(): calloc retornó NULL para a2r");
		return P_WRITER_ERROR;
	}
	a2b = (int *) calloc(max_a2b, sizeof(int));
	if (a2b == NULL){
		error_msg("init_p(): calloc retornó NULL para a2b");
		return P_WRITER_ERROR;
	}
	a2p = (float *) calloc(max_a2b, sizeof(float));
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
	a2r[(lor_index * 2) + 1] = 0;
	if (segments != NULL){
		for (column = 0; column < number_of_voxels; column++){
			current_segment_voxel = segments[column];
			if (current_segment_voxel != 0){
				a2r[(lor_index * 2) + 1]++;
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
	int written_elements, i, nonzero_elems, offset = 0;
	char message[128];
	written_elements = fwrite( (void *) a2r, sizeof(int), number_of_total_lors * 2, P_FILE);
	if (written_elements != number_of_total_lors * 2){
		error_msg("error al escribir el array a2r");
		return P_WRITER_ERROR;
	}
	for (i = 0; i < number_of_total_lors; i++){
		nonzero_elems = a2r[(i*2) + 1];	//el número de elementos no-cero para el lor i
		if (nonzero_elems != 0){
			written_elements = fwrite( (void *) &(a2b[offset]), sizeof(int), nonzero_elems, P_FILE );
			if (written_elements != nonzero_elems){
				error_msg("error al escribir el array a2b");
				return P_WRITER_ERROR;
			}
			written_elements = fwrite( (void *) &(a2p[offset]), sizeof(float), nonzero_elems, P_FILE );
			if (written_elements != nonzero_elems){
				error_msg("error al escribir el array a2p");
				return P_WRITER_ERROR;
			}
			offset += nonzero_elems;
			if (offset > number_of_nonzero_elems){
				sprintf(message, "write_p_matrix(): offset = %i, number_of_nonzero_elems = %i, i = %i, nonzero_elems = %i", offset, number_of_nonzero_elems, i, nonzero_elems);
				error_msg(message);
				return P_WRITER_ERROR;
			}
		}
	}
	return P_WRITER_SUCCESS;
}

//**********************************************************************
void close_p_file(){
	fclose(P_FILE);
}

