#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*
Изменить программу 04 так, чтобы дочерний процесс выводил информацию из файла.
Если родительский процесс собирается изменить файл, то он отправляет сигнал
SIGUSR1 (блокировка доступа к файлу). Когда родительский процесс завершил
модификацию файла, он отправляет сигнал SIGUSR2 (разрешение доступа к файлу).
Дочерний процесс отправляет новое число родительскому после того, как прочитал
файл.
*/

int file_locked = 0;
int running = 1;
int read_new_number = 0;

void lock_handler(int sig)
{
    file_locked = 1;
    // printf("locked\n");
}

void unlock_handler(int sig)
{
    file_locked = 0;
    read_new_number = 1;
    // printf("unlocked\n");
}

void terminate_handler(int sig)
{
    running = 0;
    // printf("exit\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2) exit(EXIT_FAILURE);
    int count = atoi(argv[1]);
    if (count <= 0) exit(EXIT_FAILURE);

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
            signal(SIGUSR1, lock_handler);
            signal(SIGUSR2, unlock_handler);
            signal(SIGTERM, terminate_handler);
            srand(time(NULL) ^ getpid());

            while (running)
            {
                int file = open("file.txt", O_RDONLY);
                if (!file_locked && read_new_number)
                {
                    int a;
                    read(file, &a, sizeof(int));
                    printf("pid:%d - %d\n", getpid(), a);
                    int num = rand() % 1000;
                    write(pipefd[1], &num, sizeof(int));
                }
                close(file);
                read_new_number = 0;
                usleep(100000);
            }
            close(pipefd[1]);
            exit(EXIT_SUCCESS);

            break;
        default:
            close(pipefd[1]);
            int file = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            int initial_num = 0;
            write(file, &initial_num, sizeof(int));
            close(file);
            kill(pid, SIGUSR2);
            usleep(100000);

            for (int i = 0; i < count; i++)
            {
                int num;
                read(pipefd[0], &num, sizeof(int));
                kill(pid, SIGUSR1);
                usleep(10000);
                int file1 = open("file.txt", O_WRONLY | O_TRUNC);
                write(file1, &num, sizeof(int));
                close(file1);
                kill(pid, SIGUSR2);
                usleep(100000);
            }
            close(pipefd[0]);
            kill(pid, SIGTERM);
            wait(NULL);
            break;
    }
    return 0;
}