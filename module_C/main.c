#include "header.h"
#include <stdio.h>
#include <string.h>

int main()
{
    Directory list[20];
    int cur_size = 0;

    list[cur_size].id = 1;
    list[cur_size].date.day = 2;
    list[cur_size].date.month = 3;
    list[cur_size].date.year = 4;
    strncpy(list[cur_size].name, "Egor", 20);
    strncpy(list[cur_size].last_name, "Terentev", 20);
    cur_size++;

    menu(list, &cur_size);

    return 0;
}
