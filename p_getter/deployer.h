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
#define COMPILATOR_OPTS "-DVOXEL_INDEX_X_MAX=0 -DVOXEL_INDEX_Y_MAX=0 -DVOXEL_INDEX_Z_MAX=0 -DVOXEL_SIZE_X=0 -DVOXEL_SIZE_Y=0 -DVOXEL_SIZE_Z=0 -DVOXEL_REF_X=0 -DVOXEL_REF_Y=0 -DVOXEL_REF_Z=0"

//******************************************************************************
//variables usadas para computar con el dispositivo de openCL
cl_platform_id platform;
cl_device_id device;
cl_context context;
cl_program program;
cl_kernel kernel_square;

//******************************************************************************
//variables usadas como entrada y salida para alimentar los cálculos de segmentos
float crystals[6];		//[entrada] posiciones de los cristales de un lor (x1, y1, z1, x2, y2, z3)
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
int deploy_script(char *path);

//******************************************************************************
//inicializa los arrays usados para obtener los voxeles atravezados por un lor y
//sus respectivos tamaños de segmento.
int set_input_output();
