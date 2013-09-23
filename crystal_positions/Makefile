CXX=gcc
CFLAGS=-Wall -W -O2 -lm
OBJ=vector.o writer.o calc.o
APP=crystal_positions

all: $(OBJ) main.o
	$(CXX) -o $(APP) main.o $(OBJ) $(CFLAGS)
	@echo "construido con éxito."

obj: $(OBJ)
	@echo "objetos compilados."

%.o: %.c %.h 
	$(CXX) -c -o $@ $< $(CFLAGS) 


clear:
	rm -frv $(APP) *.o

.PHONY: clear

