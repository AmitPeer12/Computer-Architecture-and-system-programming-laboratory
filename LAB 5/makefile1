all: task1

task1: task1.o LineParser.o
	gcc -m32 -Wall  -o myshell task1.o LineParser.o

LineParser.o : LineParser.c LineParser.h
	gcc -m32 -Wall  -c  LineParser.c -o LineParser.o	

task1.o : task1.c
	gcc -m32 -Wall -c task1.c -o task1.o

.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o myshell