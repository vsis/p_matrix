CXX=gcc
CFLAGS=-Wall -W -O2  -lm -fopenmp -lpthread
OBJ=vector.o writer.o reader.o calc.o list.o crystal.o debug.o
APP=lors_getter

all: $(OBJ) main.o
	$(CXX) -o $(APP) main.o $(OBJ) $(CFLAGS)
	@echo "construido con éxito."

obj: $(OBJ)
	@echo "objetos compilados."

%.o: %.c %.h 
	$(CXX) -c -o $@ $< $(CFLAGS) 


clear:
	rm -frv $(APP) *.o
	rm -fv tester


tester: tester.c
	$(CXX) tester.c -o tester $(CFLAGS) -lcurses

.PHONY: clear

