#define OUTPUT "pos_1_ring.txt"
#define WRITER_SUCCESS 0
#define WRITER_ERROR -1

//******************************************************************************
//abre el archivo donde se logean los resultados encontrados
int start_logger (char *logger_path, FILE **log_file);

//******************************************************************************
//escribe en el archivo de log el número de bloque, número de cristal y posición del centro del cristal
void log_crystal(int ring_num, int block_num, int crystal_num, vector *crystal_position, FILE *log_file);

