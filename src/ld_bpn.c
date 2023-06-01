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

int load_bpn_csv(char *acc_id, char *file_name, int *nb_dup)
{
    Account *acc;
    if ((acc = find_acc_by_id(acc_id)) == NULL)
        return -1;

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", file_name);
        exit(1);
    }

    char line[300];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    int nb = 0;
    (*nb_dup) = 0;
    Id max = next_operation_id();
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
        char sdeb[OP_LIB_LENGTH]; // Debit
        char scdt[OP_LIB_LENGTH]; // Credit
        char d2[11];              // Date operation
        char d3[11];              // Date de valeur
        int p;                    // Pointage operation
        int z;
        p = 0;
        replaceSubstring(line, ";;", ";0;");
        if ((z = sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
                        d1, lib1, lib2, ref, info, typ, cat, scat, sdeb, scdt, d2, d3, &p)) == 13)
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
            op->amount = cnv_float(scdt) - cnv_float(sdeb);

            if (!chk_duplicate_operation(op))
            {
                add_node(&head_op, op);
                nb++;
                max++;
            }
            else
            {
                (*nb_dup)++;
            }
        }
    }

    fclose(file);
    return nb;
}

void test()
{
    int nb_op = load_operation();
    int nb_acc = load_account();
    int nb_dup;
    int nb_bpn_op = load_bpn_csv("CCBPN", "./data/bqe/29052023_85391.csv", &nb_dup);
    printf("acc : %d op : %d bpn : %d : dup : %d\n", nb_op, nb_acc, nb_bpn_op, nb_dup);
}

void usage(void)
{
    printf("Usage : ld_bpn compte fichier [-s]\n-s : simulation (pas de mise à jour)\n");
    printf("Ex : ./bin/ld_bpn CCBPN ./data/bqe/29052023_85391.csv -s\n");
}

int main(int argc, char *argv[])
{
    about("Chargement du fichier de la Banque Populaire du Nord");
    // test();

    if (argc == 4)
    {
        if (strcmp("-s", argv[3]) == 0)
        {
            printf("Mode simulation\n");
        }
        else
        {
            usage();
            return 0;
        }
    }

    if (argc >= 3)
    {

        int nb_op = load_operation();
        int nb_acc = load_account();
        int nb_dup;
        int nb_bpn_op = load_bpn_csv(argv[1], argv[2], &nb_dup);
        if (argc == 3)
            save_operation();

        printf("Nombre de comptes              : %d\n",nb_acc);
        printf("Nombre opération avant         : %d\n",nb_op);
        printf("Nombre de nouvelles opérations : %d\n",nb_bpn_op); 
        printf("Nombre de doublons             : %d\n",nb_dup);
    }
    else
        usage();

    free_list(head_op);
    free_list(head_acc);
    return 0;
}