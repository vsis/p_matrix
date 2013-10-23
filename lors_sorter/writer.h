/*
 *	Busca un LOR en los arrays de lors
 *	y escribe el archivo de salida, con los LORS ordenados
 */

#define WRITER_SUCCESS 0
#define WRITER_ERROR -1
#define NUMBER_OF_MAX_LORS 296108280

FILE *writer_output;	//descriptor del archivo de salida

//******************************************************************************
//determina en qué pack se encuentra el lor menor, deja ese lor en current_lor y devuleve el índice del pack
int get_minor();

//******************************************************************************
//abre el archivo de salida, donde se escribirán los lors ordenados
int open_lor_writer(char *path);

//******************************************************************************
//escribe en el archivo de salida el 'current_lor'
void write_current_lor();

//******************************************************************************
//escribe todos los lor ordenados de menor a mayor
int write_all_lors();
