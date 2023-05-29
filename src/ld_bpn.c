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
extern Node * head_acc;

Account *find_acc_id(char *acc_id) {
    Account* acc=NULL;
    Node *n=head_acc;
    
    while (n != NULL) {
        acc=n->data;
        if (strcmp(acc->id, acc_id)==0) 
            return acc;
        n = n->next;
    }
    return NULL;
}


int load_bpn_csv(char *acc_id, char *file) {
    if (find_acc_id(acc_id)==NULL)
        return -1;
 return 3;
}

void test()
{
    int nb_op = load_operation();
    int nb_acc = load_account();
    int nb_bpn_op = load_bpn_csv("CCBPN", "./data/bqe/29052023_85391.csv");
    printf("acc : %d op : %d bpn : %d\n",nb_op,nb_acc,nb_bpn_op);
}

int main(int argc, char *argv[])
{
    about();
    test();

    if (argc >= 2 && strcmp("-v", argv[1]) == 0)
        printf("Hello\n");
    free_list(head_op);
    free_list(head_acc);
    return 0;
}