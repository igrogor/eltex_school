#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Задание 11 (Семафоры POSIX, 2 балла)
Скорректировать программу 10 так, чтобы несколько процессов (ограниченное
количество) могли читать из файла. Запись в файл возможна, когда он никем не
читается. */

#define MAX_READERS 3
#define SEM_NAME "/read_slots"

int running = 1;

void terminate_handler(int sig)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0) exit(EXIT_FAILURE);

    sem_unlink(SEM_NAME);
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, MAX_READERS);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd)) exit(EXIT_FAILURE);

    pid_t pid = fork();
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

                int a;
                read(file, &a, sizeof(int));
                printf("pid:%d - %d\n", getpid(), a);

                int num = rand() % 1000;
                write(pipefd[1], &num, sizeof(int));
                close(file);

                sem_post(sem);

                usleep(100000);
            }
            close(pipefd[1]);
            exit(EXIT_SUCCESS);
            break;

        default:
            close(pipefd[1]);

            for (int i = 0; i < MAX_READERS; i++) sem_wait(sem);
            int file = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            int initial_num = 0;
            write(file, &initial_num, sizeof(int));
            close(file);
            for (int i = 0; i < MAX_READERS; i++) sem_post(sem);

            usleep(100000);

            for (int i = 0; i < count; i++)
            {
                int num;
                read(pipefd[0], &num, sizeof(int));

                for (int j = 0; j < MAX_READERS; j++) sem_wait(sem);

                int file1 = open("file.txt", O_WRONLY | O_APPEND);
                write(file1, &num, sizeof(int));
                close(file1);

                for (int j = 0; j < MAX_READERS; j++) sem_post(sem);

                usleep(100000);
            }

            close(pipefd[0]);
            kill(pid, SIGTERM);
            wait(NULL);

            break;
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}