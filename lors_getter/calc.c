#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "crystal.h"
#include "calc.h"

//******************************************************************************
int LOR_index(int r1, int c1, int b1, int r2, int c2, int b2){
	int o, i, lor;

	o = b2 - 1 + (BLOCKS_PER_RING * b1);
	o -= (b1 * (b1 + 3)) / 2;

	i = (o * CRYSTALS_PER_BLOCK_RING * CRYSTALS_PER_BLOCK_RING);   
	i += (c1 % CRYSTALS_PER_BLOCK_RING) * CRYSTALS_PER_BLOCK_RING;
	i += c2 % CRYSTALS_PER_BLOCK_RING;

	lor = (r1 * CRYSTALS_PER_LINE) + r2;
	lor *= LP;
	lor += i;
	
	return lor;
}	

//******************************************************************************
void set_LP(){
	LP = BLOCKS_PER_RING * (BLOCKS_PER_RING - 1);
	LP /= 2;
	LP *= CRYSTALS_PER_BLOCK_RING * CRYSTALS_PER_BLOCK_RING;
}

//******************************************************************************
int LOR_crystal(crystal *a, crystal *b){
	int c1, c2, b1, b2, r1, r2;	
	crystal *min, *max;
	if ( a->num < b->num ){			//c1 debe ser menor que c2.
		min = a;
		max = b;
	} else {
		min = b;
		max = a;
	}
	c1 = min->num;
	c2 = max->num;
	b1 = min->block;
	b2 = max->block;
	r1 = min->ring;
	r2 = max->ring;
	if ( b1 == b2 ){ 	//si tienen el mismo bloque no tiene sentido el cálculo.
		return -1;
	}else if ( get_intersection_line_image ( a, b ) ){
		return LOR_index (r1, c1, b1, r2, c2, b2);
	}else{
		return -1;
	}
}


//******************************************************************************
vector *get_intersection_line_plane (
				int plane_type,
				float p_plane, 
				vector *origin, 
				vector *direction)
{
	float x,y,z;
	x = X_SIZE + 1;
	y = Y_SIZE + 1;
	z = Z_SIZE + 1;
	switch (plane_type){
	case Z_PLANE: 	//el plano es Z, se debe calcular X e Y
		if ( direction->z == 0){ 	//si direction->z == 0 entonces existen dos posibilidades:
						//la recta está dentro del plano (intersecta en todos sus puntos),
						//o la recta está fuera del plano y es paralela a él (no intersecta nunca)
			return NULL;
		}else{
			x = (p_plane - origin->z)*(direction->x / direction->z) + origin->x;
			y = (p_plane - origin->z)*(direction->y / direction->z) + origin->y;
			z = p_plane;
		}
		break;
	case Y_PLANE:	//el plano es Y, se debe calcular X y Z
		if (direction->y == 0){
			return NULL;
		}else{
			x = (p_plane - origin->y)*(direction->x / direction->y) + origin->x;
			z = (p_plane - origin->y)*(direction->z / direction->y) + origin->z;
			y = p_plane;
		}
		break;
	case X_PLANE:	//el plano es X, se debe calcular Y y Z
		if (direction->x == 0){
			return NULL;
		}else{
			y = (p_plane - origin->x)*(direction->y / direction->x) + origin->y;
			z = (p_plane - origin->x)*(direction->z / direction->x) + origin->z;
			x = p_plane;
		}
	}
	return new_vector (x,y,z);
}

//******************************************************************************
int get_intersection_line_image (crystal *a, crystal *b){
	vector *origin, *direction;
	origin = a->position;
	direction = diff(b->position, a->position);
	//variables que representan la cara actual de un voxel, el tipo de plano, y el índice del punto encontrado
	int voxel_face, plane_type, index=0,result; 	
	vector *aux, *inter[2];
	inter[0] = inter[1] = NULL;
	//calcula la intersección de la recta con los 6 planos que contiene las caras del voxel
	for (voxel_face = 0; voxel_face < 6; voxel_face++){
		plane_type = voxel_face / 2; //cada dos caras del voxel se cambia el tipo de plano
		aux = get_intersection_line_plane(plane_type, p_planes[voxel_face], origin, direction);	
		if ( (aux != NULL) && ( inside( aux ) ) ){ //determina si la intersección está dentro de la cara
			inter[index] = aux;
			index++;
			if (index == 2 && equal(inter[0], inter[1]) ){ //si hay dos puntos iguales se ignora el segundo
				index--;
				free(inter[1]);
				inter[1] = NULL;
			}else if(index == 2){ //si hay dos puntos distintos se termina el cálculo.
				break;
			}
		}else{
			free( aux );
		}
	}
	
	result = (inter[0] != NULL) && (inter[1] != NULL);
	free ( inter[0] );
	free ( inter[1] );
	free ( direction );
	return result;
}

//******************************************************************************
int inside (vector *dot){
	float dx, dy, dz;
	int result;
	dx = abs (dot->x) * 2;
	dy = abs (dot->y) * 2; 
	dz = abs (dot->z) * 2;
	result = ( (dx <= X_SIZE) && (dy <= Y_SIZE) && (dz <= Z_SIZE));
	return result;
}


//******************************************************************************
void set_planes(){
	p_planes[0] = Z_SIZE / 2;//planos Z
	p_planes[1] = - Z_SIZE / 2;
	p_planes[2] = Y_SIZE / 2;//planos Y
	p_planes[3] = - Z_SIZE / 2;
	p_planes[4] = X_SIZE / 2;//planos X
	p_planes[5] = - X_SIZE / 2;
}
