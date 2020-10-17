#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    char send = 'a';
    char receive;
    int pid = fork();
    if (pid < 0)
    {
        exit(1);
        fprintf(2, "forked failed\n");
    }
    else if (pid == 0)
    {
        sleep(1);
        read(fd[0], &receive, 1);
        printf("%d: received ping\n", getpid());
        write(fd[1], &send, 1);
    }
    else
    {
        write(fd[1], &send, 1);
        wait((int *)0);
        read(fd[0], &receive, 1);
        printf("%d: received pong\n", getpid());
    }
    close(fd[0]);
    close(fd[1]);
    exit(0);
}