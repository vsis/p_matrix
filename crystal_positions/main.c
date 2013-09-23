#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "vector.h"
#include "writer.h"
#include "calc.h"

FILE *logger;
char *output_path;

void p_help(){
	printf("UNIVERSIDAD DE SANTIAGO DE CHILE\nFACULTAD DE INGENIERÍA\n");
	printf("DEPARTAMENTO DE INGENIERÍA INFORMÁTICA\n\n");
	printf("Alumno: Roddy González\n");
	printf("Software para calcular las posiciones de cristales del escáner SIEMENS PET BIOGRAPH\n");
	printf("modo de uso:\n");
	printf("lorcalc -h\t\t\t: imprime esta ayuda\n");
	printf("lorcalc -o <archivo de salida>\t: ejecuta el cálculo y escribe el resultado en 'archivo de salida'.\n");
}
//******************************************************************************
void hecho(){
	printf("hecho\n");
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
	int help=0,output=0,error=0,opt;
	char *out_arg;
	while ((opt = getopt (argc,argv,"ho: ")) != -1){
		switch( opt ){
			case 'h':
				help = 1;
				break;
			case 'o':
				output = 1;
				out_arg = optarg;
				break;
			case '?':
				error = 1;
		}
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
	/*	iniciar loggers	*/
	printf("iniciando loggers... ");
	error = start_logger(output_path, &logger);	
	if (error == WRITER_ERROR){
		printf ("error\n");
		printf ("no se pudo abrir el archivo de salida.\n");
		return -1;
	}
	hecho();
	/*	calcular variables iniciales	*/
	printf("calculando variables iniciales... ");
	init_variables();
	hecho();
	/*	calcular posiciones de línea	*/
	printf("calculando posiciones...");
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
	hecho();
	fclose(logger);
	return 0;
}

