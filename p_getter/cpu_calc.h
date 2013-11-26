/*
	Algoritmos para calcular el tamaño de un segmento de recta 
	dentro de un voxel, mediante el uso de CPU
*/

//los tipos de planos:
#define Z_PLANE 0	//plano perpendicular al eje Z
#define Y_PLANE 1	//...al eje Y
#define X_PLANE 2	//...al eje X

//******************************************************************************
//obtiene la posición del centro de un voxel, a partir de los índices,
//el voxel inicial y el tamaño del voxel(delta).
vector *get_voxel(vector *position, vector *voxel0, vector *delta);

//******************************************************************************
//obtiene el punto de intersección entre un plano y una recta.
//plane_type: representa el tipo de plano (perpendicular al eje X, Y o Z)
//p_plane: representa el valor del eje perpendicular por donde pasa el plano
//origin: es el punto origen de la recta
//direction: es el vector dirección de la recta
vector *get_inter_line_plane (
				int plane_type,
				float p_plane, 
				vector *origin, 
				vector *direction);

//******************************************************************************
//devuelve el tamaño del segmento de recta contenido dentro de un voxel.
//necesita el punto de origen de la recta y su dirección.
float get_inter_line_voxel (
				vector *voxel_index,
				vector *voxel0,
				vector *delta,
				vector *origin,
				vector *direction);

//******************************************************************************
//igual que la anterior, pero recibe como entrada las posiciones de los cristales
float segment (
		vector *crystal0, 
		vector *crystal1, 
		vector *delta, 
		vector *voxel_index, 
		vector *voxel0);

