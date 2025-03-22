#include "header.h"
#include <stdio.h>

void print_sctuct(Directory *list, int cur_size)
{
    printf("#######################################################################################\n");
    printf("id\tday\tmonth\tyear\tname\tlast_name\n");
    for (int i = 0; i < cur_size; i++)
    {
        printf("%d\t%d\t%d\t%d\t%s\t%s\n", list[i].id, list[i].date.day, list[i].date.month, list[i].date.year, list[i].name, list[i].last_name);
    }
    printf("#######################################################################################\n");
}

void menu(Directory *list, int *cur_size)
{
    int flag = 0;
    while (flag != 4)
    {
        printf("1 : add person\n2 : edit person\n3 : print add person\n4 : exit\n");
        printf("size %d\n", *cur_size);
        scanf("%d", &flag);
        switch (flag)
        {
        case 1:
            Directory *temp = malloc(sizeof(Directory));
            printf("id\tday\tmonth\tyear\tname\tlast name\n");

            scanf("%d %d %d %d %s %s", &temp->id, &temp->date.day, &temp->date.month, &temp->date.year, temp->name, temp->last_name);
            add_person(list, cur_size, temp);
            free(temp);
            break;
        case 2:
            // edit_person();
            break;
        case 3:
            print_sctuct(list, *cur_size);
            break;
        case 4:
            break;
        default:
            printf("error\n");
            break;
        }
    }
}

void add_person(Directory *list, int *cur_size, Directory* temp)
{
    list[*cur_size].id = temp->id;
    list[*cur_size].date.day = temp->date.day;
    list[*cur_size].date.month = temp->date.month;
    list[*cur_size].date.year = temp->date.year;
    strcpy(list[*cur_size].name, temp->name);
    strcpy(list[*cur_size].last_name, temp->last_name);

    (*cur_size)++;
}
