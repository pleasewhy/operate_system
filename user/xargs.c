#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("xargs: expect 2 args, but 1 found\n");
    }
    char *exec_args[MAXARG];
    char buf[512];
    int cnt = 0;
    for (int i = 1; i < argc; i++)
    {
        exec_args[cnt] = (char *)malloc(20);
        memmove(exec_args[cnt++], argv[i], strlen(argv[i]));
    }

    int len = read(0, buf, 512);
    int start = 0;

    // printf("%s\n", buf);
    exec_args[cnt] = (char *)malloc(20);
    for (int i = 0; i < len; i++)
    {
        // printf("i:%d len:%d %d\n",i,len,strlen(buf));
        if (buf[i] == '\n')
        {
            memset(exec_args[cnt], 0, 20);
            memmove(exec_args[cnt], buf + start, i - start);
            exec_args[cnt + 1] = (char *)0;
            start = i + 1;
            // printf("%s %d\n", exec_args[cnt - 1], i);
            if (fork() == 0) // child
            {
                // printf("%s ", exec_args[cnt]);
                exec(*exec_args, exec_args);
            }
            else
            {
                wait((int *)0);
            }
        }
    }
    exit(0);
}