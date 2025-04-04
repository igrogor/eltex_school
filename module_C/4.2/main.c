#include "header.h"
#include <stdio.h>
#include <string.h>

int main()
{

    queue* head = NULL;
    generate_messages(&head, 2000);

    printf("=====================\n");

    printqueue(head);
    return 0;
}


