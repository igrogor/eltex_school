#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
    struct msg_buffer msg;
    int run = 1;

    while (run)
    {
        msgrcv(id, &msg, sizeof(msg), 0, 0);

        if (msg.msg_type == EXIT_TYPE)
        {
            printf("exit\n");
            run = 0;
        }
        else if (msg.msg_type == TYPE1)
        {
            printf("new message from Client 1: %s\n", msg.msg_text);

            if (strncmp(msg.msg_text, "exit", 4) == 0)
            {
                msg.msg_type = EXIT_TYPE;
                msgsnd(id, &msg, sizeof(msg.msg_text), 0);
                run = 0;
                break;
            }
            else
            {
                printf("Program2: ");
                fgets(msg.msg_text, BUFFER_SIZE, stdin);
                msg.msg_type = TYPE2;
                msgsnd(id, &msg, sizeof(msg.msg_text), 0);
            }
        }
    }

    return 0;
}