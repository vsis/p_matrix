#define READER_SUCCESS 0
#define READER_ERROR -1
#define READER_EOF 1
#define LINE_SIZE 512

//******************************************************************************
//abre el archivo donde se logean las posiciones de los cristales
int start_reader (char *reader_path, FILE **crys_file);

//******************************************************************************
//lee la posición de un cristal, retorna READER_EOF si llega hasta el final del archivo
int read_crys (vector **crystal, int *crys_id, int *block, int *ring, FILE *crys_file);
