#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    const int SIZE = 4096;
    const char *name = "OS";
    int n;
    scanf("%d", &n);
    if (n <= 0)
    {
        fprintf(stderr, "negative input number !!!");
        exit(0);
    }
    // create shared memory
    int shm_fd; // shared memory file descriptor
    void *ptr;  // pointer to shared memory object

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // fork a child
    pid_t pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1; // exit(1);
    }
    else if (pid == 0)
    { /* child process */
        // execlp("/bin/ls", "ls", NULL);

        // offset+=sprintf(str+offset,"%d,",rand()%100);
        ptr += sprintf(ptr, "%d", n);

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
            ptr += sprintf(ptr, ", %d", n);
        }
    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete\n");
        printf("%s\n", (char *)ptr);
        shm_unlink(name);
    }
    return 0; // exit(0);
}
