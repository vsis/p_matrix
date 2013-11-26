/*
 *	Lee los LORs calculados, junto con los números de anillo y cristal
 *	que corresponde al par de cristales con ese LOR.
 */

#define LOR_READER_ERROR 1
#define LOR_READER_SUCCESS 0

typedef struct LOR_UNIT{
	int lor_value;
	int r1, c1, r2, c2;
} lor;

FILE * lor_input_file;			//Archivo con los lors ordenados
lor * current_lor;				//Puntero auxiliar usado para trabajar con uno de los lors leídos

int lor_value, r1, c1, r2, c2;

//******************************************************************************
//crea un nuevo objeto lor, a partir de los datos recibidos como argumento
lor * new_lor(int lor_value, int r1, int c1, int r2, int c2);

//******************************************************************************
//abre el archivo de entrada que contiene los lors calculados
int open_lor_reader(char *path);

//******************************************************************************
//obtiene un LOR y lo almacena en current_lor
int get_next_lor();

//******************************************************************************
//cierra el archivo de LORs
void close_lor_reader();

