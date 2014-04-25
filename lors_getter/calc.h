#define CRYSTALS_PER_BLOCK_RING 8	//número de cristales por bloque por anillo y por línea
#define BLOCKS_PER_LINE 1		//número de bloques en una línea
#define BLOCKS_PER_RING 6		//número de bloques en un anillo.
#define CRYSTALS_PER_LINE 8		//número de anillos (cristales por línea)

//los tipos de planos:
#define Z_PLANE 0	//plano perpendicular al eje Z
#define Y_PLANE 1	//...al eje Y
#define X_PLANE 2	//...al eje X
//tamaño de la imagen
#define X_SIZE 34.68
#define Y_SIZE 34.68
#define Z_SIZE 17.8

int LP;			//constante LP para el cáluclo de LORs
float p_planes[6];	//valor de los ejes cartesianos donde se cortan los planos que definene las caras de la imágen

//******************************************************************************
//calcula el índice LOR según los números de anillo, cristal y bloque
int LOR_index(int r1, int c1, int b1, int r2, int c2, int b2);

//******************************************************************************
//asigna el valor correspondiente a la variable LP
void set_LP();

//******************************************************************************
//calcula el índice LOR dados dos cristales
int LOR_crystal(crystal *a, crystal *b);


//******************************************************************************
//obtiene el punto de intersección entre un plano y una recta
//plane_type: representa el tipo de plano (perpendicular al eje X, Y o Z)
//p_plane: representa el valor del eje perpendicular por donde pasa el plano
//origin: es el punto origen de la recta
//direction: es el vector dirección de la recta
vector *get_intersection_line_plane (
				int plane_type,
				float p_plane, 
				vector *origin, 
				vector *direction);


//******************************************************************************
//determina si la recta entre dos cristales pasa por la imagen.
int get_intersection_line_image (crystal *a, crystal *b);

//******************************************************************************
//determina si un punto está dentro de la imágen
int inside (vector *dot);

//******************************************************************************
//asigna el valor donde los planos de la imágen cortan los ejes cartesianos
void set_planes();

