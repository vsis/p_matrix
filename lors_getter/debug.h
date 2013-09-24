/*
 *	Este archivo contiene las definiciones de las instrucciones
 *	usadas para crear logs útiles para la depuración
 */

//la cantidad máxima de caracteres que tendrá un mensaje de debug
#define DEBUG_TIME_SIZE 24

//El stream donde se imprimirán los mensajes de debug
FILE * debug_output;

//define la ruta donde se imprimirán los mensajes
//si el string de entrada es igual al "stdout", los mensajes se imprimen en
//pantalla
void set_msg_debug(char * path);


//******************************************************************************
//imprime un mensaje general de depuración
//las demás funciones de mensajes son casos particulares de ésta
void debug_msg(char * debug_type, char * msg);

//******************************************************************************
//imprime un mensaje de advertencia
void warning_msg(char * msg);

//******************************************************************************
//imprime un mensaje de error
void error_msg(char * msg);

//******************************************************************************
//imprime un mensaje de información
void info_msg(char * msg);

