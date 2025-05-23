#include <stdio.h>
#include <stdlib.h>

#include "header.h"

queue *Newqueue(int priority, char *message)
{
    queue *new_queue = (queue *)malloc(sizeof(queue));
    new_queue->next = NULL;
    new_queue->Message.priority = priority;
    new_queue->Message.string = strdup(message);
    return new_queue;
}

void enqueue(queue **head, Message Message)
{
    if (Message.priority > 255 || Message.priority < 0)
    {
        printf("Error\n");
        return;
    }
    queue *node = Newqueue(Message.priority, Message.string);
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    queue *current = *head;
    queue *prev = NULL;

    while (current != NULL && current->Message.priority <= Message.priority)
    {
        prev = current;
        current = current->next;
    }
    node->next = current;
    if (prev == NULL)
    {
        *head = node;
    }
    else
    {
        prev->next = node;
    }
}

void printqueue(queue *head)
{
    queue *current = head;
    while (current != NULL)
    {
        printf("Priority: %d, Message: %s\n",
               current->Message.priority,
               current->Message.string);
        current = current->next;
    }
}

void generate_messages(queue **head, int count)
{
    for (int i = 0; i < count; i++)
    {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "message #%d", i);
        Message Message = {rand() % 256, buffer};
        printf(" %d: %s\n", Message.priority, Message.string);
        enqueue(head, Message);
    }
}
