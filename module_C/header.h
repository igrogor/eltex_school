#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void add_person(Directory *list, int *cur_size, Directory* temp);
void edit_person();
void print_sctuct(Directory *list, int cur_size);
void menu(Directory *list, int *cur_size);

#endif
