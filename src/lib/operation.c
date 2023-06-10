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

    int nb = 0;
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
        fprintf(file, "%lu;%s;%ld;%s;%s;%s;%s;%s;%s;%s;%d;%.2f;\n",
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
    int nb = 0;
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
        int category_id;
        float amount;

        if (sscanf(line, "%lu;%[^;];%ld;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d;%f",
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
    Id max = 0;
    while (n != NULL)
    {
        op = n->data;
        if (max < op->id)
            max = op->id;
        n = n->next;
    }
    return ++max;
}

// Affiche une opération

void print_operation(Operation *op)
{
    printf("Id        : %lu\n", op->id);
    printf("Compte    : %s\n", op->account_id);
    printf("Date      : %ld\n", op->date);
    printf("Libelle 1 : %s\n", op->bank_lib1);
    printf("Libelle 2 : %s\n", op->bank_lib2);
    printf("Référence : %s\n", op->bank_ref);
    printf("Info 1    : %s\n", op->bank_info);
    printf("Type      : %s\n", op->bank_type);
    printf("Info 2    : %s\n", op->bank_category);
    printf("Info 3    : %s\n", op->bank_sub_category);
    if (op->category_id > 0)
        printf("Catégorie : %s\n", find_category_by_id(op->category_id)->name);
    else
        printf("Catégorie : Non classée !!\n");
    printf("Montant   : %.2f\n", op->amount);
}

// Affiche un résumé en ligne de l'opération

void print_operation_light(Operation *op)
{
    printf("%5ld %-5s %s %7.0f %s\n", op->id,op->account_id, fmt_date(op->date), op->amount, op->bank_lib1);
}

// Affiche toutes les operations

int print_operations(char info_type)
{
    int nb = 0;
    Operation *op = NULL;

    // 'b' : affichage des opérations non classées
    // 0 : catégorie "Non classée"
    Node *n = (info_type == 'n') ? find_ops_by_cat_id(head_op, 0) : head_op;

    if (info_type == 'n') // pour les opérations non classée on a besoin des mots cle
        info_type = 'k';
    while (n != NULL)
    {
        op = n->data;
        print_operation(op);
        nb++;
        if (info_type == 'k') // affichage des mots cle
        {
            Node *nkw = extract_kw_op(op);
            printf("Mots cles : ");
            Keyword *kw = NULL;
            while (nkw != NULL)
            {
                kw = nkw->data;
                printf("%s ", kw->name);
                nkw = nkw->next;
            }
            free_list(nkw);
            printf("\n");
        }
        printf("----\n");
        n = n->next;
    }
    return nb;
}

// vérifie si l'opération n'esite pas déjà, 1=doublon 0=pas de doublon

int chk_duplicate_operation(Operation *op_curr)
{
    Operation *op = NULL;
    Node *n = head_op;
    while (n != NULL)
    {
        op = n->data;
        if (op_curr->amount == op->amount && op_curr->date == op->date &&
            strcmp(op_curr->account_id, op->account_id) == 0 &&
            strcmp(op_curr->bank_lib1, op->bank_lib1) == 0 &&
            strcmp(op_curr->bank_lib2, op->bank_lib2) == 0 &&
            strcmp(op_curr->bank_ref, op->bank_ref) == 0 &&
            strcmp(op_curr->bank_info, op->bank_info) == 0)
            return 1;
        n = n->next;
    }
    return 0;
}

// Retourne la liste des operations d'une categorie donnée
// cat_id : id de cartégorie recherché
// hop : premier élément de liste
// retour : une nouvelle liste filtrée

Node *find_ops_by_cat_id(Node *hop, int cat_id)
{
    Operation *op = NULL;
    Node *n = hop;
    Node *ops = NULL;

    while (n != NULL)
    {
        op = n->data;
        if (op->category_id == cat_id)
            add_node(&ops, op);
        n = n->next;
    }
    return ops;
}

// retourne la dernière opération

Operation *get_last_op()
{
    Operation *op = NULL;
    Operation *lst_op = NULL;
    Node *n = head_op;
    long max = 0;
    while (n != NULL)
    {
        op = n->data;
        if (max < op->date)
        {
            max = op->date;
            lst_op = op;
        }
        n = n->next;
    }
    return lst_op;
}

// tri les opérations par date

Node *sort_operation_by_date(Node *head)
{
    if (head == NULL || head->next == NULL)
    {
        // Empty list or single node, already sorted
        return head;
    }

    // Perform a simple bubble sort on the linked list
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            Operation *op1 = ptr1->data;
            Operation *op2 = ptr1->next->data;
            if (op1->date > op2->date)
            {
                // Swap the nodes
                Node *temp = ptr1->next;
                ptr1->next = ptr1->next->next;
                temp->next = ptr1;
                if (ptr1 == head)
                {
                    head = temp;
                }
                else
                {
                    Node *prev = head;
                    while (prev->next != ptr1)
                    {
                        prev = prev->next;
                    }
                    prev->next = temp;
                }
                ptr1 = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    return head;
}