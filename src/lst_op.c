/*
** Cbank 22/05/2023
** Lecture du fichier des opérations
** lst_op : LiST_OPeration
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_op;

// Générateur de données de test

void gen_fake_opp()
{
    Operation *op1 = (Operation *)malloc(sizeof(Operation));
    op1->id = 1;
    op1->date = 20230101;
    strcpy(op1->account_id, "PSBPN");
    strcpy(op1->bank_lib1, "COTISATIONS BANCAIRES");
    strcpy(op1->bank_lib2, "COTIS Conv serv Affinea");
    strcpy(op1->bank_ref, "0001527");
    strcpy(op1->bank_info, "XCCNV999 2023052600001527000001-CONTRAT CNV0000594707");
    strcpy(op1->bank_type, "Frais bancaires");
    strcpy(op1->bank_category, "Banque et assurances");
    strcpy(op1->bank_sub_category, "Frais bancaires");
    op1->category_id = 0;
    op1->amount = -20.12;

    Operation *op2 = (Operation *)malloc(sizeof(Operation));
    op2->id = 2;
    op2->date = 20230102;
    strcpy(op2->account_id, "PSBPN");
    strcpy(op2->bank_lib1, "bis_COTISATIONS BANCAIRES");
    strcpy(op2->bank_lib2, "bis_COTIS Conv serv Affinea");
    strcpy(op2->bank_ref, "bis_0001527");
    strcpy(op2->bank_info, "bis_XCCNV999 2023052600001527000001-CONTRAT CNV0000594707");
    strcpy(op2->bank_type, "bis_Frais bancaires");
    strcpy(op2->bank_category, "bis_Banque et assurances");
    strcpy(op2->bank_sub_category, "bis_Frais bancaires");
    op2->category_id = 0;
    op2->amount = 220.82;

    add_node(&head_op, op1);
    add_node(&head_op, op2);
}

void print_csv_operations()
{
    Operation *op = NULL;
    Node *n = head_op;
    while (n != NULL)
    {
        op = n->data;

        printf("%lu;%s;%ld;%s;%s;%s;%s;%s;%s;%s;%d;%.2f;\n",
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

void test()
{
    gen_fake_opp();
    save_operation();
    load_operation();
}

void usage()
{
    printf("Usage : lst_op -[option]\n");
    printf("Options :\n");
    printf(" -csv : affiche les operations en csv\n");
    printf(" -v   : Affiche le détail des opérations\n");
    printf(" -k   : Affiche le détail des opérations avec les mots clé\n");
}

int main(int argc, char *argv[])
{
    about("Liste des opérations");
    // test();
    if (argc != 2)
    {
        usage();
        return 0;
    }
    int nb = load_operation();
    int i=0;
    if (strcmp("-csv", argv[1]) == 0)
    {print_csv_operations(); i++;}
        

    if (strcmp("-v", argv[1]) == 0)
        {print_operations('v'); i++;}

    if (strcmp("-k", argv[1]) == 0)
        {print_operations('k');  i++;};

    printf("Nombre d'opérations : %d\n", nb);
        if (i==0) usage();
    free_list(head_op);
    return 0;
}