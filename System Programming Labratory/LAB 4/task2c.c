#include "util.h"
#define WRITE 4
#define STDOUT 1
#define STDIN 0
#define STDOUT 1
#define READ 3
#define OPEN 5
#define CLOSE 6
#define EXIT 1
#define BUF_SIZE 8192
#define GETDENTS 141
# define DT_UNKNOWN 0
# define DT_FIFO 1
# define DT_CHR 2     
# define DT_DIR 4    
# define DT_BLK 6
# define DT_REG 8
# define DT_LNK 10                
# define DT_SOCK 12
# define DT_WHT 14
extern infection();
extern infector();
extern code_start();
extern code_end();


struct linux_dirent {
  long           d_ino;
  long          d_off;
  unsigned short d_reclen;
  char           d_name[];
};

int main (int argc , char* argv[], char* envp[])
{
  int direct, read;
  int status = 0 ;
  int isP=0;
  int isA=0;
  char* Afile;
  char* pref;
  int direntType;
  char buf[BUF_SIZE], d_type;
  struct linux_dirent *linux_dirent;
   system_call(WRITE,STDOUT,"code line of code_start is:\t",strlen("code line of code_start is:\t"));
  system_call(WRITE,STDOUT,itoa(code_start),strlen(itoa(code_start)));
  system_call(WRITE,STDOUT,"\n",strlen("\n"));
  system_call(WRITE,STDOUT,"code line of code_end is:\t",strlen("code line of code_end is:\t"));
  system_call(WRITE,STDOUT,itoa(code_end),strlen(itoa(code_end)));
  system_call(WRITE,STDOUT,"\n",strlen("\n"));

  direct = system_call(OPEN,".", 0);
  if (direct<0)
    system_call(EXIT, 0X55, 1, 1);
  read = system_call(GETDENTS, direct, buf, BUF_SIZE);
  if (read < 0)
      system_call(EXIT, 0X55, 1, 1);
      int i=0;
      while  (i < argc)
  {
    if (!strncmp("-p", argv[i], 2))
    {
      isP = 1;
      pref = argv[i] + 2;
    }
    if (!strncmp("-a", argv[i], 2))
    {
      isA = 1;
      pref = argv[i] + 2;
    }

    i=i+1;
  }
  
  
  while (status < read)
  {
      linux_dirent = (struct linux_dirent *) (buf + status);
       direntType = *(buf + status + linux_dirent->d_reclen - 1);
      if (isP==0&&isA==0){
      system_call(WRITE,STDOUT,linux_dirent->d_name,strlen(linux_dirent->d_name));
      system_call(WRITE,STDOUT,"\n",1);
      }
      else
        if ((isP==1) && strncmp(linux_dirent->d_name, pref, strlen(pref)) == 0){
          system_call(WRITE,STDOUT,linux_dirent->d_name,strlen(linux_dirent->d_name));
          system_call(WRITE,STDOUT,"\t",1);
          printType(direntType);
          system_call(WRITE,STDOUT,"\n",1);
        }
        else
        if ((isA==1)&& strncmp(linux_dirent->d_name, pref, strlen(pref)) == 0){
           system_call(WRITE,STDOUT,linux_dirent->d_name,strlen(linux_dirent->d_name));
          system_call(WRITE,STDOUT,"\t",1);
          printType(direntType);
          infector(linux_dirent->d_name);
          system_call(WRITE,STDOUT,"\n",1);

        }
      
    status = status + linux_dirent->d_reclen;
  }
  system_call(CLOSE, direct);
  return 0;
  }
void printType(int d_type){
  int code1;
  switch (d_type)
  {
    case DT_REG:
      code1 = system_call(WRITE,STDOUT,"regular\t",strlen("regular\t"));
      break;
    case DT_DIR:
      code1 = system_call(WRITE,STDOUT,"directory\t",strlen("directory\t"));
      break;
    case DT_FIFO:
      code1 = system_call(WRITE,STDOUT,"FIFO\t",strlen("FIFO\t"));
      break;
    case DT_SOCK:
      code1 = system_call(WRITE,STDOUT,"socket\t",strlen("socket\t"));
      break;
    case DT_LNK:
      code1 = system_call(WRITE,STDOUT,"symlink\t",strlen("symlink\t"));
      break;
    case DT_BLK:
      code1 = system_call(WRITE,STDOUT,"block dev\t",strlen("block dev\t"));
      break;
    case DT_CHR:
      code1 = system_call(WRITE,STDOUT,"char dev\t",strlen("char dev\t"));
      break;
    default:
      break;
}
}
