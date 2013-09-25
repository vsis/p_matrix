//Definición de cristal
typedef struct CRYSTAL{
	int block;		//el bloque al que pertenece
	int num;		//el número dentro del anillo
	int ring;		//el anillo al que pertenece
	vector * position;	//la ubicación del centro de la cara frontal del cristal
} crystal;

//******************************************************************************
//crea un nuevo cristal, a partir del número de bloque, cristal, anillo y posición del mismo
crystal * new_crystal(int c, int b, int r, vector * pos);


