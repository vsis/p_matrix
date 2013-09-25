#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <omp.h>

#include "vector.h"
#include "crystal.h"
#include "calc.h"
#include "list.h"
#include "reader.h"
#include "writer.h"
#include "debug.h"


#define LORS_PER_THREAD 512
list *comb_list;
char *input, *output;
int number_of_threads = 0;

//******************************************************************************
//imprime el texto de ayuda.
void print_help(){
	printf("UNIVERSIDAD DE SANTIAGO DE CHILE\nFACULTAD DE INGENIERÍA\n");
	printf("DEPARTAMENTO DE INGENIERÍA INFORMÁTICA\n\n");
	printf("Alumno: Roddy González\n");
	printf("Software para calcular los LORs para el escáner PET SIEMENS BIOGRAPH\n");
	printf("modo de uso:\n");
	printf("-h\t\t\t:imprime esta ayuda\n");
	printf("-i <archivo de entrada>\t:lee las posiciones de cristales descritas en 'archivo de entrada'\n");
	printf("-o <archivo de salida>\t:escribe los lors encontrados en 'archivo de salida'\n");
	printf("-n <número de hilos>\t:determina el número de hilos para calcular\n");
	printf("-l <log>\t\t:determina un archivo donde escribir el log del programa (por defecto: stdout)\n");
}

//******************************************************************************
//añade los cristales a la lista.
void adder(FILE *reader){
	int status;
	int block, ring, num;
	vector *current_pos;
	crystal *current_crys;
	comb_list = new_list();
	while ( (status = read_crys(&current_pos, &num, &block, &ring, reader) ) != READER_EOF ){
		current_crys = new_crystal(num, block, ring, current_pos);
		add_element( current_crys, comb_list );
	}
}

//******************************************************************************
//obtiene un array de los cristales siguientes
crystal ** next_elements(int number_of_elements){
	int i;	
	crystal ** current_crystals;
	current_crystals = (crystal **) calloc(number_of_elements, sizeof(crystal));
	for (i=0; i < number_of_elements; i++){
		current_crystals[i] = get_next_j_element( comb_list );
	}
	return current_crystals;
}

//******************************************************************************
//realiza las combinaciones de cristales, calcula los lor y los escribe al archivo de salida
void comb_writer( FILE *writer, int *valid){
	int valid_lors = 0;
	crystal *current_i;
	while ( ( current_i = get_next_i_element( comb_list ) ) != NULL ){
		set_ij( comb_list );		
		#pragma omp parallel
		{
			int index, *current_lors, j_ended_flag = 0;
			crystal ** current_js;
			current_lors = (int *) calloc(LORS_PER_THREAD, sizeof(int));
			
			while (j_ended_flag == 0){
				#pragma omp critical
				{
					current_js = next_elements(LORS_PER_THREAD);
				}
				for (index = 0; index < LORS_PER_THREAD; index++){
					if (current_js[index] != NULL){
						current_lors[index] = LOR_crystal (current_i, current_js[index]);
					}else{
						j_ended_flag = 1;
						current_lors[index] = -1;
					}	
				}	
	
				#pragma omp critical
				for (index = 0; index < LORS_PER_THREAD; index++){
					if (current_lors[index] != -1){
						valid_lors++;
						log_lor ( current_lors[index], current_i, current_js[index], writer );
					}
				}
				free(current_js);
			}

			free(current_lors);
		}
		
	}
	*valid = valid_lors;
}


//******************************************************************************
//procesa los argumentos del programa
int argsm (int argc, char *argv[]){
	int help_flag = 0, error_flag = 0, log_output_flag = 0;
	int c; 
	char num_threads[10], *log_output;
	opterr = 0;
	while ((c = getopt (argc, argv, "hi:o:n:l:")) != -1){
		switch (c){
			case 'h':
				help_flag = 1;
				break;
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 'n':
				strcpy (num_threads, optarg);
				number_of_threads = atoi (num_threads);
				break;
			case 'l':
				log_output = optarg;
				log_output_flag = 1;
				break;
			case '?':
				if ( (optopt == 'c') || (optopt == 'i'))
					fprintf (stderr, "La opción -%c necesita la ruta de un archivo como argumento.\n", optopt);
				else if (optopt == 'n')
					printf ("La opción -%c necesita un número como argumento.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
				else
					fprintf (stderr, "caracter de opción desconocido `\\x%x'.\n", optopt);
				error_flag = 1;
				break;
		}
	}
	if (number_of_threads <= 0){
		error_flag = 1;
		printf ("Error: no se especificó el número de hilos.\n\n");
	}
	if ( (help_flag) || (error_flag) ){
		print_help();
	}
	if (log_output_flag){
		set_msg_debug(log_output);
	} else {
		set_msg_debug("stdout");
	}
	return error_flag;
}

//******************************************************************************
int main (int argc, char *argv[]){
	FILE *reader_file, *writer_file;
	int error, begin, end, valid_lors;
	char message[64];
	
	error = argsm(argc, argv);
	if (error){
		return -1;
	}
	error = start_reader(input, &reader_file);
	if (error == READER_SUCCESS){
		error = start_logger(output, &writer_file);
		if (error == WRITER_SUCCESS){
			begin = time(NULL);	//medit el tiempo de inicio del cálculo
			omp_set_num_threads(number_of_threads);	//setear el número de hilos
			set_planes();	//se determina el tamaño de la imágen
			set_LP();	//se determina el valor de la constante LP
			info_msg ("Leyendo las posiciones de los cristales");
			adder(reader_file);	//se agregan los cristales a la lista
			sprintf (message, "Calculando los LORs con %i hilos de ejecución", number_of_threads); 
			info_msg (message);
			comb_writer(writer_file, &valid_lors);	//se hacen las combinaciones de cristales y se escriben los lors
			end = time(NULL);
			end = end - begin;	//se determina el tiempo que tomó el cálculo
			sprintf( message, "Cálculo terminado en %i:%i minutos", end / 60, end % 60 );
			info_msg(message);
			sprintf(message, "LORS válidos calculados= %i",valid_lors);
			info_msg(message);			
			fclose(writer_file);
		}else{
			error_msg("No se pudo abrir el archivo de salida");
		}
		fclose(reader_file);	
	}else{
		error_msg ("No se pudo abrir el archivo de entrada");
	}
	return 0;
}

