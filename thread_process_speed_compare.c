#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct randomNumbers{
    int x;
    int y;
    int z;
};
int findBiggestPrimeNumberBeforX(int x){
    //find the biggest prime number before x
    int i, j;
    int isPrime = 1;
    for(i = x - 1; i > 1; i--){
        for(j = 2; j < i; j++){
            if(i % j == 0){
                isPrime = 0;
                break;
            }
        }
        if(isPrime == 1){
            return i;
        }
        isPrime = 1;
    }
    return 1;
}

int findTheBiggestCommonDenominator(int x, int y){
    //find the biggest common denominator of x and y
    int i;
    int biggestCommonDenominator = 1;
    for(i = 1; i <= x && i <= y; i++){
        if(x % i == 0 && y % i == 0){
            biggestCommonDenominator = i;
        }
    }
    return biggestCommonDenominator;
}

int findProductOfAllCommonDivisors(int x, int y){
    //find the product of all common divisors of x and y
    int i;
    int product = 1;
    for(i = 1; i <= x && i <= y; i++){
        if(x % i == 0 && y % i == 0){
            product *= i;
        }
    }
    return product;
}

void *thread1(void * randomNumbers){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    struct randomNumbers *numbers = (struct randomNumbers *)randomNumbers;
    int x = numbers->x;
    int biggestPrimeNumber = findBiggestPrimeNumberBeforX(x);
    gettimeofday(&end, NULL);
    printf("prime number in thread 1: %d ", biggestPrimeNumber);
    printf("Time taken by thread 1: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void *thread2(void * randomNumbers){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    struct randomNumbers *numbers = (struct randomNumbers *)randomNumbers;
    int y = numbers->y;
    int z = numbers->z;
    int biggestCommonDenominator = findTheBiggestCommonDenominator(y, z);
    gettimeofday(&end, NULL);
    printf("the biggest common denominator in thread 2: %d ", biggestCommonDenominator);
    printf("Time taken by thread 2: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void *thread3(void * randomNumbers){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    struct randomNumbers *numbers = (struct randomNumbers *)randomNumbers;
    int y = numbers->y;
    int z = numbers->z;
    int productOfAllCommonDivisors = findProductOfAllCommonDivisors(y, z);
    gettimeofday(&end, NULL);
    printf("the product of all common divisors in thread 3: %d ", productOfAllCommonDivisors);
    printf("Time taken by thread 3: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

int main(){
    //create three random numbers between 1 and 1000
    int random1 = rand() % 1000 + 1; //x
    int random2 = rand() % 1000 + 1; //y
    int random3 = rand() % 1000 + 1; //z

    //create three child process
    pid_t pid1 = fork();
    pid_t pid2 = fork();
    pid_t pid3 = fork();
    if (pid1 == 0){
        //child 1
        struct timeval start, end;
        gettimeofday(&start, NULL);
        int biggestPrimeNumber = findBiggestPrimeNumberBeforX(random1);
        gettimeofday(&end, NULL);
        printf("prime number in child 1: %d ", biggestPrimeNumber);
        printf("Time taken by child 1: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
    }
    else if (pid2 == 0){
        //child2
        struct timeval start, end;
        gettimeofday(&start, NULL);
        int biggestCommonDenominator = findTheBiggestCommonDenominator(random2, random3);
        gettimeofday(&end, NULL);
        printf("the biggest common denominator in child 2: %d ", biggestCommonDenominator);
        printf("Time taken by child 2: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
    }
    else if (pid3 == 0){
        //child 3
        struct timeval start, end;
        gettimeofday(&start, NULL);
        int productOfAllCommonDivisors = findProductOfAllCommonDivisors(random2, random3);
        gettimeofday(&end, NULL);
        printf("product of all common divisors in child 3: %d ", productOfAllCommonDivisors);
        printf("Time taken by child 3: %ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
    }else{
        //parent
        //wait for all child process to finish
        //create three threads
        pthread_t thread1, thread2, thread3;
        struct randomNumbers *randomStruct = (struct randomNumbers *)malloc(sizeof(struct randomNumbers));
        randomStruct->x = random1;
        randomStruct->y = random2;
        randomStruct->z = random3;
        pthread_create(&thread1, NULL, thread1, (void *)randomStruct);
        pthread_create(&thread2, NULL, thread2, (void *)randomStruct);
        pthread_create(&thread3, NULL, thread3, (void *)randomStruct);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }
}
