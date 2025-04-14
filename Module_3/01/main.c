#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Написать программу, вычисляющую площади квадратов с заданной длиной стороны.
Длины сторон передаются как аргументы запуска.
Расчеты делают родительский и дочерний процессы, разделяя задачи примерно поровну.
*/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        // return 1;
        exit(1);
    }
    int size = argc - 1;
    pid_t pid = fork();
    int start, stop;

    switch (pid)
    {
    case -1:
        exit(1);
        break;
    case 0:
        start = (size + 1) / 2;
        stop = size;
        break;
    default:
        start = 0;
        stop = (size + 1) / 2;
        break;
    }

    for (int i = start; i < stop; i++)
    {
        int num = atoi(argv[i + 1]);
        printf("PID = %d, length = %d, square = %d\n", getpid(), num, num * num);
    }

    if (pid != 0)
    {
        wait(NULL);
    }

    return 0;
}