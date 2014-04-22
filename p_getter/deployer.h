/*
 *	Aquí el programa se encarga de compilar y deployar
 *	los distintos scripts de opencl.
 */

#define DEPLOYER_SUCCESS 0
#define DEPLOYER_ERROR -1

/*	Aquí se definen las opciones que se pasan al compilador del dipositivo openCL
 *	Estas opciones definene macros usadas en los scripts. Estas macros son:
 *		VOXEL_INDEX_X_MAX:	La cantidad de voxeles que tiene la imágen en la dimensión X (El nombre es análogo para las dimensiones Y y Z)
 *		VOXEL_SIZE_X:		La longitud del voxel en la dimensión X (medido en milímetros. El nombre es análogo para las demás dimensiones)
 *		VOXEL_REF_X:		Posición de referencia en el eje X del primer voxel (El nombre es análogo para las demás dimensiones).
 */

//******************************************************************************
//variables usadas para computar con el dispositivo de openCL
cl_platform_id platform;
cl_device_id device;
cl_context context;
cl_program program;
cl_command_queue commands;
cl_kernel kernel_square;
cl_mem output, input_crystal0, input_crystal1;

//******************************************************************************
//variables usadas para definir la geometría del scanner y la imagen
int IMG_SIZE_X, IMG_SIZE_Y, IMG_SIZE_Z; //resolución de la imagen.
float VOXEL_SIZE_X, VOXEL_SIZE_Y, VOXEL_SIZE_Z; //tamaño del voxel.
int NUMBER_OF_LORS;	//cantidad de LORs que posee el scanner.
int NUMBER_OF_VOXELS; //cantidad de voxeles que posee la imagen

//******************************************************************************
//variables usadas como entrada y salida para alimentar los cálculos de segmentos
float crystals[6];			//[entrada] posiciones de los cristales de un lor (x1, y1, z1, x2, y2, z3)
int voxels_segments_size;	//[salida] tamaño de los los arrays voxels_indexes y voxels_segments
int *voxels_indexes;		//[salida] índices de los voxeles atravezados
float *voxels_segments;		//[salida] tamaño de los segmentos de lor dentro de los voxeles


//******************************************************************************
//inicializa el contexto para empezar a deployar los scripts
int set_device();

//******************************************************************************
//retorna un buffer que contiene la información imprimible de la plataforma y el 
//dispositivo seleccionados.
char * platform_and_device_info();

//******************************************************************************
//recibe la dirección de un archivo de script y lo compila en el dispositivo seleccionado en set_device()
cl_int deploy_script(char *path, int img_size_x, int img_size_y, int img_size_z, 
						float delta_x, float delta_y, float delta_z, float voxel0_x, 
						float voxel0_y, float voxel0_z);

//******************************************************************************
//recibe las posiciones de los dos cristales de un LOR y retorna un array que representa
//la fila LOR de la matriz P
float * get_segments(float * crystal0, float * crystal1);

//******************************************************************************
//libera los objetos usados para deployar el kernel
void release_script();

//******************************************************************************
//función de prueba
float * get_voxel_positions(int num_of_voxels);
