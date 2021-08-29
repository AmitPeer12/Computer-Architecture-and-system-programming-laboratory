all: encoder

encoder: encoder.c
		gcc -g -m32 -Wall -o encoder encoder.c

.PHONY: clean
clean:
	rm -rf ./*.o main