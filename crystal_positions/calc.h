#define CRYSTALS_PER_BLOCK_RING 13	//número de cristales por bloque por anillo y por línea
#define BLOCKS_PER_LINE 3		//número de bloques en una línea
#define BLOCKS_PER_RING 48		//número de bloques en un anillo.
#define CRYSTALS_PER_LINE 39		//número de anillos (cristales por línea)

//******************************************************************************
//Tipo de dato que representa un cristal del anillo
typedef struct CRYSTAL{
	vector *position;	//la posición del cristal
	int block;		//el número de bloque al que pertenece
	int num;		//el número de cristal
	int ring;		//el anillo del cristal
} crystal;

//******************************************************************************
float angle_per_block;		//ángulo que se forma en el centro del sistema entre las rectas que pasan por el centro de los bloques
float radius;			//distancia entre el centro del sistema y el centro del bloque
float inter_crystal;		//distancia entre cada cristal del bloque
float inter_crystal_half;	//la mitad de la distancia entre cristales
float crystal_width;		//ancho del cristal
float crystal_width_half;	//la mitad del ancho del cristal
float width_half;		//la mitad del ancho del cristal mas el espacio inter-cristal
float block_size;		//el ancho del bloque
float block_size_half;		//la mitad del ancho del bloque
float inter_block;		//distancia entre dos bloques en una misma línea.
float initial_z_axis;		//valor de la componente z del primer anillo

//******************************************************************************
//inicializa las variables globales
void init_variables();

//******************************************************************************
//inicializa las variables globales
crystal * create_crystal(int _num, int _block, int ring, vector *_position);

//******************************************************************************
//retorna el centro de un bloque, dado el número de bloque
vector * calc_block(int block_num);
 
//******************************************************************************
//retorna una array de los cuatro critales del anillo 0 dado un bloque y su posición
crystal **calc_crystals (int block_num, int crystal_num, vector *block_pos);

//******************************************************************************
//Dado un crital en una posición del anillo inicial, calcula la posición de los 
//demás cristales que tengan el mIsmo id pero en distintos anillos.
crystal ** crystal_line (crystal *initial, int block_num, int crystal_num);

