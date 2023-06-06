/*
** Cbank 22/05/2023
** fonctions pour la gestion des comptes bancaires
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_acc;
extern Node *head_op;

int save_account()
{
    int nb = 0;
    FILE *file = fopen(ACCOUNT_CSV, "w");

    if (file == NULL)
    {
        printf("Failed to create file: %s\n", ACCOUNT_CSV);
        exit(1);
    }
    fprintf(file, "id;name;balance\n"); // Ecriture de l'entête

    Node *n = head_acc;
    Account *acc = NULL;
    while (n != NULL)
    {
        acc = n->data;
        fprintf(file, "%s;%s;%.2f\n", acc->id, acc->name, acc->balance);
        nb++;
        n = n->next;
    }

    fclose(file);
    return nb;
}

int load_account()
{
    int nb = 0;
    FILE *file = fopen(ACCOUNT_CSV, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", ACCOUNT_CSV);
        exit(1);
    }

    char line[100];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_acc = NULL;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char id[ACC_ID_LENGTH], name[ACC_NAME_LENGTH];
        float balance;

        if (sscanf(line, "%[^;];%[^;];%f", id, name, &balance) == 3)
        {
            Account *acc = (Account *)malloc(sizeof(Account));
            strcpy(acc->id, id);
            strcpy(acc->name, name);
            acc->balance = balance;
            nb++;
            add_node(&head_acc, acc);
        }
    }

    fclose(file);
    return nb;
}

Account *find_acc_by_id(char *acc_id)
{
    Account *acc = NULL;
    Node *n = head_acc;

    while (n != NULL)
    {
        acc = n->data;
        if (strcmp(acc->id, acc_id) == 0)
            return acc;
        n = n->next;
    }
    return NULL;
}

// Calcule le solde du compte

float get_account_balance(Account *acc)
{
    Operation *op = NULL;
    Node *n = head_op;
    float balance = acc->balance;
    while (n != NULL)
    {
        op = n->data;
        if (strcmp(op->account_id, acc->id) == 0)
            balance += op->amount;
        n = n->next;
    }
    return balance;
}

// Calcule la balance générale

float get_balance()
{
    Account *acc = NULL;
    Node *n = head_acc;
    float balance = 0;
    while (n != NULL)
    {
        acc = n->data;
        balance += get_account_balance(acc);
        n = n->next;
    }
    return balance;
}