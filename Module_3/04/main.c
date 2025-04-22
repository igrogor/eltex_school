#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0)
        exit(EXIT_FAILURE);

    int pipefd[2];
    if (pipe(pipefd))
        exit(EXIT_FAILURE);

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        exit(EXIT_FAILURE);
        break;
    case 0:
        close(pipefd[0]);
        srand(time(NULL) ^ getpid());
        for (int i = 0; i < count; i++)
        {
            int num = rand() % 1000;
            if (write(pipefd[1], &num, sizeof(int)) == -1)
                exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
        break;
    default:
        close(pipefd[1]);

        int file = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        for (int i = 0; i < count; i++)
        {
            int num;
            if (read(pipefd[0], &num, sizeof(int)) < 0)
                exit(EXIT_FAILURE);
            if (write(file, &num, sizeof(int)) < 0)
                exit(EXIT_FAILURE);
            printf("%d\n", num);
        }
        close(file);
        close(pipefd[0]);
        wait(NULL);
        break;
    }

    return 0;
}
