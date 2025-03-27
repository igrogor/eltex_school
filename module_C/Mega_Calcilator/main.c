#include <stdio.h>
#include "header.h"

int main()
{
    menu();
    return 0;
}

void menu()
{
    
    Func operations[4] = {{"addition", addition}, {"subtraction", subtraction}, {"multiplication", multiplication}, {"division", division}};

    while (1)
    {
        int flag, agr1, arg2;
        for (size_t i = 0; i < sizeof(operations)/sizeof(operations[0]); i++)
        {
            printf("%d - %s\n", i, operations[i]);
        }
        scanf(" %d", &flag);
        printf("input 2 arg\n");
        scanf(" %d %d", &agr1, &arg2);
        printf("%d\n", operations[flag].function(agr1, arg2));
        
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

int division(int arg1, int arg2)
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