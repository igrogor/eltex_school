#include <stdio.h>
#include <stdarg.h>
#define N 10

#include "header.h"

int main()
{
    menu();
    return 0;
}

void menu()
{
    char sim;
    int size;

    while (1)
    {
        printf("input action\n+\t-\t*\t/\n");
        scanf(" %c", &sim);

        printf("input size operands (maximum %d)\n", N - 1);
        scanf(" %d", &size);

        int oper[N];
        printf("input operands\n");
        for (size_t i = 0; i < size; i++)
        {
            scanf("%d", &oper[i]);
        }

        switch (sim)
        {
        case '+':
            printf("%d\n", addition(size, oper[0], oper[1], oper[2], oper[3], oper[4], oper[5], oper[6], oper[7], oper[8], oper[9]));
            break;
        case '-':
            printf("%d\n", subtraction(size, oper[0], oper[1], oper[2], oper[3], oper[4], oper[5], oper[6], oper[7], oper[8], oper[9]));
            break;
        case '*':
            printf("%d\n", multiplication(size, oper[0], oper[1], oper[2], oper[3], oper[4], oper[5], oper[6], oper[7], oper[8], oper[9]));
            break;
        case '/':
            printf("%f\n", division(size, oper[0], oper[1], oper[2], oper[3], oper[4], oper[5], oper[6], oper[7], oper[8], oper[9]));
            break;

        default:
            printf("error input\n");
            break;
        }
    }
}

int addition(int size, ...)
{
    int resilt = 0;
    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++)
    {
        resilt += va_arg(args, int);
    }
    va_end(args);
    return resilt;
}

int subtraction(int size, ...)
{
    va_list args;
    va_start(args, size);

    int resilt = va_arg(args, int);

    for (int i = 0; i < size - 1; i++)
    {
        resilt -= va_arg(args, int);
    }
    va_end(args);
    return resilt;
}

int multiplication(int size, ...)
{
    va_list args;
    va_start(args, size);
    int resilt = va_arg(args, int);

    for (int i = 0; i < size - 1; i++)
    {
        resilt *= va_arg(args, int);
    }
    va_end(args);
    return resilt;
}

float division(int size, ...)
{
    va_list args;
    va_start(args, size);
    int resilt = va_arg(args, int);

    for (int i = 0; i < size -1 ; i++)
    {
        int m = va_arg(args, int);
        if (m == 0) {
            printf("Error: Division by zero\n");
            va_end(args);
            return 0;
        }
        resilt /= m;
    }
    va_end(args);
    return resilt;
}