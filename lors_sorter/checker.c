#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "checker.h"
#include "debug.h"

char message[64];
//******************************************************************************
int open_checker(char *path){
	int error;
	sorted_lors_file = fopen(path, "r");
	if (sorted_lors_file == NULL){
		sprintf(message, "open_checker(): no se pudo abrir el archivo de lors ordenados");
		error_msg(message);
		return CHECKER_ERROR;
	}
	checker_current = -1;
	checker_previous = -1;
	error = check_all_lors();
	fclose(sorted_lors_file);
	return error;
}

//******************************************************************************
void set_previous_crys(int r1, int c1, int r2, int c2){
	checker_previous_crys[0] = r1;
	checker_previous_crys[1] = c1;
	checker_previous_crys[2] = r2;
	checker_previous_crys[3] = c2;
}

//******************************************************************************
int check_next_lor(){
	int current_lor_value, c1, r1, c2, r2, read_error = 0;
	//lee el siguiente lor desde el archivo de lors ordenados
	read_error = read_error || (fread (&current_lor_value, sizeof(int), 1, sorted_lors_file) == 0);
	read_error = read_error || (fread (&r1, sizeof(int), 1, sorted_lors_file) == 0);
	read_error = read_error || (fread (&c1, sizeof(int), 1, sorted_lors_file) == 0);
	read_error = read_error || (fread (&r2, sizeof(int), 1, sorted_lors_file) == 0);
	read_error = read_error || (fread (&c2, sizeof(int), 1, sorted_lors_file) == 0);
	if ( ! read_error ){
		checker_current = current_lor_value;
		if (checker_previous != -1){
			if (checker_current > checker_previous){
				set_previous_crys(r1, c1, r2, c2);
				return CHECKER_SORTED;
			}else{
				sprintf(message, "lor=%i\tr1= %i c1= %i\tr2= %i c2= %i\n", checker_current, r1, c1, r2, c2);
				info_msg(message);
				sprintf(message, "lor=%i\tr1= %i c1= %i\tr2= %i c2= %i\n", checker_previous, checker_previous_crys[0], checker_previous_crys[1], checker_previous_crys[2], checker_previous_crys[3] );
				info_msg(message);
				set_previous_crys(r1, c1, r2, c2);
				return CHECKER_ERROR;
			}
		}else{
			set_previous_crys(r1, c1, r2, c2);
			return CHECKER_SORTED;
		}
	}else{
		return CHECKER_EOF;
	}
}

//******************************************************************************
int check_all_lors(){
	int read_elements = 0, error = CHECKER_SORTED;
	while ( error == CHECKER_SORTED ){
		error = check_next_lor();
		if (error == CHECKER_SORTED){
			checker_previous = checker_current;
			read_elements++;
		}else if( error == CHECKER_EOF){
			info_msg("check_all_lors(): fin del archivo");
		}else{
			sprintf(message, "check_all_lors(): el elemento %i no está ordenado!", read_elements);
			error_msg(message);
			//return CHECKER_ERROR;
		}
	}
	sprintf (message,"%i elementos leídos y ordenados\n", read_elements);
	info_msg(message);
	return CHECKER_SORTED;
}

