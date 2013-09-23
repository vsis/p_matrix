#include "vector.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//******************************************************************************
float module(vector *entrada){
	float x2 = pow(entrada->x, 2);
	float y2 = pow(entrada->y, 2);
	float z2 = pow(entrada->z, 2);
	return sqrt(x2 + y2 + z2);
}

//******************************************************************************
vector *diff(vector *v0, vector *v1){
	float x2 = v0->x - v1->x;
	float y2 = v0->y - v1->y;
	float z2 = v0->z - v1->z;
	return new_vector (x2, y2, z2);
}

//******************************************************************************
vector *sum(vector *v0, vector *v1){
	float x2 = v0->x + v1->x;
	float y2 = v0->y + v1->y;
	float z2 = v0->z + v1->z;
	return new_vector (x2, y2, z2);
}
//******************************************************************************
vector *mult(vector *v0, vector *v1){
	float x2 = v0->x * v1->x;
	float y2 = v0->y * v1->y;
	float z2 = v0->z * v1->z;
	return new_vector (x2, y2, z2);	
}

//******************************************************************************
vector *new_vector(float x0, float y0, float z0){
	vector *new;
	new = (vector *) malloc(sizeof(vector));
	new->x = x0;
	new->y = y0;
	new->z = z0;
	/***/if (new == NULL) {printf("vector nulo!!!\n");}
	return new;
}

//******************************************************************************
int inside (vector *voxel, vector *dot, vector *delta){
	/**************************************
	printf ("\n\t::inside\n");
	printf ("\tvoxel = (%f, %f, %f)\n", voxel->x, voxel->y, voxel->z);
	printf ("\tdot = (%f, %f, %f)\n", dot->x, dot->y, dot->z);
	**************************************/
	float dx, dy, dz;
	int result;
	dx = abs (voxel->x - dot->x);
	dy = abs (voxel->y - dot->y); 
	dz = abs (voxel->z - dot->z);
	result = ( (dx <= delta->x) && (dy <= delta->y) && (dz <= delta->z));
	/**************************************
	if (result){
		printf ("\t::return = ¡adentro!\n");
	}else{
		printf ("\t::return = afuera\n");
	}
	**************************************/
	return result;
}

//******************************************************************************
int equal (vector *v0, vector *v1){
	int eq_x, eq_y, eq_z;
	eq_x = v0->x == v1->x;
	eq_y = v0->y == v1->y;
	eq_z = v0->z == v1->z;
	return (eq_x && eq_y && eq_z);
}

//******************************************************************************
vector * mult_esc (float esc, vector *v1){
	float x2 = v1->x * esc;
	float y2 = v1->y * esc;
	float z2 = v1->z * esc;
	return new_vector (x2, y2, z2);
}

