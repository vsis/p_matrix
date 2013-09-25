#define WRITER_SUCCESS 0
#define WRITER_ERROR -1

//******************************************************************************
//abre el archivo donde se logean los resultados encontrados
int start_logger (char *logger_path, FILE **log_file);

//******************************************************************************
//escribe en el archivo de log el LOR encontrado, junto con las posiciones de sus cristales
void log_lor (int lor, crystal * crystal0, crystal * crystal1, FILE *log_file);

