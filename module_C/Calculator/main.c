#include <stdio.h>
#include "header.h"

int main()
{
    menu();
    return 0;
}

void menu()
{
    char sim;
    int arg1, arg2, flag;

    while (1)
    {
        printf("input action\n+\t-\t*\t/\n");
        scanf(" %c", &sim);

        printf("input operands\n");
        scanf("%d %d", &arg1, &arg2);

        switch (sim)
        {
        case '+':
            printf("%d\n", addition(arg1, arg2));
            break;
        case '-':
            printf("%d\n", subtraction(arg1, arg2));
            break;
        case '*':
            printf("%d\n", multiplication(arg1, arg2));
            break;
        case '/':
            printf("%f\n", division(arg1, arg2));
            break;

        default:
            printf("error input\n");
            break;
        }
    }
}

int addition(int arg1, int arg2)
{
    return arg1 + arg2;
}

int subtraction(int arg1, int arg2)
{
    return arg1 - arg2;
}

int multiplication(int arg1, int arg2)
{
    return arg1 * arg2;
}

float division(int arg1, int arg2)
{
    if (arg2 == 0)
    {
        printf("error\n");
        exit(1);
    }
    else
    {
        return (float)arg1 / arg2;
    }
}