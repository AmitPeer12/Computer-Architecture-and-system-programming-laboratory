#include <stdio.h>
#define MAX_LEN 100			/* maximal input string size */

extern void assFunc(int);

char c_checkValidity(int x) {
  return (x % 2 == 0) ? '1' : '0';
}

int main(int argc, char** argv) {

  char input[12];
  int choice = atoi(fgets(input, 12, stdin));
  assFunc(choice);
  return 0;
  
}