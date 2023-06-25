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
extern Node *head_kw;
extern Node *head_cat;

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
    printf("Usage 1 : ld_bpn compte -a\n");
    printf("-a : chargement automatique des fichiers depuis le répertoire download\n");
    printf("Ex : ./bin/ld_bpn -a\n");
}

void load(char *acc_id, char *file_name)
{
    int nb_dup;
    int nb_bpn_op = load_bpn_csv(acc_id, file_name, &nb_dup);
    printf("Nombre de nouvelles opérations : %d\n", nb_bpn_op);
    printf("Nombre de doublons             : %d\n", nb_dup);
}

int main(int argc, char *argv[])
{
    about("Chargement du fichier de la Banque Populaire du Nord");
    int nb_op = load_operation();
    load_account();
    load_keyword();
    load_category();
    printf("Nombre opération avant         : %d\n", nb_op);

    //
    if (argc == 2)
    {
        if (strcmp("-a", argv[1]) == 0)
        {
            int year, month, day;
            today(&year, &month, &day);
            char *date = fmt_fr_date(year, month, day);
            char ccbpn[200], liva[200], ldd[200];

            sprintf(ccbpn, "/home/vincent/Téléchargements/%s_85391.csv", date);
            sprintf(ldd, "/home/vincent/Téléchargements/%s_85392.csv", date);
            sprintf(liva, "/home/vincent/Téléchargements/%s_85394.csv", date);
            if (is_file_exist(ccbpn))
            {
                printf("Traitement du fichier %s (CCBPN)\n", ccbpn);
                load("CCBPN", ccbpn);
            }
            if (is_file_exist(liva))
            {
                printf("Traitement du fichier %s (LIVA)\n", liva);
                load("LIVA", liva);
            }
            if (is_file_exist(ldd))
            {
                printf("Traitement du fichier %s (LDD)\n", ldd);
                load("LDD", ldd);
            }

            int nb = auto_set_category_ops();
            printf("%d opération(s) classée(s) automatiquement\n", nb);
            save_operation();
        }
        else
        {
            usage();
            return 0;
        }
    }

    free_list(head_op);
    free_list(head_acc);
    free_list(head_kw);
    free_list(head_cat);
    return 0;
}