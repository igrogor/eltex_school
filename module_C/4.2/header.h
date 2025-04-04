#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Message
{
    int priority;
    char *string;
} Message;

typedef struct queue
{
    Message Message;
    struct queue* next;
} queue;

queue *Newqueue(int priority, char *messege);
void enqueue(queue** head, Message Message);
void dequeue(queue** head, int priority);
void printqueue(queue* head);
void free_queue(queue* head);
void generate_messages(queue** head, int count);



#endif


