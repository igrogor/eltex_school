#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


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

typedef struct btree
{
    Directory value;
    struct btree *left;
    struct btree *right;
} btree;

btree *NewNode(Directory *temp);
void AddinBtree(btree **head, Directory *temp);
void Print_Btree(btree *p);
void DelNode(btree **head, int ID);
void PrintTree (btree *pNode);
void Print_one_person(Directory list);
Directory* InputPerson();
Directory *InputPersonIsNotId();
void Menu(btree **head);
btree *FindMaxinLeft(btree *head);
void Print_Btree(btree *p);

#endif
