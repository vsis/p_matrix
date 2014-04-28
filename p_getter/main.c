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
#include "p_writer.h"
#include "debug.h"

void lor_to_crys(lor * _lor, float * crys0, float * crys1){
	get_crystal_position(_lor->r1, _lor->c1, &(crys0[0]), &(crys0[1]), &(crys0[2]));
	get_crystal_position(_lor->r2, _lor->c2, &(crys1[0]), &(crys1[1]), &(crys1[2])); 
}

int main(){
	int now, error;
	char *crystal_path, *lor_path, *device_info, message[256];
	crystal_path = "crystals.txt";
	lor_path = "sorted_lors.bin";
	set_msg_debug("stdout");
	//iniciar dispositivo openCL
	set_device();
	device_info = platform_and_device_info();
	printf ("%s", device_info);
	error = deploy_script ("calc.cl", 64, 64, 32, 0.541835, 0.541835, 0.556875, -17.0678025, -17.0678025, -17.5415625);
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
	info_msg("creando matriz P pequeña de prueba");
	now = time(NULL);
	int i = 0, total_lors = 72192;
	float * segs, crys0[3], crys1[3];
	now = time(NULL);
	error = init_p(total_lors, 64 * 64 * 32);	//se crean la matriz p resumida y vacía
												//para un total de 72192 lors y una imagen de 60x60x32
	if (error == P_WRITER_ERROR){
		error_msg("ocurrió un error al ejecutar init_p()");
		return -1;
	}
	error = open_p_file("test.pmatrix");
	if (error == P_WRITER_ERROR){
		error_msg("ocurrió un error al abrir el archivo de la matriz P");
		return -1;
	}
	get_next_lor();
	for (i=0; i<total_lors; i++){	//se calculan y escriben los lors uno por uno
		if ((current_lor != NULL) && (current_lor->lor_value == i)){
			lor_to_crys(current_lor, crys0, crys1);
			segs = get_segments(crys0, crys1);
			error = add_lor(i, segs);
			if (error == P_WRITER_ERROR){
				break;
			}
			get_next_lor();
		} else {
			error = add_lor(i, NULL);
			if (error == P_WRITER_ERROR){
				break;
			}
		}
	}
	//ahora escribimos la matriz p en un archivo
	error = write_p_matrix();
	if (error == P_WRITER_ERROR){
		error_msg("ocurrió un error al intentar escribir la matriz P");
		return -1;
	}
	info_msg("matriz P escrita");
	//cerrar archivos de entrada/salida y liberar memoria
	close_p_file();
	release_script();
	close_lor_reader();
	now = time(NULL) - now;
	sprintf(message, "completado en %i:%i", now/60, now%60);
	info_msg(message);
	return 0;
}
