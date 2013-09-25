#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "lor_reader.h"
#include "writer.h"
#include "checker.h"

char input[64], output[64];		//direcciones de los archivos de entrada y salida.

//******************************************************************************
//imprime el texto de ayuda.
void print_help(){
	printf("UNIVERSIDAD DE SANTIAGO DE CHILE\nFACULTAD DE INGENIERÍA\n");
	printf("DEPARTAMENTO DE INGENIERÍA INFORMÁTICA\n\n");
	printf("Alumno: Roddy González\n");
	printf("Software para ordenar los LORs de menor a mayor\n");
	printf("modo de uso:\n");
	printf("lorsorter -h\t\t\t: imprime esta ayuda\n");
	printf("lorsorter -i <archivo de entrada> -o <archivo de salida>\t: lee los LORs descritos en 'archivo de entrada' y los escribe ordenados de menor a mayor en 'archivo de salida'.\n");
}

//******************************************************************************
//procesa los argumentos del programa
int argsm (int argc, char *argv[]){
	int help_flag = 0, error_flag = 0, c;
	int input_flag = 0, output_flag = 0;
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
	char *sorted_path, *lor_path;
	//leer los argumentos con los que el programa fue llamado
	error = argsm(argc, argv);
	if (error){		//si los argumentos no eran correctos se termina el programa.
		return EXIT_FAILURE;
	}
	sorted_path = output;
	lor_path = input;
	//leer los lors del archivo de lors desordenado.
	printf ("cargando lors...\n");
	now = time(NULL);
	error = open_lor_reader(lor_path);
	if (error == LOR_READER_ERROR){
		printf ("Error al abrir el archivo de entrada\n");
		return -1;
	}
	now = time(NULL) - now;
	printf ("completado en %i:%i\n", now / 60, now % 60);
	//escribir los lors ordenados en el archivo de salida.
	printf ("ordenando lors\n");
	now = time (NULL);
	error = open_lor_writer(sorted_path);
	if (error != WRITER_SUCCESS){
		printf ("Error al abrir el archivo de salida\n");
	}else{
		printf ("iniciando escritura con %i lor_packs\n", number_of_packs);
		write_all_lors();
	}
	now = time(NULL) - now;
	printf("completado en %i:%i\n", now / 60, now % 60);
	printf("comprobando el orden de los lors\n");
	now = time(NULL);
	error = open_checker(sorted_path);
	if (error == CHECKER_ERROR){
		printf("los lors no están ordenados\n");
	}else{
		printf("el orden de los lors ha sido verificado con éxito\n");
	}
	now = time(NULL) - now;
	printf("completado en %i:%i\n", now / 60, now % 60);
	return EXIT_SUCCESS;
}
