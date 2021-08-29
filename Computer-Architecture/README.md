# Computer-Architecture-and-system-programming-laboratory
Assignment0:


The assignment is consist of 2 programs:
1) A function written in C and reads an input line from a user into a null-terminated string.
Then, this program calls the second program.
2) A function written in assembly language that receives a pointer to a null terminated character string as an argument and operate on the string as follows:
- Convert upper-case letters to lower-case.
- Count (and return) the number of spaces.

The function returns the counter value.
The characters conversion is in-place, which means we change the original input string to get the result string.


Assignment1:

This assignment contains two tasks:

1) C calling convention:
- C program that Prompts for and reads one integer (32 bits) number x in decimal base from the user.

- Calls 'void assFunc(int x)' written in assembly language with the above integer as an argument that performs the following steps:

  1)Calls 'char c_checkValidity(int x)' written in C to check the number x 
  
  2)If c_checkValidity(int x) returns 1, calculate z=x*4, and print z in decimal base
  
  3)Otherwise, calculate z=x*8, and prints z in decimal base

2) Converting unsigned numbers in hexadecimal base to unsigned number in binary base:
- A program in C that inputs a string (as a line) from the user, and calls the convertor function that implemented in assembly language.
The program terminated when user enters the "q" input string.
The input string contains ASCII digits, representing an unsigned hexadecimal number

- convertor receives a pointer to the beginning of a null terminated string and convert the input to a binary base number.

