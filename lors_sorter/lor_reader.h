/*
 *	Lee los LORs calculados, junto con los números de anillo y cristal
 *	que corresponde al par de cristales con ese LOR.
 */

#define NUMBER_OF_VALID_LORS 108985597	//número estimado de LORS válidos
#define LOR_READER_ERROR 1
#define LOR_READER_SUCCESS 0
#define NUMBER_OF_LORS_PER_PACK 1000000

typedef struct LOR_UNIT{
	int lor_value;
	int r1, c1, r2, c2;
} lor;

FILE * lor_input_file;
lor *** lor_packs;
lor ** current_lor_pack;
lor * current_lor;
int number_of_packs;

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
//Ordena de menor a mayor el current_lor_pack, según el valor dado en lor_value
void sort_lor_pack(int begin, int end);

//******************************************************************************
//intercambia de posición dos lors del current_lor_pack
void swap_lor_in_current_pack(int i, int j);
