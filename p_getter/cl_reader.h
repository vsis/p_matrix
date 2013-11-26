/*
 *	Aquí se define la estructura que ocupará un kernel leído desde un 
 *	archivo fuente.
 */

typedef struct READ_KERNEL {
	size_t size;
	char * script;
} read_kernel;

//******************************************************************************
//dada la dirección de un kernel, lee el script y lo deja en un read_kernel
read_kernel * read_kernel_from_file(char * path);

