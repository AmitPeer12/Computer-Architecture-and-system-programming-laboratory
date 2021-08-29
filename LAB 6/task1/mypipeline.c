#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <sys/types.h>

void debugMode(char*);
int isD=0;
int main (int argc, char* argv[]){
    for (int i=0;i<argc; i++){
        if (strcmp(argv[i],"-d")==0)
        isD=1;
    }
    int fd[2];
    pid_t p1;
     char *procs[2][4] = { {"ls", "-l", 0 },{ "tail", "-n","2", 0 }};
    if (pipe(fd)==-1)
        perror("error in pipe");
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
        fclose(stdout);
        dup(fd[1]);
        close (fd[1]);
        debugMode("(child1>going to execute cmd: …)\n");
        execvp(procs[0][0],procs[0]);
    }
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
        fclose(stdin);
        dup(fd[0]);
        close (fd[0]);
        debugMode("(child2>going to execute cmd: …)\n");
        execvp(procs[1][0],procs[1]);
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
void debugMode(char* message){
    if (isD==1)
    fprintf(stderr,"%s", message);
}