/*
 *	Lee las posiciones de los cristales,
 *	junto a sus datos (anillo y posición dentro del anillo)
 */

#define CRYSTAL_READER_SUCCESS 0
#define CRYSTAL_READER_ERROR 1
#define NUMBER_OF_ELEMENTS 384
#define NUMBER_OF_RINGS 8

FILE *crystal_input_file;		//descriptor del archivo que contiene las posiciones de los cristales
float * positions_x;		//todas las posiciones (eje x)
float * positions_y;		//todas las posiciones (eje y)
float * positions_z;		//todas las posiciones (eje z)

//******************************************************************************
//abre el archivo de entrada y asigna memoria para los arrays de posiciones
int open_crystal_reader(char * path);

//******************************************************************************
//llena los arrays con las posiciones almacenadas en el archivo de entrada
int get_crystal_elements();


//******************************************************************************
//obtiene el índice donde deben ir las posiciones a partir del los números de cristal y anillo
int get_crystal_index(int ring, int crys);

//******************************************************************************
//asigna a las variables x0, y0 y z0 los valores de posición de cristal, según el anillo y número de cristal
void get_crystal_position (int ring, int crys, float *x0, float *y0, float *z0);

