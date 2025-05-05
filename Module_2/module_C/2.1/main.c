#include <stdio.h>
#include <string.h>

#include "header.h"

int main()
{
    Directory list[20];
    int cur_size = 0;

    list[cur_size].id = 0;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "Egor", 20);
    strncpy(list[cur_size].last_name, "Terentev", 20);
    cur_size++;

    list[cur_size].id = 1;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "max", 20);
    strncpy(list[cur_size].last_name, "rt", 20);
    cur_size++;

    list[cur_size].id = 2;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "dim", 20);
    strncpy(list[cur_size].last_name, "glk", 20);
    cur_size++;

    list[cur_size].id = 9;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "stf", 20);
    strncpy(list[cur_size].last_name, "dfv", 20);
    cur_size++;

    list[cur_size].id = 3;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "stf", 20);
    strncpy(list[cur_size].last_name, "dfv", 20);
    cur_size++;

    list[cur_size].id = 11;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "dim", 20);
    strncpy(list[cur_size].last_name, "glk", 20);
    cur_size++;

    list[cur_size].id = 6;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "dim", 20);
    strncpy(list[cur_size].last_name, "glk", 20);
    cur_size++;

    menu(list, &cur_size);

    return 0;
}
