#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int n;
    scanf("%d", &n);
    if (n <= 0)
    {
        fprintf(stderr, "negative input number !!!");
        exit(0);
    }
    pid = fork(); /* fork another process */
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1; // exit(1);
    }
    else if (pid == 0)
    { /* child process */
        // execlp("/bin/ls", "ls", NULL);
        printf("%d", n);
        while (n != 1)
        {
            if (n % 2 == 0)
            {
                n /= 2;
            }
            else
            {
                n = n * 3 + 1;
            }
            printf(", %d", n);
        }
        printf("\n");
    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete\n");
    }
    return 0; // exit(0);
}
