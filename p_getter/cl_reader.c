#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cl_reader.h"
#include "debug.h"

char message[64];

//******************************************************************************
read_kernel * read_kernel_from_file(char * path){
	int file_size, error;
	FILE * kernel_file;
	read_kernel * new_kernel;
	kernel_file = fopen( path, "r");
	if ( kernel_file == NULL ){
		sprintf (message, "load_kernel_from_file(path = %s): ocurrió un error al abrir el archivo", path );
		error_msg(message);
		return NULL;
	}
	//determinar el largo del archivo
	fseek( kernel_file, 0, SEEK_END );
	file_size = ftell( kernel_file );
	rewind( kernel_file );
	//crear read_kernel y su buffer
	new_kernel = (read_kernel *) malloc ( sizeof( read_kernel ) );
	if ( new_kernel == NULL){
		sprintf (message, "load_kernel_from_file(path = %s): malloc devolvió NULL para new_kernel", path);
		error_msg(message);
		return NULL;
	}
	new_kernel->script = (char *) malloc ( sizeof (char) * (file_size + 1) );
	if ( new_kernel->script == NULL ){
		sprintf (message, "load_kernel_from_file(path = %s): malloc devolvió NULL para new_kernel->script", path);
		error_msg(message);
		return NULL;
	}
	new_kernel->size = file_size;
	new_kernel->script[file_size] = '\0';
	//lee el archivo y lo guarda en el buffer de script
	error = ( fread( new_kernel->script, sizeof(char), file_size, kernel_file ) < (unsigned int) file_size );
	if ( error ){
		sprintf(message, "load_kernel_from_file(path = %s): ocurrió un error al leer el archivo de script", path);
		error_msg(message);
		return NULL;
	}
	fclose( kernel_file );
	return new_kernel;
}

