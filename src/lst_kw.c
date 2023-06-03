/*
** Cbank 22/05/2023
** Lecture du fichier des mots clés
** lst_kw : LiST_KEYword
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_kw;
extern Node * head_op;

void print_list(Node *n)
{
    Keyword* kw=NULL;
    while (n != NULL) {
        kw=n->data;
        printf("%d;%d;%s\n",kw->id,kw->category_id,kw->name);
        n = n->next;
    }
}

// Générateur de données de test

void gen_fake_keyword() {
    int list_id = 0;
    list_id=add_keyword(list_id, 1, "CARREFOUR");
    list_id=add_keyword(list_id, 1, "CITY");

    list_id=add_keyword(0, 2, "EDF");
    list_id=add_keyword(list_id, 2, "EAU");
    list_id=add_keyword(list_id, 2, "METROPOLE");
    list_id=add_keyword(list_id, 2, "EUROPEENNE");
    list_id=add_keyword(list_id, 2, "LILLE");
}

void test () {
    gen_fake_keyword();
    save_keyword();
    load_keyword();
}

void test2 () {
    load_operation();
    Node *kw = extract_kw_op((Operation*)head_op->data);
    print_list(kw);
    free_list(kw);
}

void test3 () {
    load_keyword();
    gen_fake_keyword();
    save_keyword();
    free_list(head_kw);
    load_keyword();
}

int main(int argc, char *argv[])
{
    about("Liste des mots clé");

    int nb=load_keyword();
    if (argc>=2 && strcmp("-v",argv[1])==0)
        print_list(head_kw);
    printf("Nombre de mots clés : %d\n",nb);
    free_list(head_kw);
    return 0;
}