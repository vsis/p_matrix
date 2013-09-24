#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "debug.h"

//******************************************************************************
void set_msg_debug(char * path){
	if ( (path != NULL) && (strcmp(path, "stdout") != 0) ){
		debug_output = fopen (path,"a");
		if (debug_output == NULL){
			debug_output	 = stdout;
			error_msg("Imposible abrir el archivo de log y depuración. 'stdout' será ocupado en su lugar");
		}
	} else {
		debug_output = stdout;
	}
}

//******************************************************************************
void debug_msg(char * debug_type, char * msg){
	char d_time[DEBUG_TIME_SIZE];
	time_t now = time(NULL);		//se obtiene el momento en que 
						//	se imprime el mensaje
	strftime ( d_time, DEBUG_TIME_SIZE, "%d/%m/%y - %H:%M:%S", localtime( & now ) );
	fprintf(debug_output, "%s [%s]\t%s\n", d_time,debug_type,msg);
}

//******************************************************************************
void warning_msg(char * msg){
	debug_msg("WARNING", msg);
}

//******************************************************************************
void error_msg(char * msg){
	debug_msg("ERROR", msg);
}

//******************************************************************************
void info_msg(char * msg){
	debug_msg("INFO", msg);
}

