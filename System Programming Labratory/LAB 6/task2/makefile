all: task2

task2: task2.o LineParser.o
	gcc -m32 -Wall  -o myshell task2.o LineParser.o

LineParser.o : LineParser.c LineParser.h
	gcc -m32 -Wall  -c  LineParser.c -o LineParser.o	

task2.o : task2.c
	gcc -m32 -Wall -c task2.c -o task2.o

.PHONY: clean


clean: 
	rm -f *.o myshell