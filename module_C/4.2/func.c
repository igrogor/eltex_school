#include "header.h"
#include <stdio.h>
#include <stdlib.h>

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
        printf("Priority: %d, Message: %s\n", current->Message.priority, current->Message.string);
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

void dequeue(queue **head, int priority)
{
    queue *temp = *head;
    queue *prev = NULL;

    while (temp != NULL)
    {
        if (temp->Message.priority == priority)
        {
            printf("Get message is a %d priority : %s\n", priority, getMessage(temp).string);
            if (prev != NULL)
            {
                prev->next = temp->next;
            }else{
                (*head) = temp->next;
                temp = temp->next;
            }
            free(temp);
            dequeue(head, priority);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

void dequeue_f(queue **head)
{
    queue *temp = *head;
    (*head) = (*head)->next;
    printf("Get message is a %d priority : %s\n", getMessage(temp).priority, getMessage(temp).string);
    free(temp);
}

void dequeue_b(queue **head, int priority)
{
    queue *temp = *head;
    queue *prev = NULL;

    while (temp != NULL)
    {
        if (temp->Message.priority >= priority)
        {
            printf("Get message is a %d priority : %s\n", getMessage(temp).priority, getMessage(temp).string);
            prev->next = temp->next;
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}


Message getMessage(queue *head)
{
    return head->Message;
}

void free_queue(queue **head)
{
    while (*head != NULL)
    {
        queue *tmp = *head;
        (*head) = (*head)->next;
        free(tmp);
    }
    free(*head);
}

void menu(queue **head)
{
    int flag = 0;
    while (1)
    {
        printf("1 - gen\n2 - get first\n3 - get is priority\n4 - get ...\n5 - print\n");
        scanf("%d", &flag);
        switch (flag)
        {
        case 1:
            int size = 0;
            printf("input num gen\n");
            scanf("%d", &size);
            generate_messages(head, size);
            break;
        case 2:
            dequeue_f(head);
            break;
        case 3:
            int pri = 0;
            printf("input pri\n");
            scanf("%d", &pri);
            dequeue(head, pri);
            break;
        case 4:
            int pri1 = 0;
            printf("input num gen\n");
            scanf("%d", &pri1);
            dequeue_b(head, pri1);
            break;
        case 5:
            printqueue(*head);
            break;
        default:
            break;
        }
    }
}