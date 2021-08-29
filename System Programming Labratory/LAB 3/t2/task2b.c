#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10000
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;

struct link
{
    link *nextVirus;
    virus *vir;
};

struct fun_desc
{
    char *name;
    char (*fun)();
};

void PrintHex(unsigned char buffer[], int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}
virus *readVirus(FILE *input, char *c)
{
    virus *v = malloc(sizeof(virus));
    if (fread(v, sizeof(char), 18, input) != 18)
    {
        free(v);
        return NULL;
    }
    v->sig = malloc(v->SigSize);
    fread(v->sig, sizeof(char), v->SigSize, input);
    return v;
}
void printVirus(virus *virus, FILE *output)
{
    printf("Virus name: %s\n", virus->virusName);
    printf("Virus size: %i\n", virus->SigSize);
    printf("signature:\n");
    PrintHex(virus->sig, virus->SigSize);
}

void list_print(link *virus_list, FILE *f)
{
    while (virus_list != NULL)
    {
        printVirus(virus_list->vir, f);
        virus_list = virus_list->nextVirus;
    }
}

link *list_append(link *virus_list, virus *data)
{
    if (virus_list == NULL)
    {
        virus_list = malloc(sizeof(link));
        virus_list->vir = data;
        virus_list->nextVirus = NULL;
        return virus_list;
    }
    link *l = malloc(sizeof(link));
    l->vir = data;
    l->nextVirus = virus_list;
    return l;
}

void list_free(link *virus_list)
{
    while (virus_list != NULL)
    {
        free(virus_list->vir->sig);
        free(virus_list->vir);
        link *temp = virus_list;
        virus_list = virus_list->nextVirus;
        free(temp);
    }
}

void loadSignatures(link **virusList)
{
    char fileName[256];
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = 0;
    FILE *f = fopen(fileName, "rb");
    char c;
    for (int i = 0; i < 4; i++)
        c = fgetc(f);
    virus *v = readVirus(f, c);

    while (v != NULL)
    {
        *virusList = list_append(*virusList, v);
        v = readVirus(f, c);
    }
    fclose(f);
}
void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE *output)
{
    int i = 0;
    int compare = 0;
    while (virus_list != NULL)
    {
        virus *currVirus = virus_list->vir;
        while (i <= size - currVirus->SigSize)
        {
            compare = memcmp(currVirus->sig, buffer + i, currVirus->SigSize);
            if (compare == 0)
            {
                printf("The starting byte of virus location: %d\nThe virus name is: %s\nThe size of virus signature is: %d", i, currVirus->virusName, currVirus->SigSize);
                printf("\n");
                break;
            }
            i++;
        }
        i = 0;
        virus_list = virus_list->nextVirus;
    }
}
void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
    FILE *input = fopen(fileName, "rb+");
    if (input != NULL)
    {
        char buf[] = {0x90};
        fseek(input, signitureOffset, SEEK_SET);
        for (int i = 0; i < signitureSize; i++)
        {
            fwrite(buf, 1, 1, input);
        }
        fclose(input);
    }
}

int main(int argc, char **argv)
{
    int bounds = 0;
    int opt = 0;
    link *virusList = NULL;
    struct fun_desc menu[] = {{"Load signatures", loadSignatures}, {"Print signatures", list_print}, {"Detect viruses", detect_virus}, {
                                                                                                                                           "Fix file",
                                                                                                                                       },
                              {NULL, NULL}};
    while (1)
    {
        printf("Please choose a function:\n");
        for (int i = 0; menu[i].name != NULL; i++)
        {
            printf("%d) %s\n", i + 1, menu[i].name);
            bounds++;
        }
        printf("Option: ");
        opt = fgetc(stdin) - '0';
        getchar();
        if (opt < bounds && opt > 0)
        {
            if (opt == 1)
            {
                menu[opt - 1].fun(&virusList);
            }
            else if (opt == 2)
            {
                menu[opt - 1].fun(virusList, stdout);
            }
            else if (opt == 3)
            {
                char *buffer = malloc(BUFFER_SIZE);
                FILE *input = fopen(argv[1], "rb");
                if (input != NULL)
                {
                    fseek(input, 0, SEEK_END);
                    long fileSize = ftell(input);
                    fseek(input, 0, SEEK_SET);
                    if (fread(buffer, sizeof(char), fileSize, input) != -1)
                    {
                        menu[opt - 1].fun(buffer, MIN(BUFFER_SIZE, fileSize), virusList, stdout);
                    }
                }
            }
            else if (opt == 4)
            {
                int offset = 0;
                int signitureSize;
                scanf("%i", &offset);
                scanf("%i", &signitureSize);
                kill_virus(argv[1], offset, signitureSize);
            }
            else
                break;
            printf("Done\n");
        }
        if (opt > bounds || opt <= 0)
            break;
    }
    if (opt < bounds && opt > 0)
        list_free(virusList);
    return 0;
}
