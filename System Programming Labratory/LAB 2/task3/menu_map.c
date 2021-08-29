//#include</home/caspl202/Desktop/archi/Lab2/Task2/Task2.c>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc
{
    char *name;
    char (*fun)(char);
};
char censor(char c)
{
    if (c == '!')
        return '.';
    else
        return c;
}
char encrypt(char c)
{
    if (c >= 0x20 && c <= 0x7E)
        c = c + ('D' - 'A');
    return c;
}
/* Gets a char c and returns its encrypted form by adding 3 to its value. 
If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c)
{
    if (c >= 0x20 && c <= 0x7E)
        c = c - ('D' - 'A');
    return c;
}
/* Gets a char c and returns its decrypted form by reducing 3 to its value. 
If c is not between 0x20 and 0x7E it is returned unchanged */
char cprt(char c)
{
    if (c >= 0x20 && c <= 0x7E)
        printf("%c\n", c);
    else
        printf(".\n");
    return c;
}
/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
the value of c unchanged. */
char my_get(char c)
{
    return fgetc(stdin);
}
/* Ignores c, reads and returns a character from stdin using fgetc. */

char *map(char *array, int array_length, char (*f)(char))
{
    getchar();
    char *mapped_array = (char *)(malloc(array_length * sizeof(char)));
    for (int i = 0; i < array_length; i++)
    {
        mapped_array[i] = f(array[i]);
    }
    free(array);
    return mapped_array;
}
int main(int argc, char **argv)
{
    struct fun_desc menu[] = {{"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt}, {"print string", cprt}, {"Get string", my_get}, {NULL, NULL}};
    char *carray = (char *)(malloc(5 * sizeof(char)));
    char opt = 0;
    strcpy(carray, "");
    while (1)
    {
        printf("Please choose a function:\n");
        for (int i = 0; menu[i].name != NULL; i++)
        {
            printf("%d) %s\n", i, menu[i].name);
        }
        printf("Option: ");
        opt = getc(stdin) - '0';
        if (opt >= 0 && opt <= 4)
        {
            printf("Within bounds\n");
            carray = map(carray, 5, menu[opt].fun);
            printf("DONE.\n");
        }
        else
            break;
    }
    printf("Not within bounds\n");
    free(carray);
    exit(1);
    return 0;
}