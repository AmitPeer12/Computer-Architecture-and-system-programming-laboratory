int digit_cnt (char* string){
    int i = 0;
    int counter=0;
    while (string[i] != 0)
    {
        if (string[i]<='9'&&string[i]>='0')
            counter=counter+1;
        i=i+1;
    }
    return counter;
}
int main(int argc, char * argv[]){}