#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000
#define INPUT_SIZE 100
#define ONE_BYTE 1
#define TWO_BYTES 2
#define FOUR_BYTES 4



typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  long file_size;
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
} state;

typedef struct {
  char *name;
  void (*fun)(state *);
}fun_desc;

void toggleDebugMode(state *s){
  if (s->debug_mode){
    s->debug_mode=0;
    printf("Debug flag now off\n");
  }
  else{
    printf("Debug flag now on\n");
    s->debug_mode=1;
  }
}
void setFileName(state *s){
   if (s) {
        printf("Please Choose File\n");
        scanf("%s", s->file_name);
    }
    if(s->debug_mode)
        printf("Debug: file name set to:%s \n",s->file_name);
}
void setUnitSize(state *s){
  int size;
  printf("Please select size\n");
  scanf("%d", &size);
        if (size != 1 && size != 2 && size != 4)
         printf("Invalid size\n");
         else {
            s->unit_size = size;
            if (s->debug_mode)
                printf("Debug: set size to %d\n", size);
        }
}
void quit(state *s){
  if (s->debug_mode)
  printf("quitting\n");
  free(s);
  exit(0);
}
void loadIntoMemory(state *s){
  FILE* f=NULL;
   int location;
    int length;
    char buff[2*INPUT_SIZE];
  if (strcmp(s->file_name,"")==0){
  printf("error: no file detached\n");
  return;
}
   f=fopen(s->file_name,"rb");
   if (f){
      printf("Please enter <location> <length>\n");
      fgets(buff, INPUT_SIZE, stdin);
      sscanf(buff, "%X %d\n", &location, &length);
      s->mem_count = length * s->unit_size;
      fseek(f, location, SEEK_SET);
      fread(s->mem_buf, s->unit_size, s->mem_count, f);
      fclose(f);
      printf("Loaded %d bytes into memory\n",s->mem_count);
      if (s->debug_mode){
        printf("Debug filename: %s\n", s->file_name);
        printf("Debug location: %X\n", location);
        printf("Debug length: %d\n", s->mem_count);
   }
   }
   else{
      printf("error: failed to open file\n");
     return;
   }
   }
void memoryDisplay(state *s){
  int u;
  int addr;
  char buff[2*INPUT_SIZE];
  char* mem_buff;
  int toConvert;
  printf("Enter address and length\n");
  fgets(buff, 2*INPUT_SIZE, stdin);
  sscanf(buff, "%X %d\n", &addr, &u);
   if (addr==0)
    mem_buff=(char*)s->mem_buf;
  else
   mem_buff=(char*)addr;
  printf("Hexadecimal\n");
  printf("===========\n");
    for (int i = 0; i < u; i=i+1) {
    
        toConvert = *((int*)(mem_buff+(i*s->unit_size)));
        if (s->unit_size == ONE_BYTE)
        printf("%hhx\n", toConvert);
        else if (s->unit_size == TWO_BYTES){printf("%hX\n", toConvert);}
        else    {printf("%X\n", toConvert);}
    }
  printf("Decimal\n");
  printf("===========\n");
  for (int i = 0; i < u; i=i+1) {

        toConvert = *((int*)(mem_buff+(i*s->unit_size)));

        if (s->unit_size == ONE_BYTE)
        printf("%hhd\n", toConvert);
        else if (s->unit_size == TWO_BYTES){printf("%hd\n", toConvert);}
        else    {printf("%d\n", toConvert);}
    }

}
void saveIntoFile(state *s){
  char buff[3*INPUT_SIZE];
  FILE* f=NULL;
  long fSize=0;
  unsigned long source;
  unsigned long target;
  int length;
  
  printf("Please enter <source-address> <target-location> <length>\n");
  fgets(buff, INPUT_SIZE, stdin);
  sscanf(buff, "%lx %lx %d\n", &source, &target, &length);
   if(s->debug_mode==1)
        printf("Debug:\nsource: %lx\ntarget: %lx\nlength: %d\n", source, target, length);
  f=fopen(s->file_name,"rb+");
  if (!f){
    printf("error: cannot open the file");
    return;
  }
  fseek(f,0L,SEEK_END); //search for the end to know file size
  fSize=ftell(f);
  if (target>fSize){
  printf("error: target location is greater than file size");
  return;
  }
  fseek(f,target,SEEK_SET);
  
  if (source==0)
      fwrite(s->mem_buf,s->unit_size,length,f);
  else if (f)
    fwrite((unsigned char*)source,s->unit_size,length,f);
    
  fclose(f);
}
void memoryModify(state *s){
   char buff[INPUT_SIZE];
    int location=0;
    int value=0;
    printf("Please enter <location> <val>\n");
    fgets(buff,INPUT_SIZE,stdin);
    sscanf(buff,"%x %x",&location,&value);
    if (s->debug_mode==1)
    printf("Debug: location: %X\nDebug: value: %x\n",location,value);
    if(location > BUF_SZ){
     printf("error: location bigger then buffer!\n");
     return;
    }
     int * writeP = (int*)(&s->mem_buf[location]);
        *writeP= value; 

}

int main(int argc, char** argv){
    int opt;
    int numOfOptions=0;
    state* s = calloc(1,sizeof(state));
    s->debug_mode = 0;
    s->unit_size = ONE_BYTE;
  fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode},
                      {"Set File Name", setFileName},
                      {"Set Unit Size", setUnitSize},
                      {"Load Into Memory", loadIntoMemory},
                      {"Memory Display", memoryDisplay},
                      {"Save Into File", saveIntoFile},
                      {"Memory Modify", memoryModify},
                      {"Quit", quit},
                        {NULL, NULL}};
    for (int i = 0; menu[i].name!=NULL; i++)
        numOfOptions++;      
    while(1){
      printf("Please choose a function:\n");
      for (int i = 0; menu[i].name!=NULL; i++){
      printf("%d-%s\n",i,menu[i].name);
      }
    printf("Option: ");
    scanf("%d", &opt);
    if (opt>=numOfOptions||opt<0)
      break;
    getchar();
    menu[opt].fun(s);
    }
    return 0;
}