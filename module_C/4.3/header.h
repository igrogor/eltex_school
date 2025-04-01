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
    Directory* valuel;
    struct btree *left;
    struct btree *right;
} btree;


#endif
