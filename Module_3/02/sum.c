#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    //printf("%s_%s_%s", argv[0], argv[1], argv[2]);
    if (argc != 3) exit(1);
    printf("%d\n", (atoi(argv[1]) + atoi(argv[2])));
    return 0;
}