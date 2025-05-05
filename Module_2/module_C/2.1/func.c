#include <stdio.h>

#include "header.h"

void print_sctuct(Directory *list, int cur_size)
{
    printf(
        "######################################################################"
        "##"
        "###############\n");
    printf("id\tday\tmonth\tyear\tname\tlast_name\n");
    for (int i = 0; i < cur_size; i++)
    {
        printf("%d\t%d\t%d\t%d\t%s\t%s\n",
               list[i].id,
               list[i].date.day,
               list[i].date.month,
               list[i].date.year,
               list[i].name,
               list[i].last_name);
    }
    printf(
        "######################################################################"
        "##"
        "###############\n");
}

void menu(Directory *list, int *cur_size)
{
    int flag = 0;
    while (flag != 5)
    {
        printf(
            "1 : add person\n2 : edit person\n3 : print add person\n4 : "
            "delete\n5 "
            ": exit\n");
        printf("size %d\n", *cur_size);
        scanf("%d", &flag);
        switch (flag)
        {
            case 1:
                Directory *temp = malloc(sizeof(Directory));
                printf("id\tday\tmonth\tyear\tname\tlast name\n");

                scanf("%d %d %d %d %s %s",
                      &temp->id,
                      &temp->date.day,
                      &temp->date.month,
                      &temp->date.year,
                      temp->name,
                      temp->last_name);
                add_person(list, cur_size, temp);
                free(temp);
                break;
            case 2:
                printf("input ID user\n");
                int ID1;
                scanf("%d", &ID1);
                Directory *temp1 = malloc(sizeof(Directory));
                printf("id\tday\tmonth\tyear\tname\tlast name\n");
                scanf("%d%d %d %d %s %s",
                      &temp1->id,
                      &temp1->date.day,
                      &temp1->date.month,
                      &temp1->date.year,
                      temp1->name,
                      temp1->last_name);
                edit_user(ID1, list, temp1, cur_size);
                free(temp1);

                break;
            case 3:
                print_sctuct(list, *cur_size);
                break;
            case 4:
                printf("input ID user\n");
                int ID;
                scanf("%d", &ID);
                del(ID, cur_size, list);
                break;
            case 5:
                break;
            default:
                printf("error\n");
                break;
        }
    }
}

void add_person(Directory *list, int *cur_size, Directory *temp)
{
    list[*cur_size].id = temp->id;
    list[*cur_size].date.day = temp->date.day;
    list[*cur_size].date.month = temp->date.month;
    list[*cur_size].date.year = temp->date.year;
    strcpy(list[*cur_size].name, temp->name);
    strcpy(list[*cur_size].last_name, temp->last_name);

    (*cur_size)++;
}

void del(int ID, int *cur_size, Directory *list)
{
    int pos = 0;
    for (size_t i = 0; i < *cur_size; i++)
    {
        if (list[i].id == ID)
        {
            break;
        }
        pos++;
    }

    for (size_t i = pos; i < *cur_size - 1; i++)
    {
        list[i] = list[i + 1];
    }
    list[*cur_size - 1].id = 0;
    list[*cur_size - 1].date.day = 0;
    list[*cur_size - 1].date.month = 0;
    list[*cur_size - 1].date.year = 0;
    memset(list[*cur_size - 1].name, 0, sizeof(list[*cur_size - 1].name));
    memset(list[*cur_size - 1].last_name,
           0,
           sizeof(list[*cur_size - 1].last_name));

    (*cur_size)--;
}

void edit_user(int ID1, Directory *list, Directory *temp, int *cur_size)
{
    int pos = 0;
    for (size_t i = 0; i < *cur_size; i++)
    {
        if (list[i].id == ID1)
        {
            break;
        }
        pos++;
    }

    list[pos].id = temp->id;
    list[pos].date.day = temp->date.day;
    list[pos].date.month = temp->date.month;
    list[pos].date.year = temp->date.year;
    strcpy(list[pos].name, temp->name);
    strcpy(list[pos].last_name, temp->last_name);
}