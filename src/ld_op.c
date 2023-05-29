/*
** Cbank 22/05/2023
** Lecture du fichier des opérations
** ld_op : LoaD_OPeration
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_op;

void print_list()
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

// Générateur de données de test

void gen_fake_opp()
{
    Operation *op1 = (Operation *)malloc(sizeof(Operation));
    op1->id=1;
    op1->date=20230101;
    strcpy(op1->account_id, "PSBPN");
    strcpy(op1->bank_lib1,"COTISATIONS BANCAIRES");
    strcpy(op1->bank_lib2,"COTIS Conv serv Affinea");
    strcpy(op1->bank_ref,"0001527");
    strcpy(op1->bank_info,"XCCNV999 2023052600001527000001-CONTRAT CNV0000594707");
    strcpy(op1->bank_type,"Frais bancaires");
    strcpy(op1->bank_category,"Banque et assurances");
    strcpy(op1->bank_sub_category,"Frais bancaires");
    op1->category_id=0;
    op1->amount=-20.12;

    Operation *op2 = (Operation *)malloc(sizeof(Operation));
    op2->id=2;
    op2->date=20230102;
    strcpy(op2->account_id, "PSBPN");
    strcpy(op2->bank_lib1,"bis_COTISATIONS BANCAIRES");
    strcpy(op2->bank_lib2,"bis_COTIS Conv serv Affinea");
    strcpy(op2->bank_ref,"bis_0001527");
    strcpy(op2->bank_info,"bis_XCCNV999 2023052600001527000001-CONTRAT CNV0000594707");
    strcpy(op2->bank_type,"bis_Frais bancaires");
    strcpy(op2->bank_category,"bis_Banque et assurances");
    strcpy(op2->bank_sub_category,"bis_Frais bancaires");
    op2->category_id=0;
    op2->amount=220.82;

    add_node(&head_op, op1);
    add_node(&head_op, op2);
}

void test()
{
    gen_fake_opp();
    save_operation();
    load_operation();
}

int main(int argc, char *argv[])
{
    about();
    test();
    //load_operation();
    if (argc >= 2 && strcmp("-v", argv[1]) == 0)
        print_list();
    free_list(head_op);
    return 0;
}