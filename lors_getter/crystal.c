#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "crystal.h"
#include "debug.h"

//******************************************************************************
crystal * new_crystal(int c, int b, int r, vector * pos){
	crystal * result;
	result = (crystal *)malloc( sizeof(crystal) );
	if (result != NULL){
		result->num = c;
		result->block = b;
		result->ring = r;
		result->position = pos;
	} else {
		error_msg("new_crystal(): malloc retornó NULL");
	}
	return result;
}


