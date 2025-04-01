#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *NewNode(Directory *temp)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = *temp;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void AddinHead(Node **head, Directory *temp)
{
    Node *node = NewNode(temp);
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    node->next = *head;
    (*head)->prev = node;
    (*head) = node;
}

void AddinTail(Node **head, Directory *temp)
{
    Node *node = NewNode(temp);

    if (*head == NULL)
    {
        *head = node;
        return;
    }

    Node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = node;
    node->prev = current;
}

void PrintList(Node *head)
{
    printf("id\tday\tmonth\tyear\tname\tlast_name\n");

    while (head != NULL)
    {
        Print_one_person(head->value);
        head = head->next;
    }
    printf("\n");
}

void AddinSort(Node **head, Directory *temp)
{
    if (temp->id < 0)
    {
        printf("Error!\n");
        return;
    }
    Node *node = NewNode(temp);
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    if (temp->id == 0)
    {
        AddinTail(head, temp);
    }
    else
    {
        if (node->value.id > (*head)->value.id)
        {
            node->next = *head;
            (*head)->prev = node;
            (*head) = node;
        }
        else
        {
            Node *current = (*head);
            while (node->value.id < current->next->value.id)
            {
                current = current->next;
            }
            node->next = current->next;
            if (current->next != NULL)
                current->next->prev = node;
            node->prev = current;
            current->next = node;
        }
    }
}

void DeleteNode(Node **head, int ID)
{
    if (*head == NULL)
    {
        printf("Error\n");
        return;
    }
    Node *tmp = *head;
    if ((*head)->value.id == ID)
    {
        (*head) = (*head)->next;
        free(tmp);
    }
    else
    {
        while (ID != tmp->value.id)
        {
            tmp = tmp->next;
        }
        tmp->prev->next = tmp->next;
        if (tmp->next != NULL)
            tmp->next->prev = tmp->prev;
        free(tmp);
    }
}

void EditNode(Node **head, Directory *temp, int ID)
{
    if (*head == NULL)
    {
        printf("Error\n");
        return;
    }
    Node *current = *head;
    while (current->value.id != ID)
    {
        current = current->next;
    }
    strcpy(current->value.name, temp->name);
    strcpy(current->value.last_name, temp->last_name);
    current->value.date = temp->date;
}

void Print_one_person(Directory list)
{
    printf("%d\t%d\t%d\t%d\t%s\t%s\n", list.id, list.date.day, list.date.month, list.date.year, list.name, list.last_name);
}

void Menu(Node **head)
{
    while (1)
    {
        printf("input options:\n");
        printf("1-add person\n2-delete person\n3-edit person\n4-print list\n>>");
        int flag = 0;
        scanf("%d", &flag);
        switch (flag)
        {
        case 1:
            Directory *temp1 = InputPerson();
            AddinSort(head, temp1);
            free(temp1);
            break;
        case 2:
            printf("input ID:\n");
            int ID1;
            scanf("%d", &ID1);
            DeleteNode(head, ID1);
            break;
        case 3:
            printf("input ID:\n");
            int ID2;
            scanf("%d", &ID2);
            Directory *temp = InputPersonIsNotId();
            EditNode(head, temp, ID2);
            free(temp);
            break;
        case 4:
            PrintList(*head);
            break;
        default:
            printf("Error input");
            break;
        }
    }
}

Directory *InputPerson()
{
    Directory *temp = (Directory *)malloc(sizeof(Directory));
    printf("input: ID\tName\tLast Name\tDay\tMonth\tYear\n");
    scanf("%d %s %s %d %d %d", &temp->id, temp->name, temp->last_name, &temp->date.day, &temp->date.month, &temp->date.year);
    return temp;
}

Directory *InputPersonIsNotId()
{
    Directory *temp = (Directory *)malloc(sizeof(Directory));
    printf("input:\tName\tLast Name\tDay\tMonth\tYear\n");
    scanf("%s %s %d %d %d", temp->name, temp->last_name, &temp->date.day, &temp->date.month, &temp->date.year);
    return temp;
} 