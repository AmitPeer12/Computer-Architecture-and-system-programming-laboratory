#include "LineParser.h"
#include<linux/limits.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


void execute(cmdLine*);
void cd(char*);
int isD=0;
__pid_t p;

int main (int argc, char** argv){
    char buff[2048];
    cmdLine* cLine;
    int i=0;
    char path[PATH_MAX];
    while(i<argc){
        if(strncmp("-d",argv[i],2)==0)
            isD=1;
        i++;
    }
    
     while (1)
    {
        printf("%s> ",getcwd(path,PATH_MAX));
        fgets(buff,2048,stdin);
        cLine=parseCmdLines(buff);
        if (!strcmp(cLine->arguments[0],"quit")){
            if (isD==1)
                fprintf(stderr," now executing: %s pid: %d \n",cLine->arguments[0],p);
            freeCmdLines(cLine);
            break;
        }
         if(!strcmp(cLine->arguments[0],"cd")){
            cd(cLine->arguments[1]);
            if (isD==1)
                fprintf(stderr," now executing: %s pid: %d \n",cLine->arguments[0],p);
            freeCmdLines(cLine);
            continue;
         }
         execute(cLine);
        
        if (isD==1)
            fprintf(stderr," now executing: %s pid: %d \n",cLine->arguments[0],p);
        freeCmdLines(cLine);
    }
    return 0;
    }
void execute(cmdLine* pCmdLine){
    p=fork();
    if(pCmdLine->blocking == 1)
        waitpid(p,NULL,0);
    if (p==0){
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
        perror("error");
        _exit(-1);
    }
}
}
void cd(char * target){
    int error=0;
    if(!target)perror("no target directory");
    error = chdir(target);
    if(error==-1) perror("directory not found");
}

