#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *args[])
{

    if (argc != 2)
    {
        exit(0);
        fprintf(1, "except 2 arguments, but %d found", argc);
    }
    int t = atoi(args[1]);
    fprintf(1, "sleeping\n", t);
    sleep(t);
    exit(0);
}
