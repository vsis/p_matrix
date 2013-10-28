#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cl_reader.h"

//******************************************************************************
read_kernel * read_kernel_from_file(char * path){
	int file_size, error;
	FILE * kernel_file;
	read_kernel * new_kernel;
	kernel_file = fopen( path, "r");
	if ( kernel_file == NULL ){
		printf ("DEBUG\tload_kernel_from_file(path = %s): ocurrió un error al abrir el archivo\n", path );
		return NULL;
	}
	//determinar el largo del archivo
	fseek( kernel_file, 0, SEEK_END );
	file_size = ftell( kernel_file );
	rewind( kernel_file );
	//crear read_kernel y su buffer
	new_kernel = (read_kernel *) malloc ( sizeof( read_kernel ) );
	if ( new_kernel == NULL){
		printf ("DEBUG\tload_kernel_from_file(path = %s): malloc devolvió NULL para new_kernel\n", path);
		return NULL;
	}
	new_kernel->script = (char *) malloc ( sizeof (char) * (file_size + 1) );
	if ( new_kernel->script == NULL ){
		printf ("DEBUG\tload_kernel_from_file(path = %s): malloc devolvió NULL para new_kernel->script\n", path);
		return NULL;
	}
	new_kernel->size = file_size;
	new_kernel->script[file_size] = '\0';
	//lee el archivo y lo guarda en el buffer de script
	error = ( fread( new_kernel->script, sizeof(char), file_size, kernel_file ) < (unsigned int) file_size );
	if ( error ){
		printf("DEBUG\tload_kernel_from_file(path = %s): ocurrió un error al leer el archivo de script\n", path);
		return NULL;
	}
	fclose( kernel_file );
	return new_kernel;
}

