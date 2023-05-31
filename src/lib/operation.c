/*
** Cbank 22/05/2023
** fonctions pour la gestion des opérations bancaires
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_op;

int save_operation()
{
    FILE *file = fopen(OPERATION_CSV, "w");

    int nb=0;
    if (file == NULL)
    {
        printf("Failed to create file: %s\n", OPERATION_CSV);
        exit(1);
    }
    fprintf(file, "id;account_id;date;bank_lib1;bank_lib2;bank_ref;bank_info;bank_type;bank_category;bank_sub_category;category;amount\n"); // Ecriture de l'entête

    Node *n = head_op;
    Operation *op = NULL;
    while (n != NULL)
    {
        nb++;
        op = n->data;
        fprintf(file, "%lu;%s;%ld;%s;%s;%s;%s;%s;%s;%s;%lu;%.2f;\n",
                op->id,
                op->account_id,
                op->date,
                op->bank_lib1,
                op->bank_lib2,
                op->bank_ref,
                op->bank_info,
                op->bank_type,
                op->bank_category,
                op->bank_sub_category,
                op->category_id,
                op->amount);
        n = n->next;
    }

    fclose(file);
    return nb;
}

int load_operation()
{
    FILE *file = fopen(OPERATION_CSV, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", OPERATION_CSV);
        exit(1);
    }

    char line[300];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_op = NULL;
    int nb=0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        Id id;
        char account_id[ACC_ID_LENGTH];
        long date;
        char bank_lib1[OP_LIB_LENGTH];
        char bank_lib2[OP_LIB_LENGTH];
        char bank_ref[OP_LIB_LENGTH];
        char bank_info[OP_LIB_LENGTH];
        char bank_type[OP_LIB_LENGTH];
        char bank_category[OP_LIB_LENGTH];
        char bank_sub_category[OP_LIB_LENGTH];
        Id category_id;
        float amount;

        if (sscanf(line, "%lu;%[^;];%ld;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%lu;%f",
                   &id,
                   account_id,
                   &date,
                   bank_lib1,
                   bank_lib2,
                   bank_ref,
                   bank_info,
                   bank_type,
                   bank_category,
                   bank_sub_category,
                   &category_id,
                   &amount) == 12)
        {

            Operation *op = (Operation *)malloc(sizeof(Operation));
            op->id = id;
            op->date = date;
            strcpy(op->account_id, account_id);
            strcpy(op->bank_lib1, bank_lib1);
            strcpy(op->bank_lib2, bank_lib2);
            strcpy(op->bank_ref, bank_ref);
            strcpy(op->bank_info, bank_info);
            strcpy(op->bank_type, bank_type);
            strcpy(op->bank_category, bank_category);
            strcpy(op->bank_sub_category, bank_sub_category);
            op->category_id = category_id;
            op->amount = amount;

            add_node(&head_op, op);
            nb++;
        }
    }

    fclose(file);
    return nb;
}

Id next_operation_id(void)
{
    Operation *op = NULL;
    Node *n = head_op;
    Id max=0;
    while (n != NULL)
    {
        op = n->data;
        if (max<op->id) 
            max=op->id; 
        n = n->next;
    }
    return ++max;
}

void print_operations()
{
    Operation *op = NULL;
    Node *n = head_op;
    while (n != NULL)
    {
        op = n->data;

        printf("%lu;%s;%ld;%s;%s;%s;%s;%s;%s;%s;%lu;%.2f;\n", 
            op->id,
            op->account_id,
            op->date,
            op->bank_lib1,
            op->bank_lib2,
            op->bank_ref,
            op->bank_info,
            op->bank_type,
            op->bank_category,
            op->bank_sub_category,
            op->category_id,
            op->amount);
        n = n->next;
    }
}
