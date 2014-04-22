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

void lor_to_crys(lor * _lor, float * crys0, float * crys1){
	get_crystal_position(_lor->r1, _lor->c1, &(crys0[0]), &(crys0[1]), &(crys0[2]));
	get_crystal_position(_lor->r2, _lor->c2, &(crys1[0]), &(crys1[1]), &(crys1[2])); 
}

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
	error = deploy_script ("calc.cl", 2, 2, 2, 1, 1, 1, -.5, -.5, -.5);
	if (error != CL_SUCCESS){
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
	info_msg("calulando imagen pequeña de prueba (1024 LORS)");
	now = time(NULL);
	float * segs, crys0[3], crys1[3];
	int i = 0;
	get_next_lor();
	while(current_lor != NULL) {
		lor_to_crys(current_lor, crys0, crys1);
		segs = get_segments(crys0, crys1);
		if (segs != NULL){
			free(segs);
		} else {
			sprintf(message, "get_segments retornó NULL para el LOR %i", current_lor->lor_value);
			error_msg(message);
		}
		if (i%1000000 == 0){
			sprintf(message, "%i M LORs procesados", i / 1000000);
			info_msg(message);
		}
		get_next_lor();
		i++;
	}
	sprintf(message, "%i M LORs procesados. Fin del procedimiento", i / 1000000);
	info_msg(message);
	release_script();
	close_lor_reader();
	now = time(NULL) - now;
	sprintf(message, "completado en %i:%i", now/60, now%60);
	info_msg(message);
	return 0;
}
