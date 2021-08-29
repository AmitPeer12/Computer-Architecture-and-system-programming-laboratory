#include <stdio.h>
int main (int argc, char **argv) {
    FILE *output = stdout;
    FILE *input = stdin;
    char r;
    int count = 0;
    int condition = 0;
    int adding = 1;
    char key;
    for(int i=1;i<argc;i++){
        //1a case
        if(argc == 1)
            break;
        else{
            //1b case
            if(strcmp(argv[i],"-D")==0)
                condition = 1;
            //1c case of substract
            else if(strncmp(argv[i],"-e",2)==0){  
                condition = 2;
                adding = 0;
            }
            //1c case of adding
            else if(strncmp(argv[i],"+e",2)==0)
                condition = 2;
            //1d case
            else if(strncmp(argv[i],"-i",2)==0){
                input=fopen(argv[i] + 2,"r");
                if(input==NULL){
                    printf("Error in opening file\n");
                    exit(1);
                } 
            }
            else if(strncmp(argv[i], "-o",2)==0){
                condition = 4;
                output = fopen(argv[i] + 2, "w");
            }
        }
    }
    while((r=fgetc(stdin))!=EOF){
        if(condition == 1 && r !=10)
            fprintf(stderr, "%i\t",r);
        if(condition>=0 && condition<=1){
            if(r>=65 && r<=90){
            r = r +('a' - 'A');
            count = count +1;
            }   
        }
        else if(condition == 2){
            if(adding==0)
                r = r - (key - '0');
            else r = r  +(key - '0');
        }
         if(condition == 1 && r!=10)
            fprintf(stderr, "%i\n",r);
         if(r == '\n' && condition == 1)
            fprintf(stderr,"the number of letters: %i\n", count);
        fputc(r,output);
    }
    if(condition==4)
        fclose(output);
    printf("\n\n");
    return 0; 
}