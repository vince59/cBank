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
    Keyword *kw1 = (Keyword*)malloc(sizeof(Keyword));
    kw1->id=1;
    kw1->category_id=1;
    strcpy(kw1->name,"CARREFOUR");

    Keyword *kw2 = (Keyword*)malloc(sizeof(Keyword));
    kw2->id=1;
    kw2->category_id=1;
    strcpy(kw2->name,"CITY");

    Keyword *kw3 = (Keyword*)malloc(sizeof(Keyword));
    kw3->id=2;
    kw3->category_id=2;
    strcpy(kw3->name,"EDF");

    Keyword *kw4 = (Keyword*)malloc(sizeof(Keyword));
    kw4->id=3;
    kw4->category_id=3;
    strcpy(kw4->name,"EAU");

    Keyword *kw5 = (Keyword*)malloc(sizeof(Keyword));
    kw5->id=3;
    kw5->category_id=3;
    strcpy(kw5->name,"METROPOLE");

    Keyword *kw6 = (Keyword*)malloc(sizeof(Keyword));
    kw6->id=3;
    kw6->category_id=3;
    strcpy(kw6->name,"EUROPEENNE");

    Keyword *kw7 = (Keyword*)malloc(sizeof(Keyword));
    kw7->id=3;
    kw7->category_id=3;
    strcpy(kw7->name,"LILLE");

    add_node(&head_kw, kw1);
    add_node(&head_kw, kw2);
    add_node(&head_kw, kw3);
    add_node(&head_kw, kw4);
    add_node(&head_kw, kw5);
    add_node(&head_kw, kw6);
    add_node(&head_kw, kw7);
}

void test () {
    gen_fake_keyword();
    save_keyword();
    load_keyword();
}

void test2 () {
    load_operation();
    Node *kw = extract_kw((Operation*)head_op->data);
    print_list(kw);
}

int main(int argc, char *argv[])
{
    about("Liste des mots clé");
    //test();
    test2();
    return 0;
    int nb=load_keyword();
    if (argc>=2 && strcmp("-v",argv[1])==0)
        print_list(head_kw);
    printf("Nombre de mots clés : %d\n",nb);
    free_list(head_kw);
    return 0;
}