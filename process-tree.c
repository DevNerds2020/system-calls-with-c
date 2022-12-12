#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int pid, pid1, pid2;
    int *ptr11, *ptr12 ,*ptr21, *ptr22, *ptr3;
    
    int parentpipe[2];
    pipe(parentpipe);

    //make three random numbers and put them in a,b,c between 1 to 20
    int a = rand() % 20 + 1;
    int b = rand() % 20 + 1;
    int c = rand() % 20 + 1;

    /*variable pid will store the
	value returned from fork() system call*/
	pid = fork();
	/* If fork() returns zero then it
	 means it is child process. ch1*/
	if (pid == 0) {
        int ch11, ch12;
        int ch1pipea[2];
        int ch1pipeb[2];
        pipe(ch1pipea);
        pipe(ch1pipeb);
        //ch11
        ch11 = fork();
        //calculate the sum of squares of one to a in process ch11 and put it in an pointer
        if(ch11 == 0){
            int sum = 0;
            for(int i = 1; i <= a; i++){
                sum += i * i;
            }
            printf("sum of squares of 1 to %d is %d in ch11 \n", a, sum);
            //send this pointer to parent
            //doesnt need to read from parent
            close(ch1pipea[0]);
            write(ch1pipea[1], &sum, sizeof(sum));
            //show value of the pointer
            // int result = *ptr11;
            // printf("ptr11 is %d in ch11 \n", result);
        }else{
            //ch12
            ch12 = fork();
            //calculate the sum of one to a in process ch12 and put it in another pointer
            if(ch12 == 0){
                int sum12 = 0;
                for(int i = 1; i <= a; i++){
                    sum12 += i;
                }
                printf("sum12 of numbers of 1 to %d is %d in ch12 \n", a, sum12);
                
                //pas sum to parent with a pointer
                //send this pointer to parent
                //doesn't need to read from parent
                close(ch1pipeb[0]);
                write(ch1pipeb[1], &sum12, sizeof(sum12));
                //show value of the pointer
                // int result12 = *ptr12;
                // printf("ptr12 is %d in ch12 \n", result12);
            }else{
                // printf("here in ch1 before wait \n");
                //ch1 should wait for ch11 and ch12 to finish
                waitpid(ch11, NULL, 0);
                waitpid(ch12, NULL, 0);
                // printf("here in ch1 \n ");
                //get child pointers
                //doesn't need to write to child
                int sum1, sum2;
                close(ch1pipea[1]);
                read(ch1pipea[0], &sum1, sizeof(sum1));
                close(ch1pipeb[1]);
                read(ch1pipeb[0], &sum2, sizeof(sum2));
                int ch11Result = sum1/a;
                int ch12Result = sum2/a;
                printf("result 11 is %d in ch1 \n", ch11Result);
                printf("result 12 is %d in ch1 \n", ch12Result);
                //save result11 and result 12 in a file
                FILE *fptr;
                fptr = fopen("result.txt", "w");
                fprintf(fptr, "%d %d", ch11Result, ch12Result);
                fclose(fptr);
                exit(0);
            }
        }    
	}

	else {
        //ch2
        int pipech221[2];
        int pipech222[2];
        pipe(pipech221);
        pipe(pipech222);
		pid1 = fork();
		if (pid1 == 0) {
            int ch21, ch22;
            //ch21
            ch21 = fork();
            //calculate the variance of a b c in ch21
            if(ch21 == 0){
                //calculate the mean
                int mean = (a + b + c) / 3;
                //calculate the variance
                int variance = (a - mean) * (a - mean) + (b - mean) * (b - mean) + (c - mean) * (c - mean);
                //print the variance
                printf("variance of %d %d %d is %d in ch21 \n", a, b, c, variance);
                ptr21 = &variance;
                //convert the variance to string
                char stringvariance [80];
                sprintf(stringvariance, "%d", variance);
                close(pipech221[0]); //does not need to read
                write(pipech221[1], stringvariance, (strlen(stringvariance)+1));
                exit(0);
            }else{
                //ch22
                ch22 = fork();
                //calculate the mean of a b c in ch22
                if(ch22 == 0){
                    //calculate the mean
                    //wait for ch21 to finish
                    int numbersmean = (a + b + c) / 3;
                    //print the numbersmean
                    printf("numbersmean of %d %d %d is %d in ch22 \n", a, b, c, numbersmean);
                    ptr22 = &numbersmean;
                    char stringnumbersmean [80];
                    sprintf(stringnumbersmean, "%d", numbersmean);
                    close(pipech222[0]); //does not need to read
                    write(pipech222[1], stringnumbersmean, (strlen(stringnumbersmean)+1));
                    exit(0);
                }else{
                    //ch2 should wait for ch21 and ch22 to finish
                    waitpid(ch21, NULL, 0);
                    waitpid(ch22, NULL, 0);
                    // printf("hereee in ch2 \n");
                    /* Parent process closes up output side of pipe */
                    close(pipech221[1]);//Parent process does not need this end of the pipe
                    /* Read in a string from the pipe */
                    char readbuffervariance[80];
                    int nbytes = read(pipech221[0], readbuffervariance, sizeof(readbuffervariance));
                    // printf("Read string IN CH2 FINALYY: %s \n", readbuffervariance);
                    //convert variance to int
                    int variance = atoi(readbuffervariance);
                    /* Parent process closes up output side of pipe */
                    close(pipech222[1]);//Parent process does not need this end of the pipe
                    char readbuffermean[80];
                    int nbytes1 = read(pipech222[0], readbuffermean, sizeof(readbuffermean));
                    // printf("Read string IN CH2 FINALYY: %s \n", readbuffermean);
                    //convert mean to int
                    int mean = atoi(readbuffermean);
                    //calculate coEfficient of variation
                    int coEfficient = variance / mean;
                    close(parentpipe[0]); //does not need to read
                    write(parentpipe[1], &coEfficient, sizeof(coEfficient));
                    exit(0);
                }
            }
		}
		else {
            //ch3
			pid2 = vfork();
            //find the biggest number between a b c in ch3
			if (pid2 == 0) {
                int max = a;
                if(b > max){
                    max = b;
                }
                if(c > max){
                    max = c;
                }
                printf("max of %d %d %d is %d in ch3 \n", a, b, c, max);
                ptr3 = &max;
                exit(0);
			}

			// If value returned from fork()
			// in not zero and >0 that means
			// this is parent process.
			else {
                //wait for ch1 ch2 ch3 to finish
                waitpid(pid, NULL, 0);
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
                //get the result from ch1 from file
                FILE *fptr;
                fptr = fopen("result.txt", "r");
                int ch11Result, ch12Result;
                fscanf(fptr, "%d %d", &ch11Result, &ch12Result);
                fclose(fptr);
                printf("result 1 is %d %d in parent \n", ch11Result, ch12Result);
                //get the result from ch2 
                close(parentpipe[1]); //does not need to write
                int coEfficient;
                read(parentpipe[0], &coEfficient, sizeof(coEfficient));
                printf("result 2 is %d in parent \n", coEfficient);
                //get the result from ch3
                int ch3Result = *ptr3;
                printf("result 3 is %d in parent \n", ch3Result);
			}
		}
	}

	return 0;
}
