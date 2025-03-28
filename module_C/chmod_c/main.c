#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    Directory temp = {3,"fsdsd", "dfgfd", {1, 2, 3}};
    Directory temp2 = {5, "Alice", "Smith", {4, 5, 2023}};
    Node* Head = NULL;


    AddinHead(&Head, &temp);
    AddinHead(&Head, &temp2);
    AddinHead(&Head, &temp);

    printList(Head);
    return 0;
}