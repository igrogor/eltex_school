#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Задание 11 (Семафоры POSIX, 2 балла)
Скорректировать программу 10 так, чтобы несколько процессов (ограниченное
количество) могли читать из файла. Запись в файл возможна, когда он никем не
читается. */

/* Для компиляции make, затем ./main N, где N - число для генерации */


#define PROCESS_COUNT 5      
#define MAX_READERS 3        

int running = 1;

void terminate_handler(int sig)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int write_count = atoi(argv[1]);
    if (write_count <= 0) exit(EXIT_FAILURE);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    sem_t *sem = sem_open("/read_sem", O_CREAT | O_EXCL, 0666, MAX_READERS);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int file = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int initial_value = 0;
    write(file, &initial_value, sizeof(int));
    close(file);

    pid_t pids[PROCESS_COUNT];
    for (int i = 0; i < PROCESS_COUNT; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            close(pipefd[0]); 
            signal(SIGTERM, terminate_handler);
            srand(time(NULL) ^ getpid());

            while (running) {
                sem_wait(sem); 

                int file = open("file.txt", O_RDONLY);
                lseek(file, -sizeof(int), SEEK_END);
                int num;
                read(file, &num, sizeof(int));
                printf("pid:%d — read: %d\n", getpid(), num);
                close(file);

                int new_num = rand() % 1000;
                write(pipefd[1], &new_num, sizeof(int));

                sem_post(sem); 

                usleep(100000);
            }

            close(pipefd[1]);
            sem_close(sem);
            exit(EXIT_SUCCESS);
        } else {
            pids[i] = pid;
        }
    }

    close(pipefd[1]); 
    for (int i = 0; i < write_count; ++i) {
        int value;
        read(pipefd[0], &value, sizeof(int));

        for (int j = 0; j < MAX_READERS; ++j) {
            sem_wait(sem);
        }

        int file = open("file.txt", O_WRONLY | O_APPEND);
        printf("parent — write: %d\n", value);
        write(file, &value, sizeof(int));
        close(file);

        for (int j = 0; j < MAX_READERS; ++j) {
            sem_post(sem);
        }

        usleep(100000);
    }
    close(pipefd[0]);

    for (int i = 0; i < PROCESS_COUNT; ++i) {
        kill(pids[i], SIGTERM);
        waitpid(pids[i], NULL, 0);
    }

    sem_close(sem);
    sem_unlink("/read_sem");

    return 0;
}