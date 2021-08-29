all: task1 task2 task3

task1: task1.o
	gcc -m32 -g -Wall -o task1 task1.o 
	
# Depends on the source and header files
task1.o: 
	gcc -m32 -g -Wall -c -o task1.o task1.c 
	
task2: task2.o
	gcc -m32 -g -Wall -o task2 task2.o 

# Depends on the source and header files
task2.o: 
	gcc -m32 -g -Wall -c -o task2.o task2.c 

task3: task3.o
	gcc -m32 -g -Wall -o task3 task3.o 

# Depends on the source and header files
task3.o: 
	gcc -m32 -g -Wall -c -o task3.o task3.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task3