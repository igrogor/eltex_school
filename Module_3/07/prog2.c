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
    struct mq_attr mes;
    int priority;
    int run = 1;

    ds = mq_open("/my_queue", O_RDWR);
    if (ds == (mqd_t)-1)
    {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(ds, &mes) == -1)
    {
        perror("mq_getattr failed");
        exit(EXIT_FAILURE);
    }

    char buffer[mes.mq_msgsize + 1];

    while (run)
    {
        ssize_t bytes_read = mq_receive(ds, buffer, mes.mq_msgsize, &priority);
        if (bytes_read == -1)
        {
            perror("mq_receive failed");
            exit(EXIT_FAILURE);
        }
        buffer[bytes_read] = '\0';
        printf("Received message: %s (priority: %d)\n", buffer, priority);
        if (strncmp(buffer, "exit", 4) == 0)
        {
            run = 0;
        }
        if (!run)
        {
            break;
        }

        printf("prog2: \n");
        fgets(buffer, mes.mq_msgsize, stdin);

        if (mq_send(ds, buffer, strlen(buffer), PRIO_2) == -1)
        {
            perror("mq_send failed");
            exit(EXIT_FAILURE);
        }
        printf("Message sent->\n");

        if (strncmp(buffer, "exit", 4) == 0)
        {
            run = 0;
        }
        if (!run)
        {
            break;
        }
    }

    mq_close(ds);
    mq_unlink("/my_queue");
    return 0;
}