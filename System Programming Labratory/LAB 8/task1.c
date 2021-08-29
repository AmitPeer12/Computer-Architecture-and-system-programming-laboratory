#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME_LEN 128
#define BUF_SZ 10000
#define INPUT_SIZE 100

char dMode = 0;
int Currentfd = -1;
void *map_start = NULL;
off_t Currentfd_size = 0;
Elf32_Ehdr *header;
Elf32_Shdr *section_headers;

typedef struct
{
    char *name;
    void (*fun)();
} fun_desc;

void toggleDebugMode();
void examine_ELF_File();
void print_section_names();
void print_symbols()
{
    printf("not implemented yet\n");
}
void rel_tab()
{
    printf("not implemented yet\n");
}
void quit();
void printEIData(unsigned char c);

int main(int argc, char **argv)
{
    fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode},
                       {"Examine ELF File", examine_ELF_File},
                       {"Print Section Names", print_section_names},
                       {"Print Symbols", print_symbols},
                       {"Relocation Tables", rel_tab},
                       {"Quit", quit},
                       {NULL, NULL}};
    int opt = 0;
    while (1)
    {
        int numOfOptions = 0;
        printf("Choose action:\n");
        for (int i = 0; menu[i].fun != NULL && menu[i].name != NULL; i++)
        {
            numOfOptions++;
            printf("%d) %s\n", i, menu[i].name);
        }
        scanf("%d", &opt);
        getchar();
        if (opt > numOfOptions || opt < 0)
        {
            printf("Invalid number\n");
            break;
        }
        menu[opt].fun();
    }
    return 0;
}

void toggleDebugMode()
{
    if (dMode == 0)
        dMode = 1;
    else
        dMode = 0;
    fprintf(stderr, "Debug flag now %s\n", dMode == 1 ? "on" : "off");
}
void examine_ELF_File()
{
    FILE *file = NULL;
    int err = 0;
    char buf_name[100];
    struct stat fd_stat;
    printf("Enter file name: ");
    scanf("%s", buf_name);
    //close open file in Currentfd
    if (Currentfd != -1)
    {
        err = close(Currentfd);
        if (err < 0)
        {
            printf("couldn't close file indicated by fd: %d\n", Currentfd);
            err = 0;
        }
        else
            munmap(map_start, Currentfd_size); //allocate mapping in map_start address for Currentfd_size size of file
    }
    file = fopen(buf_name, "r");
    if (!file)
    {
        printf("couldn't open %s file\n", buf_name);
        Currentfd = -1; //there is no valid file
        return;
    }
    Currentfd = fileno(file);            //get the file descriptor
    if (fstat(Currentfd, &fd_stat) != 0) //return stat structure of the file
    {
        perror("stat failed");
        return;
    }
    Currentfd_size = fd_stat.st_size;
    if ((map_start = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_SHARED, Currentfd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        return;
    }
    header = (Elf32_Ehdr *)map_start;
    printf("First three bytes of file: ");
    for (int i = 1; i < 4; i++)
    {
        printf("%X\t", header->e_ident[i]);
    }
    printf("\n");
    if (!(header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F'))
    {
        printf("Not an elf file!\n");
        return;
    }
    //print The data encoding scheme of the object file.
    printEIData(header->e_ident[EI_DATA]);
    //print entry point
    printf("Entry point: %X \n", header->e_entry);
    //print The file offset in which the section header table resides.
    printf("File offset in section header: %X\n", header->e_shoff);
    //print the number of section headers
    printf("Number of section headers: %X\n", header->e_shnum);
    //print the size of each section header entry.
    printf("Sections headers size: %X\n", header->e_shentsize);
    //print the file offset in which the program header table resides.
    printf("File offset in program header: %X\n", header->e_phoff);
    //print the number of program header entries.
    printf("Number of program header entries: %X\n", header->e_phnum);
    //print the size of each program header entry.
    printf("Program headers size: %X\n", header->e_phentsize);
}
void printEIData(unsigned char c)
{
    c == ELFDATA2LSB   ? printf("The data is 2's complement, little endian\n")
    : c == ELFDATA2MSB ? printf("The data is 2's complement, big endian\n")
                       : printf("Data content: Unknown data format.\n");
}

void quit()
{
    fprintf(stderr, "quitting\n");
    if (Currentfd != -1)
    {
        if ((header != NULL) && (Currentfd_size != -1))
            munmap(header, Currentfd_size);
        close(Currentfd);
    }
    exit(0);
}

void print_section_names()
{
    char *stringTable;
    if (Currentfd == -1)
    {
        printf("Invalid file!\n");
        return;
    }
    header = (Elf32_Ehdr *)map_start;
    section_headers = (Elf32_Shdr *)(map_start + header->e_shoff); //pointer to section headers
    stringTable = (char *)(map_start + section_headers[header->e_shstrndx].sh_offset);
    // pointer to beginning of names in section header
    if (dMode == 1)
        printf("Section headers string table offset: %X\n", header->e_shstrndx);
    printf("%-2s %-27s %-23s %-15s %-12s %-5s\n", "[Nr]", "Name", "Addr", "Off", "Size", "Type");
    for (int i = 0; i < header->e_shnum; i++)
    {
        printf("[%d] %-15s\t\t%08X\t\t%06X\t\t%06X\t\t%X\n", i, stringTable + section_headers[i].sh_name,
               section_headers[i].sh_addr,
               section_headers[i].sh_offset, section_headers[i].sh_size,
               section_headers[i].sh_type);
    }
}