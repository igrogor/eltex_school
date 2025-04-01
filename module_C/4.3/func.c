#include "header.h"
#include <stdio.h>

btree *NewNode(Directory *temp)
{
    btree *node = (btree *)malloc(sizeof(btree));
    node->valuel = temp;
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
    if ((*head)->valuel->id > temp->id)
    {
        AddinBtree(&(*head)->left, temp);
    }else{
        AddinBtree(&(*head)->right, temp);
    }
}