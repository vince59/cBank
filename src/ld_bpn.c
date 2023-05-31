/*
** Cbank 22/05/2023
** Lecture du fichier de la BPN
** ld_bpn : LoaD_Banque Populaire du Nord
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_op;
extern Node *head_acc;

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

int load_bpn_csv(char *acc_id, char *file_name)
{
    Account *acc;
    if ((acc=find_acc_by_id(acc_id)) == NULL)
        return -1;

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", OPERATION_CSV);
        exit(1);
    }

    char line[300];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    int nb = 0;
    Id max=next_operation_id();
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char d1[10];              // Date de comptabilisation
        char lib1[OP_LIB_LENGTH]; // Libelle simplifie
        char lib2[OP_LIB_LENGTH]; // Libelle operation
        char ref[OP_LIB_LENGTH];  // Reference
        char info[OP_LIB_LENGTH]; // Informations complementaires
        char typ[OP_LIB_LENGTH];  // Type operation
        char cat[OP_LIB_LENGTH];  // Categorie
        char scat[OP_LIB_LENGTH]; // Sous categorie
        char sdeb[OP_LIB_LENGTH];                // Debit
        char scdt[OP_LIB_LENGTH];                // Credit
        char d2[11];              // Date operation
        char d3[11];              // Date de valeur
        int p;                    // Pointage operation
                int z;
        p=0;                
        replaceSubstring(line, ";;", ";0;");
        //printf("\n>>>>%s\n",line);
        if ((z=sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
                   d1,lib1,lib2,ref,info,typ,cat,scat,sdeb,scdt,d2,d3,&p)) == 13)
        {
            Operation *op = (Operation *)malloc(sizeof(Operation));
            
            op->id = max;
            op->date = cnv_date(d3);
            strcpy(op->account_id, acc->id);
            strcpy(op->bank_lib1, lib1);
            strcpy(op->bank_lib2, lib2);
            strcpy(op->bank_ref, ref);
            strcpy(op->bank_info, info);
            strcpy(op->bank_type, typ);
            strcpy(op->bank_category, cat);
            strcpy(op->bank_sub_category, scat);
            op->category_id = 0;
            op->amount = cnv_float(scdt)-cnv_float(sdeb);

            add_node(&head_op, op);
            nb++;
            max++;
        }
        /*
        printf("aa%d\n",z);
            printf("Date de comptabilisation: %s\n", d1);
    printf("Libelle simplifie: %s\n", lib1);
    printf("Libelle operation: %s\n", lib2);
    printf("Reference: %s\n", ref);
    printf("Informations complementaires: %s\n", info);
    printf("Type operation: %s\n", typ);
    printf("Categorie: %s\n", cat);
    printf("Sous categorie: %s\n", scat);
    printf("Debit: %.2f\n", cnv_float(sdeb));
    printf("Credit: %.2f\n", cnv_float(scdt));
    printf("Date operation: %s\n", d2);
    printf("Date de valeur: %s\n", d3);
    printf("Pointage operation: %d\n", p);
        break;*/
    }

    fclose(file);
    return nb;
}

void test()
{
    int nb_op = load_operation();
    int nb_acc = load_account();
    int nb_bpn_op = load_bpn_csv("CCBPN", "./data/bqe/29052023_85391.csv");
    printf("acc : %d op : %d bpn : %d\n", nb_op, nb_acc, nb_bpn_op);
}

int main(int argc, char *argv[])
{
    about();
    test();

    if (argc >= 2 && strcmp("-v", argv[1]) == 0)
        print_operations();
    free_list(head_op);
    free_list(head_acc);
    return 0;
}