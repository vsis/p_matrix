#define P_WRITER_SUCCESS 0
#define P_WRITER_ERROR -1

int number_of_total_lors, number_of_voxels, number_of_nonzero_elems;
int *a2r, *a2b, max_a2b;
float *a2p;
FILE * P_FILE;

//**********************************************************************
//inicializa los arrays que definen la matriz P
int init_p(int max_lors, int num_of_voxels);

//**********************************************************************
//abre el archivo donde se va a escribir la matriz P
int open_p_file(char * path);

//**********************************************************************
//añade las intersecciones de un LOR en la matriz P resumida
int add_lor(int lor_index, float * segments);

//**********************************************************************
//escribe en el archivo de la matriz P los arrays a2r, a2b y a2p
int write_p_matrix();

//**********************************************************************
//cierra el archivo de la matriz P
void close_p_file();
