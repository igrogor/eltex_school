#include <stdio.h>
#include <string.h>

#include "header.h"

int main()
{
    btree* head = NULL;

    Directory temp1 = {8, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp2 = {1, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp3 = {12, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp4 = {1, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp5 = {6, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp6 = {10, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp7 = {14, "sdffd", "sdfs", {2, 3, 4}};

    Directory temp8 = {4, "sdffd", "sdfs", {2, 3, 4}};

    // Directory temp9 = {7, "sdffd", "sdfs", {2, 3, 4}};

    AddinBtree(&head, &temp1);
    AddinBtree(&head, &temp2);
    AddinBtree(&head, &temp3);
    AddinBtree(&head, &temp4);
    AddinBtree(&head, &temp5);
    AddinBtree(&head, &temp6);
    AddinBtree(&head, &temp7);
    AddinBtree(&head, &temp8);
    // //AddinBtree(&head,&temp9);

    printf("\t%d\n", head->value.id);
    printf("%d\t\t%d\n", head->right->value.id, head->left->value.id);

    Menu(&head);
    btree* max = FindMaxinLeft(head);
    printf("%d", max->value.id);

    // Print_Btree(head);

    // DelNode(&head, 12);

    // //Print_Btree(head);
    // PrintTree(head, 15);

    return 0;
}