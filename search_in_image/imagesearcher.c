#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(){
    //create three process and check each folder pictures_1 pictures_2 pictures_3 with each process and grep every picture in each folder
    pid_t pid1, pid2, pid3;
    int status1, status2, status3;
    pid1 = fork();
    if(pid1 == 0){
        //child process 1
        printf("Child process 1 is running with pid %d and parent pid %d \n", getpid(), getppid());
        //search for ubuntu with catting every image in pictures_1 and grep ubuntu and if ubuntu is found print the image name
        #define SCRIPT1 "for i in pictures_1/*; do cat $i | grep ubuntu; if [ $? -eq 0 ]; then echo $i; fi; done"
        system(SCRIPT1);
    }
    else{
        //parent process
        pid2 = fork();
        if(pid2 == 0){
            //child process 2
            #define SCRIPT2 "for i in pictures_2/*; do cat $i | grep ubuntu; if [ $? -eq 0 ]; then echo $i; fi; done"
            printf("Child process 2 is running with pid %d and parent pid %d \n", getpid(), getppid());
            system(SCRIPT2);
        }
        else{
            //parent process
            pid3 = fork();
            if(pid3 == 0){
                //child process 3
                #define SCRIPT3 "for i in pictures_3/*; do cat $i | grep ubuntu; if [ $? -eq 0 ]; then echo $i; fi; done"
                printf("Child process 3 is running with pid %d and parent pid %d \n", getpid(), getppid());
                system(SCRIPT3);
            }
            else{
                //parent process
                waitpid(pid1, &status1, 0);
                waitpid(pid2, &status2, 0);
                waitpid(pid3, &status3, 0);
                printf("Parent process is running with pid %d \n", getpid());
            }
        }
    }

}