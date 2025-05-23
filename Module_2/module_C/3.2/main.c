#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "header.h"

int main(int argc, char *argv[])
{
    uint32_t net = moveip(argv[1]);
    uint32_t mask = moveip(argv[2]);

    int N = atoi(argv[3]);
    int count = 0;
    uint32_t newIP = 0;
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        newIP = 0;
        int r = rand() % 256;
        newIP = newIP | r;
        newIP = newIP << 8;

        r = rand() % 256;
        newIP = newIP | r;
        newIP = newIP << 8;

        r = rand() % 256;
        newIP = newIP | r;
        newIP = newIP << 8;

        r = rand() % 256;
        newIP = newIP | r;

        if (is_in_subnet(newIP, mask, net))
            count += 1;
    }
    printf("ALL: %d\t true: %d\t -conetct %f\n", N, count, ((double)count / N)* 100);
    return 0;
}
