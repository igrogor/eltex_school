#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Задание 12 (Разделяемая память System V, 3 балла)
Родительский процесс генерирует наборы из случайного количества случайных чисел
и помещает в разделяемую память. Дочерний процесс находит максимальное и
минимальное число и также помещает их в разделяемую память, после чего
родительский процесс выводит найденные значения на экран. Процесс повторяется до
получения сигнала SIGINT, после чего выводится количество обработанных наборов
данных. */

#define MAX_SIZE 100
#define MAX_NUMBER 10000

volatile sig_atomic_t running = 1;

struct Pac
{
    int size;
    int num[MAX_SIZE];
    int max, min;
    int count;
} typedef Pac;

void clear(Pac *pac)
{
    pac->max = 0;
    pac->min = 0;
    pac->size = 0;
}

void sigint_handler(int sig)
{
    running = 0;
}



int main()
{
    srand(time(NULL));
    signal(SIGINT, sigint_handler);
    sem_t *sem = sem_open("/semaphore_1", O_CREAT|O_EXCL, 0666, 0);
    sem_t *sem_0 = sem_open("/semaphore_2", O_CREAT|O_EXCL, 0666, 0);

    int shmid = shmget(IPC_PRIVATE, sizeof(Pac), 0666 | IPC_CREAT);
    Pac *object = (Pac *)shmat(shmid, NULL, 0);
    __pid_t pid = fork();

    switch (pid)
    {
        case -1:
            exit(EXIT_FAILURE);
            break;
        case 0:
            while (running)
            {
                sem_wait(sem_0);
                printf("start find ---------------------\n");

                if (!running) break;

                int min = object->num[0];
                int max = object->num[0];
                for (int i = 1; i < object->size; i++)
                {
                    if (object->num[i] < min) min = object->num[i];
                    if (object->num[i] > max) max = object->num[i];
                }
                object->min = min;
                object->max = max;

                sem_post(sem);
                
                printf("stop find ---------------------\n");
                usleep(10000000);

            }
            shmdt(object);
            exit(EXIT_SUCCESS);
        default:
            while (running)
            {
                printf("start gen\n");

                int size = (rand() % MAX_SIZE) + 1;
                object->size = size;
                for (int i = 0; i < size; i++)
                {
                    object->num[i] = rand() % MAX_NUMBER;
                }

                printf("stop gen\n");

                sem_post(sem_0);
                sem_wait(sem);

                printf("start print\n");

                printf("max - %d\nmin - %d\n", object->max, object->min);
                object->count++;

                printf("stop print\n");
                clear(object);
                usleep(10000000);
            }
            sem_post(sem_0); // Разблокируем дочерний процесс

            if (wait(NULL) == -1)
            {
                perror("wait");
                exit(EXIT_FAILURE);
            }

            printf("Total sets processed: %d\n", object->count);
            sem_close(sem);
            sem_close(sem_0);
            sem_unlink("/semaphore_1");
            sem_unlink("/semaphore_2");
            shmdt(object);
            shmctl(shmid, IPC_RMID, NULL);
            break;
    }

    return 0;
}
