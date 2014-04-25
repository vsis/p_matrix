#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "lor_reader.h"
#include "writer.h"
#include "checker.h"
#include "debug.h"

char input[64], output[64], output_log[64];		//direcciones de los archivos de entrada, salida y log de mensajes.

//******************************************************************************
//imprime el texto de ayuda.
void print_help(){
	printf("UNIVERSIDAD DE SANTIAGO DE CHILE\nFACULTAD DE INGENIERÍA\n");
	printf("DEPARTAMENTO DE INGENIERÍA INFORMÁTICA\n\n");
	printf("Alumno: Roddy González\n");
	printf("Software para ordenar los LORs de menor a mayor\n");
	printf("modo de uso:\n");
	printf("-h:\t\timprime esta ayuda\n");
	printf("-i <archivo de entrada>:\tlee los LORs descritos en 'archivo de entrada'.\n");
	printf("-o <archivo de salida>:\tescribe los LORs ordenados de menor a mayor en 'archivo de salida'\n");
	printf("-l <archivo de log>:\tescribe el log de mensajes y bugs en 'archivo de log'\n");
}

//******************************************************************************
//procesa los argumentos del programa
int argsm (int argc, char *argv[]){
	int help_flag = 0, error_flag = 0, c;
	int input_flag = 0, output_flag = 0, output_log_flag = 0;
	while ((c = getopt (argc, argv, "hi:o:")) != -1){
		switch (c){
			case 'h':
				help_flag = 1;
				break;
			case 'i':
				strcpy(input,optarg);
				input_flag = 1;
				break;
			case 'o':
				strcpy(output,optarg);
				output_flag = 1;
				break;
			case 'l':
				strcpy(output_log, optarg);
				output_log_flag = 1;
				break;
			case '?':
				if ( (optopt == 'i') || (optopt == 'o')){
					fprintf (stderr, "La opción -%c necesita la ruta de un archivo como argumento.\n", optopt);
				}
				else{
					fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
				}
				error_flag = 1;
				break;
		}
	}

	if(output_log_flag){
		set_msg_debug(output_log);
	} else {
		set_msg_debug("stdout");
	}

	if ( !( input_flag && output_flag ) ){
		error_flag = 1;
	}

	if ( (help_flag) || (error_flag) ){
		print_help();
	}

	return error_flag;
}

//******************************************************************************
//procedimiento principal
int main(int argc, char *argv[]){
	int now, error;
	char *sorted_path, *lor_path, message[256];
	//leer los argumentos con los que el programa fue llamado
	error = argsm(argc, argv);
	if (error){		//si los argumentos no eran correctos se termina el programa.
		return EXIT_FAILURE;
	}
	sorted_path = output;
	lor_path = input;
	//leer los lors del archivo de lors desordenado.
	info_msg("cargando lors");
	now = time(NULL);
	error = open_lor_reader(lor_path);
	if (error == LOR_READER_ERROR){
		error_msg("no se pudo abrir el archivo de entrada");
		return -1;
	}
	now = time(NULL) - now;
	sprintf (message,"completado en %i:%i", now / 60, now % 60);
	info_msg(message);
	//escribir los lors ordenados en el archivo de salida.
	info_msg ("ordenando lors");
	now = time (NULL);
	error = open_lor_writer(sorted_path);
	if (error != WRITER_SUCCESS){
		error_msg ("Error al abrir el archivo de salida");
	}else{
		sprintf (message,"iniciando escritura con %i lor_packs", number_of_packs);
		info_msg(message);
		write_all_lors();
	}
	now = time(NULL) - now;
	sprintf(message,"completado en %i:%i", now / 60, now % 60);
	info_msg(message);
	info_msg("comprobando el orden de los lors");
	now = time(NULL);
	error = open_checker(sorted_path);
	if (error == CHECKER_ERROR){
		error_msg("los LORs no están ordenados");
	}else{
		info_msg("los LORs están correctamente ordenados");
	}
	now = time(NULL) - now;
	sprintf(message,"completado en %i:%i", now / 60, now % 60);
	info_msg(message);
	return EXIT_SUCCESS;
}
