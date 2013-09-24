#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "crystal.h"

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
		printf ("Cristal vacío c=%i, b=%i, r=%i\n", c,b,r);
	}
	return result;
}


