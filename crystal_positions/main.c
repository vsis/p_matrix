#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "vector.h"
#include "writer.h"
#include "calc.h"
#include "debug.h"

FILE *logger;
char *output_path, *debug_log_path;

void p_help(){
	printf("UNIVERSIDAD DE SANTIAGO DE CHILE\nFACULTAD DE INGENIERÍA\n");
	printf("DEPARTAMENTO DE INGENIERÍA INFORMÁTICA\n\n");
	printf("Alumno: Roddy González\n");
	printf("Software para calcular las posiciones de cristales del escáner SIEMENS PET BIOGRAPH\n");
	printf("modo de uso:\n");
	printf("-h\t\t\t: imprime esta ayuda\n");
	printf("-o <archivo de salida>\t: ejecuta el cálculo y escribe el resultado en 'archivo de salida'.\n");
	printf("-l <archivo de log>\t: define un stream de salida para los logs del programa. (por defecto: stdout)\n");
}

//******************************************************************************
//liberar la memoria de un cristal
void free_crys (crystal *crys){
	if (crys != NULL){
		free (crys->position);
		free (crys);
	}
}


//******************************************************************************
//loggea un cristal
void log_crys (crystal *c, FILE *log){
	log_crystal (c->ring, c->block, c->num, c->position, log);
}

//******************************************************************************
//lee los parámetros enviados al programa y retorna la acción a ejecutar.
int args (int argc, char *argv[]){
	int help=0,output=0,error=0,debug=0,opt;
	char *out_arg, *log_arg;
	while ((opt = getopt (argc,argv,"ho:l:")) != -1){
		switch( opt ){
			case 'h':
				help = 1;
				break;
			case 'o':
				output = 1;
				out_arg = optarg;
				break;
			case 'l':
				debug = 1;
				log_arg = optarg;
				break;
			case '?':
				error = 1;
		}
	}
	if (debug){
		debug_log_path = log_arg;
	} else {
		debug_log_path = NULL;
	}
	if (error || help){
		return 1;
	}
	else if(output){
		output_path = out_arg;
		return 0;
	}

	return 1;
}

//******************************************************************************
int main (int argc, char *argv[]){
	int error, j, current_ring, current_block=0, current_crystal=0;
	vector *current;
	/*	leer argumentos de entrada */
	error = args (argc, argv);
	if (error){
		p_help();
		return 0;
	}
	/*	se abre el stream de salida de mensajes de log y depuración */
	set_msg_debug(debug_log_path);
	/*	iniciar loggers	*/
	info_msg("Abriendo el archivo de salida");
	error = start_logger(output_path, &logger);	
	if (error == WRITER_ERROR){
		error_msg("No se pudo abrir el archivo de salida");
		return -1;
	}
	/*	calcular variables iniciales	*/
	info_msg("Calculando varialbes iniciales");
	init_variables();
	/*	calcular posiciones de línea	*/
	info_msg("Calculando posiciones");
	for (current_block = 0; current_block< BLOCKS_PER_RING; current_block++){		//para cada bloque...
		crystal **block_crystals, **line_crystal;
		current = calc_block (current_block);						//... se calcula la posición del bloque actual...
		block_crystals = calc_crystals(current_block, current_crystal, current);	//... y después se calculan los cristales de ese bloque (pero sólo la posición x,y; z se calcula cuando se calcula el anillo).
		if (block_crystals != NULL){
			for (j=0 ;j< CRYSTALS_PER_BLOCK_RING; j++){						//para cada cristal del bloque actual...
				line_crystal = crystal_line(block_crystals[j], current_block, current_crystal);	//...se calculan las posiciones del mísmo cristal en los distintos anillos (las componentes z que faltaban).
				if (line_crystal != NULL){
					for (current_ring= 0; current_ring< CRYSTALS_PER_LINE; current_ring++){	//para cada cristal calculado...
						log_crys(line_crystal[current_ring], logger );			//... se loggea en el archivo de salida.
						free_crys( line_crystal[current_ring] );
					}
					free( line_crystal );
				}
				free_crys( block_crystals[j] );
				current_crystal++;
			}
			free(block_crystals);
		}		
	}
	fclose(logger);
	info_msg("Cálculo terminado. Cerrando aplicación");
	return 0;
}

