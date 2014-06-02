#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "cpu_calc.h"
#include "debug.h"

//******************************************************************************
vector *get_voxel(vector *position, vector *voxel0, vector *delta){
	vector *multiplication, *result;
	multiplication = mult(position, delta);
	result = sum(voxel0, multiplication);
	free(multiplication);
	return result;
}

//******************************************************************************
vector *get_inter_line_plane (
				int plane_type,
				float p_plane, 
				vector *origin, 
				vector *direction)
{
	switch (plane_type){
	case Z_PLANE: 	//el plano es Z, se debe calcular X e Y
		if ( direction->z == 0){ 	//si direction->z == 0 entonces existen dos posibilidades:
						//la recta está dentro del plano (intersecta en todos sus puntos),
						//o la recta está fuera del plano y es paralela a él (no intersecta nunca)
			//error_msg("get_inter_line_plane(): recibido Z_PLANE y direction->z = 0. No se puede calcular la intersección");
			return NULL;
		}else{
			float x,y;
			x = (p_plane - origin->z)*(direction->x / direction->z) + origin->x;
			y = (p_plane - origin->z)*(direction->y / direction->z) + origin->y;
			return new_vector (x, y, p_plane);
		}
	case Y_PLANE:	//el plano es Y, se debe calcular X y Z
		if (direction->y == 0){
			//error_msg("get_inter_line_plane(): recibido Y_PLANE y direction->y = 0. No se puede calcular la intersección");
			return NULL;
		}else{
			float x,z;
			x = (p_plane - origin->y)*(direction->x / direction->y) + origin->x;
			z = (p_plane - origin->y)*(direction->z / direction->y) + origin->z;
			return new_vector (x, p_plane, z);
		}
	case X_PLANE:	//el plano es X, se debe calcular Y y Z
		if (direction->x == 0){
			//error_msg("get_inter_line_plane(): recibido X_PLANE y direction->x = 0. No se puede calcular la intersección");
			return NULL;
		}else{
			float y,z;
			y = (p_plane - origin->x)*(direction->y / direction->x) + origin->y;
			z = (p_plane - origin->x)*(direction->z / direction->x) + origin->z;
			return new_vector (p_plane, y, z);
		}
	}
	error_msg("get_inter_plane(): se recibió un tipo de plano desconocido");
	return NULL;
}

//******************************************************************************
float get_inter_line_voxel (
				vector *voxel_index,
				vector *voxel0,
				vector *delta,
				vector *origin,
				vector *direction){
	//variables que representan la cara actual de un voxel, el tipo de plano, y el índice del punto encontrado
	int voxel_face, plane_type, index=0; 
	float p_planes[6], result=0;	
	vector *voxel=NULL, *aux=NULL, *inter[2], *difference=NULL;
	inter[0] = inter[1] = NULL;
	voxel = get_voxel(voxel_index, voxel0, delta);
	//definición de los planos
	p_planes[0] = voxel->z + (delta->z / 2);//planos Z
	p_planes[1] = voxel->z - (delta->z / 2);
	p_planes[2] = voxel->y + (delta->y / 2);//planos Y
	p_planes[3] = voxel->y - (delta->y / 2);
	p_planes[4] = voxel->x + (delta->x / 2);//planos X
	p_planes[5] = voxel->x - (delta->x / 2);
	//calcula la intersección de la recta con los 6 planos que contiene las caras del voxel
	for (voxel_face = 0; voxel_face < 6; voxel_face++){
		plane_type = voxel_face / 2; //cada dos caras del voxel se cambia el tipo de plano
		aux = get_inter_line_plane(plane_type, p_planes[voxel_face], origin, direction);	
		if ( (aux != NULL) && (inside(voxel, aux, delta)) ){ //determina si la intersección está dentro de la cara
			inter[index] = aux;
			index++;
			if (  (index == 2) && ( equal(inter[0], inter[1]) ) ){ //si hay dos puntos iguales se ignora el segundo
				index = 1;
				free (inter[1]);
				inter[1] = NULL;
			}else if(index == 2){ //si hay dos puntos distintos se calcula la distancia.
				difference = diff(inter[0], inter[1]);
				result =  module ( difference );
				free(inter[0]);
				free(inter[1]);
				inter[0] = inter[1] = NULL;
			}
		} else {
			free(aux);
			aux = NULL;
		}
	}
	free(voxel);
	free(difference);
	return result;
}

//******************************************************************************
float segment (vector *crystal0, vector *crystal1, vector *delta, vector *voxel_index, vector *voxel0){
	vector *direction;
	float result;
	direction = diff(crystal1, crystal0);
	result = get_inter_line_voxel(voxel_index, voxel0, delta, crystal0, direction);
	free(direction);
	if (result < 0.25){
		result = 0;
	}
	return result;
}

