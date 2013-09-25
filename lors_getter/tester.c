#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>


int exitKey(){
	char pressed;
	pressed = getch();
	return (pressed == 'q') || (pressed == 'Q');
}


void main (){
	int lor, r1, c1, r2, c2;
	FILE *input_file;
	input_file = fopen ("salida.bin", "r");
	if (input_file == NULL){
		printf ("Error al abrir el archivo\n");
		return;
	}
	initscr();
	timeout(-1);
	while ( 1 ){
		fread (&lor, sizeof(int), 1, input_file);
		fread (&r1, sizeof(int), 1, input_file);
		fread (&c1, sizeof(int), 1, input_file);
		fread (&r2, sizeof(int), 1, input_file);
		fread (&c2, sizeof(int), 1, input_file);
		printf ("%i\t%i %i %i %i\n\r", lor, r1, c1, r2, c2);
		if ( exitKey() ){
			break;
		}
	}
	endwin();
}


