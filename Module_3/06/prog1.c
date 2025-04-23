#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* Задание 06 (Очереди сообщений System V, 3 балла).
Написать две программы, использующие одну очередь сообщений для двухстороннего взаимодействия (персональный чат).
Сообщения отправляются по очереди (пинг-понг).
Для завершения обмена предусмотреть отправку данных с заранее известным приоритетом. */

#define BUFFER_SIZE 1024
#define TYPE1 1
#define TYPE2 2
#define EXIT_TYPE 999

struct msg_buffer
{
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

int main()
{
    key_t key = ftok("prog", 100);
    int id = msgget(key, 0666 | IPC_CREAT);
    int run = 1;
    struct msg_buffer msg;

    while (run)
    {
        printf("Prog_1:\n");
        fgets(msg.msg_text, BUFFER_SIZE, stdin);
        msg.msg_type = TYPE1;

        if (strncmp(msg.msg_text, "exit", 4) == 0)
        {
            msgsnd(id, &msg, sizeof(msg), 0);
            run = 0;
        }
        else
        {
            msgsnd(id, &msg, sizeof(msg), 0);
        }

        if (!run)
            break;

        msgrcv(id, &msg, sizeof(msg), 0, 0);
        if (msg.msg_type == EXIT_TYPE)
        {
            printf("exit\n");
            run = 0;
        }
        else if (msg.msg_type == TYPE2)
        {
            printf("new message from Client 2: %s\n", msg.msg_text);
        }
    }
    msgctl(id, IPC_RMID, NULL);
    return 0;
}