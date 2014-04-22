#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CL/opencl.h>

#include "crystal_reader.h"
#include "lor_reader.h"
#include "deployer.h"
#include "cl_reader.h"
#include "vector.h"
#include "cpu_calc.h"
#include "debug.h"
#include "p_getter.h"

//******************************************************************************
void set_img_size(int x, int y, int z){
	num_x_voxels = x;
	num_y_voxels = y;
	num_z_voxels = z;
	num_of_voxels = num_x_voxels * num_y_voxels * num_z_voxels;
	if ( (num_x_voxels < 1) || (num_y_voxels < 1) || (num_z_voxels < 1) ){
		error_msg("set_img_size(): número inválido de voxeles");
	}
}

//******************************************************************************
void set_voxel_size(float x, float y, float z){
	delta_x = x;
	delta_y = y;
	delta_z = z;
	if ( (delta_x < 1) || (delta_y < 1) || (delta_z < 1) ){
		error_msg("set_voxel_size(): tamaño de voxel inválido");
	}
	delta = new_vector(delta_x, delta_y, delta_z);
}

//******************************************************************************
void set_voxel0_position(float x, float y, float z){
	voxel0_x = x;
	voxel0_y = y;
	voxel0_z = z;
	voxel0 = new_vector(voxel0_x, voxel0_y, voxel0_z);
}

//******************************************************************************
void set_p_method(int method){
	p_method = method;
	if ((p_method != P_GPU) && (p_method != P_CPU)){
		error_msg("set_p_method(): método de cálculo inválido");
	} else if (p_method == P_CPU){
		warning_msg("set_p_method(): seleccionado método de cálculo por CPU");
	}
 }

//******************************************************************************
float * get_p_row_cpu(){
	float * p_row, x, y, z;
	int voxel_index, index_x, index_y, index_z;
	vector * v_index, *origin, *destiny;
	
	p_row = (float *) calloc(num_of_voxels, sizeof(float));
	if (p_row == NULL){
		error_msg("get_p_row(): calloc retornó null para p_row");
		return NULL;
	}
	#pragma omp parallel for shared(p_row), private(index_x, index_y, index_z, v_index, origin, destiny, x, y, z)
	for (voxel_index = 0; voxel_index < num_of_voxels; voxel_index++){
		index_z = voxel_index % num_z_voxels;
		index_y = (voxel_index / num_z_voxels) % num_y_voxels;
		index_x = (voxel_index / num_y_voxels) / num_z_voxels;
		v_index = new_vector(index_x, index_y, index_z);
		get_crystal_position(current_lor->r1,current_lor->c1, &x, &y, &z);
		origin = new_vector(x, y, z);
		get_crystal_position(current_lor->r2,current_lor->c2, &x, &y, &z);
		destiny = new_vector(x, y, z);
		p_row[voxel_index] = segment(origin, destiny, delta, v_index, voxel0);
		if (p_row[voxel_index] != 0){
			char message[64];
			sprintf(message, "voxel_index=%i p=%.4f", voxel_index, p_row[voxel_index]);
			#pragma omp critical
			info_msg(message);
		}
		free(v_index);
		free(origin);
		free(destiny);
	}
	return p_row;
}


//******************************************************************************
float * get_p_row(){
	if (p_method == P_CPU){
		return get_p_row_cpu();
	}
	return NULL;
}

