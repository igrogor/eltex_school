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

/* Для компиляции make, затем ./main N, где N - число для генерации */

#define KEY 1234
#define MAX 3
#define PROCESS 5

int running = 1;

void terminate_handler(int sig)
{
    running = 0;
}

void reader_start(int semid)
{
    struct sembuf sb[] = {
        {1, -1, SEM_UNDO}, {2, -1, SEM_UNDO}, {0, 1, SEM_UNDO}};
    semop(semid, sb, 3);

    struct sembuf sb2 = {1, 1, SEM_UNDO};
    semop(semid, &sb2, 1);

    printf("pid:%d — start read\n", getpid());
    usleep(500000);
}

void reader_stop(int semid)
{
    struct sembuf sb[] = {{0, -1, SEM_UNDO}, {2, 1, SEM_UNDO}};
    semop(semid, sb, 2);

    printf("pid:%d — stop read\n", getpid());
    usleep(500000);
}

void writer_start(int semid)
{
    struct sembuf sb[] = {{1, -1, SEM_UNDO}, {0, 0, SEM_UNDO}};
    semop(semid, sb, 2);

    printf("pid:%d — start write\n", getpid());
    usleep(500000);
}

void writer_stop(int semid)
{
    struct sembuf sb[] = {{1, 1, SEM_UNDO}};
    semop(semid, sb, 1);

    printf("pid:%d — stop write\n", getpid());
    usleep(500000);
}

int main(int argc, char *argv[])
{
    if (argc != 2) exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0) exit(EXIT_FAILURE);
    // printf("start not fork\n");
    int pipefd[2];
    if (pipe(pipefd)) exit(EXIT_FAILURE);

    int semid = semget(KEY, 3, IPC_CREAT | 0666);

    semctl(semid, 0, SETVAL, 0);  // для читателей
    semctl(semid, 1, SETVAL, 1);  // для писателя 0 или 1
    semctl(semid, 2, SETVAL, MAX);  // для максимального колличества

    int file = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int initial_value = 0;
    write(file, &initial_value, sizeof(int));
    close(file);

    pid_t pid[PROCESS];
    // printf("start fork\n");

    for (int i = 0; i < PROCESS; i++)
    {
        pid[i] = fork();

        if (pid[i] == -1)
        {
            exit(EXIT_FAILURE);
        }
        if (pid[i] == 0)
        {
            // printf("child pid start\n");

            close(pipefd[0]);
            signal(SIGTERM, terminate_handler);
            srand(time(NULL) ^ getpid());

            while (running)
            {
                reader_start(semid);
                // printf("start read\n");

                int file = open("file.txt", O_RDONLY);
                lseek(file, -sizeof(int), SEEK_END);

                int a;
                read(file, &a, sizeof(int));
                printf("pid:%d - %d\n", getpid(), a);
                int num = rand() % 1000;
                write(pipefd[1], &num, sizeof(int));

                close(file);

                reader_stop(semid);
                // printf("stop read\n");

                usleep(100000);
            }
            // close(pipefd[1]);
            // printf("child pid stop\n");
            exit(EXIT_SUCCESS);
        }
    }
    close(pipefd[1]);
    usleep(100000);
    // printf("perent pid start\n");
    for (int i = 0; i < count; i++)
    {
        int num;
        read(pipefd[0], &num, sizeof(int));
        writer_start(semid);
        // printf("start writer\n");

        usleep(10000);
        int file1 = open("file.txt", O_WRONLY | O_APPEND);
        write(file1, &num, sizeof(int));
        close(file1);
        writer_stop(semid);
        // printf("stop writer\n");

        usleep(100000);
    }
    close(pipefd[0]);
    // printf("perent pid stop\n");

    for (int i = 0; i < PROCESS; i++)
    {
        kill(pid[i], SIGTERM);
        wait(NULL);
    }

    semctl(semid, 0, IPC_RMID, 0);

    return 0;
}