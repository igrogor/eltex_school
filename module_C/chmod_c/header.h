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
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct List
{
    struct Node* head;
    struct Node* tail;
} List;

void printList(Node* head);
void AddinHead(Node** head,  Directory* temp);
Node* NewNode(Directory* temp);

void add_person(Directory *list, int *cur_size, Directory *temp);
void edit_user(int ID, Directory *list, Directory *temp, int *cur_size);
void print_sctuct(Directory *list, int cur_size);
void menu(Directory *list, int *cur_size);
void del(int ID, int *cur_size, Directory *list);

#endif