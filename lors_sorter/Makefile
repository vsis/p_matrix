CXX=gcc
CFLAGS=-Wall -W -O2 -lm
OBJ=writer.o lor_reader.o checker.o debug.o
APP=lors_sorter

all: $(OBJ) main.o
	$(CXX) -o $(APP) main.o $(OBJ) $(CFLAGS)
	@echo "construido con éxito."

obj: $(OBJ)
	@echo "objetos compilados."

%.o: %.c %.h 
	$(CXX) -c -o $@ $< $(CFLAGS) 

clear:
	rm -frv $(APP) *.o tester

tester: tester.c
	$(CXX) tester.c -o tester $(CFLAGS) -lcurses

.PHONY: clear
