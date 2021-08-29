all: mypipeline
	
mypipe: mypipeline.c
	gcc -g -m32 -Wall -o mypipeline mypipeline.c

.PHONY: clean
clean:
	rm -rf ./*.o mypipeline
