#include "LineParser.h"
#include<linux/limits.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#define ARR_SIZE 10


void execute(cmdLine*);
void cd(char*);
void executePipeCommand(cmdLine*,const char*,const char*);
void addToHistory (char*);
void printHistory();
int isD=0;
__pid_t p;
char* history[ARR_SIZE];
int ind=0;

void debugMode(char* message){
    if (isD==1)
    fprintf(stderr,"%s", message);
}

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
        if (buff[0]==33){
            int i=buff[1]-48;
            if (i>=ind){
            perror("Invalid command index");
            continue;
            }
            addToHistory(history[i]);
            cLine=parseCmdLines(history[i]);
        }
         else{
            cLine=parseCmdLines(buff);
                    addToHistory(buff);
         }
                   
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
         if(!strcmp(cLine->arguments[0],"history")){
             printHistory();
             freeCmdLines(cLine);
             continue;
         }
         execute(cLine);
        
        if (isD==1)
            fprintf(stderr," now executing: %s pid: %d \n",cLine->arguments[0],p);
        freeCmdLines(cLine);
    }
    for (int i=0;i<ind;i++){
        if (history[i])
        free(history[i]);
    }
    return 0;
    }
void execute(cmdLine* pCmdLine){
    if (pCmdLine->next){
        executePipeCommand(pCmdLine,pCmdLine->inputRedirect,pCmdLine->outputRedirect);
    }
    else{
    p=fork();
    if(pCmdLine->blocking == 1)
        waitpid(p,NULL,0);
    if (p==0){
        if(pCmdLine->inputRedirect){ 
            fclose(stdin);
            if(!fopen(pCmdLine->inputRedirect,"r")){
                perror("failed to load input!");
                exit(0);
            }
        }
        if(pCmdLine->outputRedirect){ 
            fclose(stdout);
            if(!fopen(pCmdLine->outputRedirect,"a+"))
            {
                perror("failed to load output output!");
                exit(0);
            }
        }
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
        perror("failed to execute");
        _exit(-1);
    }
}
}
}
void cd(char * target){
    int error=0;
    if(!target)perror("no target directory");
    error = chdir(target);
    if(error==-1) perror("directory not found");
}
void executePipeCommand(cmdLine* pCmdLine,const char* inputRedirect,const char* outputRedircet){
    int fd[2];
    pid_t p1;
    pipe(fd);
    debugMode("(parent_process>forking…)\n");
        p1=fork();
        if(isD)
        fprintf(stderr,"(parent_process>created process with id: %d)\n",p1);
    if (p1==-1){
        perror("erorr in fork 1");
        exit (-1);
    }
        
    if (p1==0){
        debugMode("(child1>redirecting stdout to the write end of the pipe…)\n");
        if(pCmdLine->inputRedirect){ 
            fclose(stdin);
            if(!fopen(pCmdLine->inputRedirect,"r")){
                perror("failed to load input!");
                exit(0);
            }
        }
        if(pCmdLine->outputRedirect){ 
            fclose(stdout);
            if(!fopen(pCmdLine->outputRedirect,"a+"))
            {
                perror("failed to load output output!");
                exit(0);
            }
        }
        fclose(stdout);
        dup(fd[1]);
        close (fd[1]);
        debugMode("(child1>going to execute cmd: …)\n");
        if (execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
        perror("failed to execute");
        exit(-1);
    }}
    else{
        debugMode("(parent_process>closing the write end of the pipe…)\n");
         close (fd[1]);
    }
    pid_t p2;
    debugMode("(parent_process>forking…)\n");
    p2=fork();
    if(isD==1)
        fprintf(stderr,"(parent_process>created process with id: %d)\n",p2);
    if (p2==-1){
        perror("erorr in fork 2");
        exit (-1);
    }
    if (p2==0){
        debugMode("(child2>redirecting stdin to the read end of the pipe…)\n");
        if(pCmdLine->next->inputRedirect){ 
            fclose(stdin);
            if(!fopen(pCmdLine->next->inputRedirect,"r")){
                perror("failed to load input!");
                exit(0);
            }
        }
        if(pCmdLine->next->outputRedirect){ 
            fclose(stdout);
            if(!fopen(pCmdLine->next->outputRedirect,"a+"))
            {
                perror("failed to load output output!");
                exit(0);
            }
        }
        fclose(stdin);
        dup(fd[0]);
        close (fd[0]);
        debugMode("(child2>going to execute cmd: …)\n");
        if (execvp(pCmdLine->next->arguments[0],pCmdLine->next->arguments)==-1){
        perror ("failed to execute");
        exit (-1);
        }
    }
    else{  
    debugMode("(parent_process>closing the read end of the pipe…)\n");
     close (fd[0]);
     debugMode("(parent_process>waiting for child processes to terminate…)\n");
    }
    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);
        debugMode("(parent_process>exiting…)\n");

}
void addToHistory(char* proc){
    if (ind<ARR_SIZE){
        history[ind] = malloc(strlen(proc) + 1);
        strcpy(history[ind],proc);  
        ind=ind+1;
}
}
void printHistory(){
    for (int i=0; i<ind; i++){
    printf ("#%i  %s", i, history[i]);
    }
}


