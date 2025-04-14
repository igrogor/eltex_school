#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define N 256
#define M 30

/*
Написать программу, похожую на командный интерпретатор.
При запуске выводится приглашение, и пользователь вводит имя и аргументы программы, которую желает запустить.
Программы для запуска написать самостоятельно и поместить в тот же каталог (например, программа для вычисления суммы аргументов, «склеивания» строк, поиска наибольшего значения или наибольшей длины строки и т.д.).

*/

int main()
{
    while (1)
    {
        char input[N];
        printf("\n> ");
        if (!fgets(input, N, stdin))
        {
            return 1;
        }
        char *args[64];
        int arg_count = 0;

        input[strcspn(input, "\n")] = '\0';

        char *token = strtok(input, " ");
        while (token != NULL && arg_count < M)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            perror("error");
            break;
        case 0:
            execv(args[0], args);
            break;
        default:
            waitpid(pid, NULL, 0);
            break;
        }
    }

    return 0;
}