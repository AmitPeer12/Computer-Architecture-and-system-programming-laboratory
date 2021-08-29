all: task3

task3: task3.o LineParser.o
	gcc -m32 -Wall  -o myshell task3.o LineParser.o

LineParser.o : LineParser.c LineParser.h
	gcc -m32 -Wall  -c  LineParser.c -o LineParser.o	

task3.o : task3.c
	gcc -m32 -Wall -c task3.c -o task3.o

.PHONY: clean


clean: 
	rm -f *.o myshell