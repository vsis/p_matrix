#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "calc.h"
#include "debug.h"

//******************************************************************************
void init_variables(){
	angle_per_block = (2.00 * M_PI)  / BLOCKS_PER_RING; 	//el escáner tiene 6 bloques en un anillo
	radius = 15.515;		
	inter_crystal = 0.067;
	inter_crystal_half = inter_crystal / 2.00;
	crystal_width = 2.162;
	crystal_width_half = crystal_width / 2.00;
	width_half = inter_crystal_half + crystal_width_half;
	block_size = 8 * ( crystal_width + inter_crystal );
	block_size_half = block_size / 2.00;
	inter_block = crystal_width;
	initial_z_axis = -3.5 * (inter_crystal + crystal_width);
}


//******************************************************************************
crystal * create_crystal(int _num, int _block, int _ring, vector *_position){
	crystal *new_crystal;
	new_crystal = (crystal *) malloc( sizeof( crystal ) );
	if (new_crystal != NULL){
		new_crystal->num = _num;
		new_crystal->block = _block;
		new_crystal->ring = _ring;
		new_crystal->position = _position;
		return new_crystal;
	}
	error_msg("create_crystal(): malloc ha retornado NULL");
	return NULL;
}

//******************************************************************************
vector * calc_block(int block_num){
	float x,y;
	x = cosf( block_num * angle_per_block ) * radius;
	y = sinf( block_num * angle_per_block ) * radius;
	return new_vector (x, y, 0.00);
}
 
//******************************************************************************
crystal **calc_crystals (int block_num, int crystal_num, vector *block_pos){
	vector *block_norm, *perpendicular, *aux;
	crystal ** block_crystals;
	float block_mod;
	int i;
	block_crystals = (crystal **)calloc (CRYSTALS_PER_BLOCK_RING, sizeof( crystal *) );	//en cada bloque del anillo hay 13 cristales
	if (block_crystals != NULL){	
		block_mod = module (block_pos);
		block_norm = new_vector ( block_pos->x / block_mod, block_pos->y / block_mod, 0.00 );
		perpendicular = new_vector (block_norm->y * -1.00 , block_norm->x, 0.00);
		aux = sum (block_pos, mult_esc( -block_size_half, perpendicular) );
		for (i = 0; i < CRYSTALS_PER_BLOCK_RING; i++){			//se ubican los trece cristales del anillo bloque
			aux = sum(aux, mult_esc( width_half, perpendicular ) );
			block_crystals[i] = create_crystal (crystal_num, block_num, 0, aux);
			aux = sum(aux, mult_esc( width_half, perpendicular ) );
			
		}
		//***/printf ("terminado el bloque %i\n", block_num);
		return block_crystals;
	}
	error_msg("calc_crystals(): malloc ha retornado NULL");
	return NULL;
}

//******************************************************************************
crystal ** crystal_line (crystal *initial, int block_num, int crystal_num){
	int i,j,index;
	float x,y,z_axis;
	crystal *aux, **line;
	if (initial == NULL)
	{
		warning_msg("crystal_line(): se recibió el parámetro initial=NULL");
		return NULL;
	}
	z_axis = initial_z_axis;
	x = initial->position->x;
	y = initial->position->y;
	line = (crystal **) calloc (CRYSTALS_PER_LINE, sizeof(crystal *) );
	for (i = 0; i < BLOCKS_PER_LINE; i++){
		for (j= 0; j< CRYSTALS_PER_BLOCK_RING; j++){
			index = j + i * CRYSTALS_PER_BLOCK_RING;
			aux = create_crystal (crystal_num, block_num, index, 
				new_vector(x, y, z_axis)
			);
			line[index] = aux;
			z_axis += inter_crystal + crystal_width;
		}
		z_axis += inter_block;
	}
	return line;
}

