#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*
Задание 10 (Семафоры POSIX, 2 балла)
Скорректировать программу 05 так, чтобы доступ к файлу регулировался семафором.

*/

int running = 1;
int read_new_number = 0;

void terminate_handler(int sig)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0) exit(EXIT_FAILURE);
    int pipefd[2];
    if (pipe(pipefd)) exit(EXIT_FAILURE);
    pid_t pid = fork();

    sem_t *sem = sem_open("/semaphore", O_CREAT, 0666, 1);

    switch (pid)
    {
        case -1:
            exit(EXIT_FAILURE);
            break;
        case 0:
            close(pipefd[0]);

            signal(SIGTERM, terminate_handler);
            srand(time(NULL) ^ getpid());

            while (running)
            {
                sem_wait(sem);
                int file = open("file.txt", O_RDONLY);

                lseek(file, -sizeof(int), SEEK_END);
                //??
                int a;
                read(file, &a, sizeof(int));

                printf("pid:%d - %d\n", getpid(), a);
                int num = rand() % 1000;
                write(pipefd[1], &num, sizeof(int));
                close(file);
                read_new_number = 0;

                sem_post(sem);

                usleep(100000);
            }
            close(pipefd[1]);
            exit(EXIT_SUCCESS);

            break;
        default:
            close(pipefd[1]);
            sem_wait(sem);

            int file = open("file.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
            int initial_num = 0;
            write(file, &initial_num, sizeof(int));
            close(file);
            sem_post(sem);
            usleep(100000);

            for (int i = 0; i < count; i++)
            {
                int num;
                read(pipefd[0], &num, sizeof(int));
                sem_wait(sem);
                usleep(10000);
                int file1 = open("file.txt", O_WRONLY | O_APPEND);
                write(file1, &num, sizeof(int));
                close(file1);
                sem_post(sem);
                usleep(100000);
            }
            close(pipefd[0]);
            kill(pid, SIGTERM);
            wait(NULL);

            break;
    }

    sem_close(sem);
    sem_unlink("/semaphore");

    return 0;
}