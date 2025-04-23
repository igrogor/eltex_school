
/* Задание 07 (Очереди сообщений POSIX, 3 балла).
Написать две программы, использующие одну очередь сообщений для двухстороннего
взаимодействия (персональный чат) Сообщения отправляются по очереди (пинг-понг).
Для завершения обмена предусмотреть отправку данных с заранее известным
приоритетом. */

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRIO_1 1
#define PRIO_2 2
#define EX 20

int main()
{
    mqd_t ds;
    struct mq_attr mes = {
        .mq_flags = 0, .mq_maxmsg = 10, .mq_msgsize = 256, .mq_curmsgs = 0};
    int run = 1;
    int priority;

    mq_unlink("/my_queue");

    ds = mq_open("/my_queue", O_CREAT | O_RDWR, 0666, &mes);
    if (ds == (mqd_t)-1)
    {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }
    char message[mes.mq_msgsize];

    while (run)
    {
        printf("prog1: \n");
        fgets(message, mes.mq_msgsize, stdin);

        if (mq_send(ds, message, strlen(message), PRIO_1) == -1)
        {
            perror("failed");
            exit(EXIT_FAILURE);
        }
        printf("Message sent->\n");

        if (strncmp(message, "exit", 4) == 0)
        {
            run = 0;
        }
        if (!run)
        {
            break;
        }

        char buffer[mes.mq_msgsize + 1];

        ssize_t bytes_read = mq_receive(ds, buffer, mes.mq_msgsize, &priority);
        if (bytes_read == -1)
        {
            perror("mq_receive failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';

        if (strncmp(buffer, "exit", 4) == 0)
        {
            run = 0;
        }
        if (!run)
        {
            break;
        }
        printf(" message: %s (priority: %d)\n", buffer, priority);
    }

    mq_close(ds);
    return 0;
}