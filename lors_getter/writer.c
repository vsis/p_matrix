#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "crystal.h"
#include "writer.h"

#define LINE_SIZE 512


//******************************************************************************
int start_logger (char *logger_path, FILE **log_file){
	*log_file = fopen (logger_path, "w");
	if (*log_file != NULL){
		//fprintf (*log_file,"LOR\tCRYSTAL 0\tCRYSTAL 1\n");
		return WRITER_SUCCESS;
	}
	else{
		return WRITER_ERROR;
	}
}

//******************************************************************************
void log_lor (int lor, crystal * crystal0, crystal * crystal1, FILE *log_file){
	int r1, c1, r2, c2;
	r1 = crystal0->ring;
	c1 = crystal0->num;
	r2 = crystal1->ring;
	c2 = crystal1->num;
	fwrite(&lor, sizeof(int), 1, log_file);
	fwrite(&r1, sizeof(int), 1, log_file);
	fwrite(&c1, sizeof(int), 1, log_file);
	fwrite(&r2, sizeof(int), 1, log_file);
	fwrite(&c2, sizeof(int), 1, log_file);
}

