#ifndef HEADER_H_
#define HEADER_H_

typedef struct Date
{
    int day, month, year;
} Date;

typedef struct Directory
{
    int id;
    char name[20];
    char last_name[20];
    Date date;
} Directory;

typedef struct Node
{
    Directory value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List
{
    struct Node *head;
    struct Node *tail;
} List;

void PrintList(Node *head);
void AddinHead(Node **head, Directory *temp);
Node *NewNode(Directory *temp);
void AddinSort(Node **head, Directory *temp);
void AddinTail(Node **head, Directory *temp);
void DeleteNode(Node **head, int ID);
void EditNode(Node **head, Directory *temp, int ID);
void Print_one_person(Directory list);
Directory* InputPerson();
Directory *InputPersonIsNotId();
void Menu(Node **head);

#endif