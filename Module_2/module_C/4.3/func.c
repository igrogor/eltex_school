#include <stdio.h>

#include "header.h"

btree *NewNode(Directory *temp)
{
    btree *node = (btree *)malloc(sizeof(btree));
    node->value = *temp;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void AddinBtree(btree **head, Directory *temp)
{
    btree *node = NewNode(temp);
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    if ((*head)->value.id > temp->id)
    {
        AddinBtree(&(*head)->left, temp);
    }
    else
    {
        AddinBtree(&(*head)->right, temp);
    }
}

void DelNode(btree **head, int ID)
{
    if (*head == NULL)
    {
        return;
    }
    if ((*head)->value.id == ID)
    {
        btree *tmp = FindMaxinLeft(*head);

        (*head)->value = tmp->right->value;
        printf("%d", tmp->value.id);
        // (*head)->left = tmp->left;
        // (*head)->right = tmp->right;
        free(tmp->right);
        tmp->right = NULL;
    }
    else
    {
        if ((*head)->value.id > ID)
        {
            DelNode(&(*head)->left, ID);
        }
        else
        {
            DelNode(&(*head)->right, ID);
        }
    }
}

btree *FindMaxinLeft(btree *head)
{
    btree *max = NULL;
    max = head;
    max = max->left;
    while (max->right->right != NULL)
    {
        max = max->right;
    }
    return max;
}

void EditNode(btree **head, int ID, Directory *temp)
{
    if (*head == NULL)
    {
        return;
    }
    if ((*head)->value.id == ID)
    {
        (*head)->value = *temp;
    }
    else
    {
        if ((*head)->value.id > ID)
        {
            EditNode(&(*head)->left, ID, temp);
        }
        else
        {
            EditNode(&(*head)->right, ID, temp);
        }
    }
}

void Print_Btree(btree *p)
{
    if (p == NULL) return;
    Print_Btree(p->left);
    printf("%d\t", p->value.id);

    // Print_one_person(p->value);
    Print_Btree(p->right);
    printf("\n");
}

void PrintTreeRecursive(btree *node, int level)
{
    if (node == NULL) return;

    PrintTreeRecursive(node->right, level + 1);

    for (int i = 0; i < level; i++) printf("   ");

    printf("%d\n", node->value.id);

    PrintTreeRecursive(node->left, level + 1);
}

void PrintTree(btree *root)
{
    printf("Дерево:\n");
    PrintTreeRecursive(root, 0);
    printf("\n");
}

void Print_one_person(Directory list)
{
    printf("%d\t%d\t%d\t%d\t%s\t%s",
           list.id,
           list.date.day,
           list.date.month,
           list.date.year,
           list.name,
           list.last_name);
}

void Menu(btree **head)
{
    while (1)
    {
        printf("input options:\n");
        printf(
            "1-add person\n2-delete person\n3-edit person\n4-print list\n>>");
        int flag = 0;
        scanf("%d", &flag);
        switch (flag)
        {
            case 1:
                Directory *temp1 = InputPerson();
                AddinBtree(head, temp1);
                free(temp1);
                break;
            case 2:
                printf("input ID:\n");
                int ID1;
                scanf("%d", &ID1);
                DelNode(head, ID1);
                break;
            case 3:
                printf("input ID:\n");
                int ID2;
                scanf("%d", &ID2);
                Directory *temp = InputPersonIsNotId();
                temp->id = ID2;
                EditNode(head, ID2, temp);
                free(temp);
                break;
            case 4:
                PrintTree(*head);
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
    scanf("%d %s %s %d %d %d",
          &temp->id,
          temp->name,
          temp->last_name,
          &temp->date.day,
          &temp->date.month,
          &temp->date.year);
    return temp;
}

Directory *InputPersonIsNotId()
{
    Directory *temp = (Directory *)malloc(sizeof(Directory));
    printf("input:\tName\tLast Name\tDay\tMonth\tYear\n");
    scanf("%s %s %d %d %d",
          temp->name,
          temp->last_name,
          &temp->date.day,
          &temp->date.month,
          &temp->date.year);
    return temp;
}
