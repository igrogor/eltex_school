#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*
Задание 09 (Семафоры System V, 2 балла)
Скорректировать программу 08 так, чтобы несколько процессов (ограниченное
количество) могли читать из файла. Запись в файл возможна, когда он никем не
читается.

*/

#define KEY 1234

int running = 1;
int read_new_number = 0;

void terminate_handler(int sig)
{
    running = 0;
}

void sem_lock(int semid)
{
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_unlock(int semid)
{
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main(int argc, char *argv[])
{
    if (argc != 2) exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0) exit(EXIT_FAILURE);

    int pipefd[2];
    if (pipe(pipefd)) exit(EXIT_FAILURE);

    pid_t pid = fork();

    int semid = semget(KEY, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

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
                sem_lock(semid);
                int file = open("file.txt", O_RDONLY);

                int a;
                read(file, &a, sizeof(int));
                printf("pid:%d - %d\n", getpid(), a);
                int num = rand() % 1000;
                write(pipefd[1], &num, sizeof(int));

                close(file);
                read_new_number = 0;

                sem_unlock(semid);
                usleep(100000);
            }
            close(pipefd[1]);
            exit(EXIT_SUCCESS);

            break;
        default:
            close(pipefd[1]);
            sem_lock(semid);
            int file = open("file.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
            int initial_num = 0;
            write(file, &initial_num, sizeof(int));
            close(file);
            sem_unlock(semid);
            usleep(100000);

            for (int i = 0; i < count; i++)
            {
                int num;
                read(pipefd[0], &num, sizeof(int));
                sem_lock(semid);
                usleep(10000);
                int file1 = open("file.txt", O_WRONLY | O_APPEND);
                write(file1, &num, sizeof(int));
                close(file1);
                sem_unlock(semid);
                usleep(100000);
            }
            close(pipefd[0]);
            kill(pid, SIGTERM);
            wait(NULL);

            semctl(semid, 0, IPC_RMID);
            break;
    }
    return 0;
}