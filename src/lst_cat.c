/*
** Cbank 22/05/2023
** Lecture du fichier des comptes bancaires
** lst_cat : LiST_CATegory
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_cat;

void print_list()
{
    Category* cat=NULL;
    Node *n=head_cat;
    while (n != NULL) {
        cat=n->data;
        printf("%d;%s\n",cat->id,cat->name);
        n = n->next;
    }
}

// Générateur de données de test

void gen_fake_category() {
    Category *cat1 = (Category*)malloc(sizeof(Category));
    cat1->id=1;
    strcpy(cat1->name,"Alimentation");

    Category *cat2 = (Category*)malloc(sizeof(Category));
    cat2->id=2;
    strcpy(cat2->name,"Electricité");

    Category *cat3 = (Category*)malloc(sizeof(Category));
    cat3->id=3;
    strcpy(cat3->name,"Eau");

    Category *cat4 = (Category*)malloc(sizeof(Category));
    cat4->id=4;
    strcpy(cat4->name,"Assurance voiture");

    add_node(&head_cat, cat1);
    add_node(&head_cat, cat2);
    add_node(&head_cat, cat3);
    add_node(&head_cat, cat4);

}

void test () {
    gen_fake_category();
    save_category();
    load_category();
}

int main(int argc, char *argv[])
{
    about("Liste des categories");
    test();
    //int nb=load_category();
    if (argc>=2 && strcmp("-v",argv[1])==0)
        print_list();
    //printf("%d catégories\n",nb);
    free_list(head_cat);
    return 0;
}