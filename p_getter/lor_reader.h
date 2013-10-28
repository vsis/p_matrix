/*
 *	Lee los LORs calculados, junto con los números de anillo y cristal
 *	que corresponde al par de cristales con ese LOR.
 */

#define NUMBER_OF_VALID_LORS 108985597	//número estimado de LORS válidos
#define LOR_READER_ERROR 1
#define LOR_READER_SUCCESS 0
#define NUMBER_OF_LORS_PER_PACK 102400

typedef struct LOR_UNIT{
	int lor_value;
	int r1, c1, r2, c2;
} lor;

FILE * lor_input_file;			//Archivo con los lors ordenados
lor *** lor_packs;			//Array de arrays de lors. Aquí están todos los lors almacenados
lor ** current_lor_pack;		//Array de lors, antes de ser asignado al array de arrays
lor * current_lor;			//Puntero auxiliar usado para trabajar con uno de los lors leídos
int number_of_packs;			//número de arrays que contiene 'lor_packs'
int *pack_min, *pack_max;		//Índices lor menores y mayores de 'lor_packs'

int lor_value, r1, c1, r2, c2;

//******************************************************************************
//crea un nuevo objeto lor, a partir de los datos recibidos como argumento
lor * new_lor(int lor_value, int r1, int c1, int r2, int c2);

//******************************************************************************
//abre el archivo de entrada que contiene los lors calculados
int open_lor_reader(char *path);

//******************************************************************************
//obtiene un array de lors que contiene NUMBER_OF_LORS_PER_PACK elementos
int get_next_lor_pack();

//******************************************************************************
//obtiene todos los lor_packs
int get_all_lor_packs();

//******************************************************************************
//dado un índice, busca un lor que tenga ese índice.
//retorna NULL si no encuentra el lor.
lor * search(int index);

//******************************************************************************
//hace búsqueda binaria en 'curren_pack'
lor * binary_search(int index, int begin, int end);

