/*
 *	Funciones utilizadas para comprobar que la lista generada está ordenada 
 *	de mayor a menor.
 */

#define CHECKER_SORTED 0
#define CHECKER_ERROR -1
#define CHECKER_EOF 1

int checker_current, checker_previous;		//el valor lor actual y el anterior en la revisión
int checker_previous_crys[4];			//el valor de r1, c1, r2 y c2 del lor anterior revisado
FILE *sorted_lors_file;				//el archivo de lors que se está verificando

//******************************************************************************
//abre el archivo de lors ordenados y comprueba que estén ordenados
int open_checker(char *path);


//******************************************************************************
//almacenar el lor leído en 'checker_previous_crys'
void set_previous_crys(int r1, int c1, int r2, int c2);

//******************************************************************************
//lee el siguiente lor, desde el archivo de lors ordenados. devuelve CHECKER_SORTED si está ordenado respecto al anterior.
int check_next_lor();

//******************************************************************************
//comprueba si el archivo de salida de lors ordenados está efectivamente ordenado.
int check_all_lors();
