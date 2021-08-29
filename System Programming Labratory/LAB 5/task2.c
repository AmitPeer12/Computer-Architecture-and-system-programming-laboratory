#include <linux/limits.h>
#include <stdio.h>
#include "LineParser.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0


typedef struct process
{
    cmdLine *cmd;         /* the parsed command line*/
    pid_t pid;            /* the process id that is running the command*/
    int status;           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next; /* next process in chain */
} process;
void updateProcessList(process **);

void addProcess(process **process_list, cmdLine *cmd, pid_t pid)
{
    process *p = *process_list;
    process *proc =(process*) malloc(sizeof(process));
    proc->cmd = cmd;
    proc->pid = pid;
    proc->status = RUNNING;
    proc->next = NULL;
    if (p == NULL)
    {
        *process_list = proc;
    }
    else
    {
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = proc;
    }
}
void delete(process**process_list,process * toDelete){
    
    process * curr = *process_list;
    if(curr==toDelete){
        *process_list=(*process_list)->next;
        
    }
    else{
    while (curr->next!=toDelete)
        curr=curr->next;
    
    curr->next=toDelete->next; 
    toDelete->next=NULL;
    }
    freeCmdLines(toDelete->cmd); 
    free(toDelete); 
}
    

void printProcessList(process **process_list)
{
    updateProcessList(process_list);
    process *next = *process_list;
    process* prev=*process_list;
    printf("PID\tCommand\t\tSTATUS\n");
    while (next != NULL)
    {
        printf("%d\t", next->pid);
        for (int i = 0; i < next->cmd->argCount; i++)
            printf("%s ", next->cmd->arguments[i]);
        next->status == TERMINATED ? printf("\t\tTERMINATED\n") : next->status == RUNNING ? printf("\t\tRUNNING\n")
                                                                                    : printf("\t\tSUSPENDED\n");
        if (next->status==TERMINATED){
         prev=next;
         next=next->next;
         delete(process_list,prev);
         continue;                       
    }
    next=next->next;
}
}
void freeProcessList(process *process_list)
{
    process *p = process_list;
    while (p != NULL)
    {
        process *temp = p;
        p = p->next;
        if (temp->cmd!=NULL);
        freeCmdLines(temp->cmd);
        free(temp);
    }
}


void updateProcessStatus(process* process_list, int pid, int status){
    process* temp=process_list;
    while (temp!=NULL){
        if (temp->pid==pid){
            temp->status=status;
            break;
    }
        temp=temp->next;
    }
}
void updateProcessList(process **process_list){
    int  status=0;
    pid_t p=0;
    process* tempProc=*process_list;
    while (tempProc!=NULL){
        status=0;
        p=waitpid(tempProc->pid,&status,WNOHANG|WUNTRACED|WCONTINUED);
        if (WIFCONTINUED(status))
            updateProcessStatus(*process_list,tempProc->pid,RUNNING);
        else if (WIFSTOPPED(status))
            updateProcessStatus(*process_list,tempProc->pid,SUSPENDED);
        else if (p==-1||WIFSIGNALED(status))
            updateProcessStatus(*process_list,tempProc->pid,TERMINATED);
        tempProc=tempProc->next;
}
}
int dMode = 0;
__pid_t pid;
void execute(cmdLine *pCmdLine)
{
    int status;
    pid = fork();
    if (pCmdLine->blocking == 1)
        waitpid(pid, &status, 0);
    if (pid == 0)
    {
        if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
        {
            perror("Could not execute the line\n");
        }
        exit(-1);
    }
}
void cd(char *target)
{
    int err = 0;
    if (!target)
        perror("couldn't execute cd command without target directory");
    err = chdir(target);
    if (err == -1)
        perror("couldn't find the directory");
}

process *procList = NULL;

int main(int argc, char *argv[])
{
    char buf[2048];
    char path[PATH_MAX];
    for (int i = 0; i < argc; i++)
    {
        if (strncmp("-d", argv[i], 2) == 0)
            dMode = 1;
    }
    while (1)
    {
        printf("%s> ", getcwd(path, PATH_MAX));
        fgets(buf, 2048, stdin);
        cmdLine *line = parseCmdLines(buf);
        if (strcmp(line->arguments[0], "quit") == 0)
        {
            freeCmdLines(line);
            break;
        }
        else if (strcmp(line->arguments[0], "procs") == 0)
        {
            printProcessList(&procList);
            freeCmdLines(line);
            continue;
        }
        else if (strcmp(line->arguments[0], "cd") == 0)
        {
            cd(line->arguments[1]);
            freeCmdLines(line);
            continue;
        }
        else if (strcmp(line->arguments[0],"suspend")==0){
            pid_t proc_id=atoi(line->arguments[1]);
            int time=atoi(line->arguments[2]);       
            if ((pid=fork())==0){
              int killVal= kill(proc_id,SIGTSTP);
               if(killVal==-1){
               perror("unable to suspend");
               exit (0);
               }
                sleep(time);
                killVal=kill(proc_id, SIGCONT);
                if(killVal==-1)
                    perror("unable to return from suspend");
                exit (0);
            }
            freeCmdLines(line);
            continue;
        }
        if(strcmp(line->arguments[0],"kill")==0){
            int killVal=kill(atoi(line->arguments[1]),SIGINT);
            freeCmdLines(line);
            if(killVal==-1){
                perror("unable to kill");
            }
            continue;
        }
        execute(line);
        if (dMode == 1)
        {
            fprintf(stderr, "pid: %d\tExecuting command: %s", pid, buf);
        }
        addProcess(&procList, line, pid);
        
    }
    freeProcessList(procList);
    return 0;
}
