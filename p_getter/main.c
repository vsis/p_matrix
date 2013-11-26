#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CL/opencl.h>

#include "crystal_reader.h"
#include "lor_reader.h"
#include "deployer.h"
#include "cl_reader.h"
#include "vector.h"
#include "cpu_calc.h"
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
	error = deploy_script ("calc.cl");
	if (error < CL_SUCCESS){
		error_msg ("no se cargó el kernel");
		return -1;	
	}
	//cargar en memoria las posiciones de los cristales
	info_msg("cargando cristales");
	now = time(NULL);
	error = open_crystal_reader(crystal_path);
	if (error == CRYSTAL_READER_ERROR){
		error_msg ("no se pudo abrir el archivo de cristales");
		return -1;
	}
	now = time(NULL) - now;
	sprintf (message, "completado en %i:%i", now / 60, now % 60);
	info_msg(message);
	//cargar en memoria los lors
	info_msg ("cargando lors");
	now = time(NULL);
	error = open_lor_reader(lor_path);
	if (error == LOR_READER_ERROR){
		error_msg ("no se pudo abrir el archivo de LORs");
		return -1;
	}
	now = time(NULL) - now;
	sprintf (message, "completado en %i:%i", now / 60, now % 60);
	info_msg(message);
	//calcular pequeña imagen
	info_msg("calulando imagen pequeña de prueba");
	vector *voxel0, *delta, *index = NULL, *origin=NULL, *destiny=NULL;
	int i,j,k, lor_error;
	float seg, x, y, z;
	voxel0 = new_vector(-1,-1,-1);
	delta = new_vector (1,1,1);
	lor_error = get_next_lor();
	while ( ! lor_error ){
		for (i=0; i<2; i++){
			for (j=0; j<2; j++){
				for(k=0; k<2; k++){
					free(index);
					free(origin);
					free(destiny);
					if (lor_error){
						error_msg("no se pudo cargar un LOR");
						return -1;
					}
					index = new_vector(i,j,k);
					get_crystal_position(current_lor->r1,current_lor->c1, &x, &y, &z);
					origin = new_vector(x, y, z);
					get_crystal_position(current_lor->r2,current_lor->c2, &x, &y, &z);
					destiny = new_vector(x, y, z);
					seg = segment(origin, destiny, delta, index, voxel0);
					if (seg != 0){
						sprintf(message, "índice %i,%i,%i\tLOR %i, seg %.4f", i, j, k, current_lor->lor_value, seg);
						info_msg(message);
					}
				}
			}
		}
		lor_error = get_next_lor();
	}
	close_lor_reader();
	info_msg("fin del cálculo");
	return 0;
}
