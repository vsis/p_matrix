#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <CL/opencl.h>

#include "crystal_reader.h"
#include "lor_reader.h"
#include "deployer.h"
#include "cl_reader.h"
#include "vector.h"
#include "p_getter.h"
#include "debug.h"

int main(){
	int now, error;
	char *crystal_path, *lor_path, *device_info, message[64];
	crystal_path = "crystals.txt";
	lor_path = "sorted_lors.bin";
	set_msg_debug("stdout");
	//iniciar dispositivo openCL
	set_device();
	device_info = platform_and_device_info();
	printf ("%s", device_info);
	error = deploy_script ("calc.cl", 2, 2, 2, 1, 1, 1, -1, -1, -1);
	if (error < CL_SUCCESS){
		error_msg ("no se cargó el kernel");
		return -1;	
	}
	//cargar en memoria las posiciones de los cristales
	info_msg("cargando cristales");
	error = open_crystal_reader(crystal_path);
	if (error == CRYSTAL_READER_ERROR){
		error_msg ("no se pudo abrir el archivo de cristales");
		return -1;
	}
	//cargar en memoria los lors
	info_msg ("cargando lors");
	error = open_lor_reader(lor_path);
	if (error == LOR_READER_ERROR){
		error_msg ("no se pudo abrir el archivo de LORs");
		return -1;
	}
	//calcular pequeña imagen
	info_msg("calulando imagen pequeña de prueba");
	now = time(NULL);
	float * pos;
	int i;
	pos = get_voxel_positions(8);
	if (pos){
		for (i=0; i<24; i += 3){
			printf("pos: %.4f %.4f %.4f\n", pos[i], pos[i+1], pos[i+2]);
		}
	}
	close_lor_reader();
	now = time(NULL) - now;
	sprintf(message, "completado en %i:%i", now/60, now%60);
	info_msg(message);
	clReleaseMemObject(output);
    clReleaseKernel(kernel_square);
    clReleaseProgram(program);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
    free(pos);
	return 0;
}
