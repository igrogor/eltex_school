#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 0)
        exit(1);
    for (int i = 2; i != argc; i++)
    {
        strcat(argv[1], argv[i]);
    }

    printf("%s\n", argv[1]);
    return 0;
}