/*
 *	
 */

//argumentos para la función set_p_method
#define P_CPU 0			//el cálculo de la matriz se hace por CPU
#define P_GPU 1			//el cálculo de la matriz se hace en openCL por GPU
#define P_NO_METHOD -1 	//no hay un método definido

//geometría de la imagen
int num_x_voxels, num_y_voxels, num_z_voxels;	//dimensiones de la imagen
float voxel0_x, voxel0_y, voxel0_z;				//posición del voxel 0
float delta_x, delta_y, delta_z;				//tamaño del voxel
vector *voxel0, *delta;

int p_method;				//método para calcular la matriz p
int num_of_voxels;			//número de voxeles

//******************************************************************************
//define el tamaño de la imagen en voxeles
void set_img_size(int x, int y, int z);

//******************************************************************************
//define el tamaño del voxel
void set_voxel_size(float x, float y, float z);

//******************************************************************************
//define el centro geométrico del voxel 0,0,0
void set_voxel0_position(float x, float y, float z);

//******************************************************************************
//define la forma de calcular la matriz p
void set_p_method(int method);

//******************************************************************************
//retorna una fila de la matriz p por el método CPU
float * get_p_row_cpu();

//******************************************************************************
//retorna una fila de la matriz p por el método seleccionado con set_p_method
float * get_p_row();


