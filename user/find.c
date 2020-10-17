#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define false 0
#define true 1

int path_max_length = 100;

void find(char *path, char *pattern)
{
    char buf[512];
    char *p;
    p = buf;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find:can't open %s\n", path);
        return;
    }
    if ((fstat(fd, &st)) < 0)
    {
        fprintf(2, "find:can't stat %s\n", path);
        return;
    }

    if (st.type != T_DIR)
    {
        printf("find: can't find %s in %s", pattern, path);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
        {
            continue;
        }
        switch (st.type)
        {
        case T_FILE:
            // printf("%s,%s\n",p,pattern);
            if (strcmp(p, pattern) == 0)
            {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            find(buf, pattern);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("find: expect 2 arguments, but %d found.", argc);
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}