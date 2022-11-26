#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int main(){
    pid_t pid;
    //use categorized.sh as a script for categorizing files in categorized folder
    #define SCRIPT "bash categorized.sh"
    //count the number of directories that are inside of categorized folder only directories not files
    #define COUNT "ls -l categorized | grep ^d | wc -l > output.txt"
    //run script2 and store the output in a file
    system(COUNT);
    //get the number number of folders from out put file in line 1
    FILE *fp = fopen("output.txt", "r");
    char line[100];
    fgets(line, 100, fp);
    int num = atoi(line);
    //close the file
    fclose(fp);
    //remove the output file
    system("rm output.txt");
    //show the number of folders
    printf("Number of folders: %d \n", num);
    //create a process for each folder
    for(int i = 0; i < num; i++){
        pid = fork();
        if(pid == 0){
            //run the script for each folder
            system(SCRIPT);
            exit(0);
        }
    }
}