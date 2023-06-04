/*
** Cbank 22/05/2023
** Lecture du fichier des comptes bancaires
** lst_acc : LiST_ACCount
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_acc;

void print_list()
{
    Account* acc=NULL;
    Node *n=head_acc;
    float global_balance=0;
    while (n != NULL) {
        acc=n->data;
        float balance = get_account_balance(acc);
        printf("\n%s - %s\n",acc->id,acc->name);
        printf("  Solde initial : %.2f\n",acc->balance);
        printf("  Solde final   : %.2f\n",balance);
        global_balance+=balance;
        n = n->next;
    }
    printf("\nTotal des liquidités : %.2f\n",global_balance);
}

// Générateur de données de test

void gen_fake_acc() {
    Account *acc1 = (Account*)malloc(sizeof(Account));
    strcpy(acc1->id,"PSBPN");
    strcpy(acc1->name,"Part sociale BPN");
    acc1->balance=545.86;

    Account *acc2 = (Account*)malloc(sizeof(Account));
    strcpy(acc2->id,"LIVA");
    strcpy(acc2->name,"Livret A");
    acc2->balance=78.15;

    Account *acc3 = (Account*)malloc(sizeof(Account));
    strcpy(acc3->id,"LDD");
    strcpy(acc3->name,"Livret de développement dubrable");
    acc3->balance=258.19;

    Account *acc4 = (Account*)malloc(sizeof(Account));
    strcpy(acc4->id,"CCBPN");
    strcpy(acc4->name,"Compte courrant BPN");
    acc4->balance=544;

    add_node(&head_acc, acc1);
    add_node(&head_acc, acc2);
    add_node(&head_acc, acc3);
    add_node(&head_acc, acc4);

}

void test () {
    gen_fake_acc();
    save_account();
    load_account();
}

int main()
{
    about("Liste des comptes bancaires");
    load_account();
    load_operation();
    print_list();
    free_list(head_acc);
    return 0;
}