CXX=gcc
CFLAGS=-Wall -W -O2 -lm -lOpenCL -fopenmp
OBJ=vector.o crystal_reader.o lor_reader.o deployer.o cl_reader.o debug.o cpu_calc.o p_getter.o p_writer.o
APP=p_getter

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
