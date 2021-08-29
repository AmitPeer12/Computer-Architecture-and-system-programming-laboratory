
#include "util.h"

#define WRITE 4
#define STDOUT 1
#define READ 3
#define STDIN 0
#define OPEN 5
#define ERROR 2
#define CLOSE 6
#define O_CREATE 64
#define mode 0777
#define O_TRUNC 512
extern int system_call();

int main(int argc, char *argv[], char *envp[])
{
  char c;
  int r;
  int input = STDIN;
  int output = STDOUT;
  int inputMode = 0;
  int outputMode = 0;
  char *inputName;
  char *outputName;
  int i = 1;
  while (i < argc)
  {
    if (strncmp("-i", argv[i], 2) == 0)
    {
      inputMode = 1;
      inputName = argv[i] + 2;
      if ((input = system_call(OPEN, inputName, 0, mode)) < 0)
      {
        system_call(WRITE, STDOUT, "Error! could not open the input file\n", strlen("Error! could not open the input file\n"));
        system_call(1, 0X55, 1, 1);
      }
    }
    if (strncmp("-o", argv[i], 2) == 0)
    {
      outputMode = 1;
      outputName = argv[i] + 2;
      if ((output = system_call(OPEN, outputName, 1 | O_CREATE | O_TRUNC, mode)) < 0)
      {
        system_call(WRITE, STDOUT, "Error! could not open the output file\n", strlen("Error! could not open the output file\n"));
        system_call(1, 0X55, 11);
      }
    }
    i++;
  }
  while ((r = system_call(READ, input, &c, 1)) == 1)
  {
    if (c >= 'A' && c <= 'Z')
      c = c + ('a' - 'A');
    system_call(WRITE, output, &c, 1);
  }
  if (inputMode == 1)
    system_call(CLOSE, inputName);
  if (outputMode == 1)
    system_call(CLOSE, outputName);
  return 0;
}
