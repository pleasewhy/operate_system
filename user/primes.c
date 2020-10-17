#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define false 0
#define true 1

int readInt(int fd)
{
    int res = 0;
    read(fd, &res, 4);
    return res;
}

void convert_pipe(int *right, int *left)
{
    int tmp0, tmp1;
    tmp0 = right[0];
    tmp1 = right[1];
    right[0] = left[0];
    right[1] = left[1];
    left[0] = tmp0;
    left[1] = tmp1;
}

int main(int argc, char *argv[])
{
    int end = 35;
    int factor = 2;
    int right[2];
    int left[2];
    pipe(right);
    if (fork() > 0) // parent
    {
        close(right[0]); // close read end.
        for (int i = 2; i <= end; i++)
        {
            write(right[1], &i, sizeof(int));
        }
        close(right[1]); // close write end.
        wait((int *)0);
        exit(0);
    }
    else // child
    {
        left[0] = right[0];
        left[1] = right[1];
        char create_child = false;
        factor = readInt(left[0]);
        int num;
        close(left[1]);
        printf("prime %d\n", factor);
        while ((num = readInt(left[0])) != 0)
        {
            if (num % factor != 0)
            {
                if (create_child == true)
                {
                    write(right[1], &num, sizeof(int));
                }
                else
                {
                    pipe(right);
                    convert_pipe(right, left);
                    create_child = false;
                    if (fork() == 0)
                    {
                        close(left[1]);
                        factor = readInt(left[0]);
                        printf("prime %d\n", factor);
                        continue;
                    }
                    else
                    {
                        convert_pipe(right, left);
                        create_child = true;
                        write(right[1], &num, sizeof(int));
                        close(right[0]);
                    }
                }
            }
        }
        close(right[1]);
        wait((int*)0);
    }
    exit(0);
}