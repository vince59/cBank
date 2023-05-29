/*
** Cbank 22/05/2023
** Lecture du fichier des comptes bancaires
** ld_acc : ListData_ACCount
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
    while (n != NULL) {
        acc=n->data;
        printf("%s;%s;%.2f;\n",acc->id,acc->name,acc->balance);
        n = n->next;
    }
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

int main(int argc, char *argv[])
{
    about();
    //test();
    load_account();
    if (argc>=2 && strcmp("-v",argv[1])==0)
        print_list();
    free_list(head_acc);
    return 0;
}