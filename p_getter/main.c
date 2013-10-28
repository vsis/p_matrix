#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CL/opencl.h>

#include "crystal_reader.h"
#include "lor_reader.h"
#include "deployer.h"
#include "cl_reader.h"

int main(){
	int now, error;
	char *crystal_path, *lor_path, *device_info;
	crystal_path = "crystals.txt";
	lor_path = "salida.bin";
	
	set_device();
	device_info = platform_and_device_info();
	printf ("%s", device_info);
	error = deploy_script ("calc.cl");
	if (error < CL_SUCCESS){
		printf ("no se cargó el kernel\n");
	}
	return EXIT_SUCCESS;
	/*
	printf("cargando cristales...\n");
	now = time(NULL);
	error = open_crystal_reader(crystal_path);
	if (error == CRYSTAL_READER_ERROR){
		printf ("Error al abrir el archivo de cristales\n");
		return -1;
	}
	now = time(NULL) - now;
	printf ("completado en %i:%i\n", now / 60, now % 60);
	printf ("cargando lors...\n");
	now = time(NULL);
	error = open_lor_reader(lor_path);
	if (error == LOR_READER_ERROR){
		printf ("Error al abrir el archivo de lors\n");
		return -1;
	}
	now = time(NULL) - now;
	printf ("completado en %i:%i\n", now / 60, now % 60);
	*/
	return 0;
}
