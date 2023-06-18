#include <stdio.h>
#include <stdlib.h>
#include "libcbank.h"

void add_node(Node **head, void *data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Failed to allocate memory for new node.\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

void free_list(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

long int nb_node(Node *head)
{
    long int i = 0;
    Node *current = head;
    while (current != NULL)
    {
        Node *next = current->next;
        i++;
        current = next;
    }
    return i;
}