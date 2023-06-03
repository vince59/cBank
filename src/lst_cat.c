/*
** Cbank 22/05/2023
** Lecture du fichier des ategories
** lst_cat : LiST_CATegory
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_cat;

// Générateur de données de test

void gen_fake_category()
{

    add_category("Alimentation");
    add_category("Electricité");
    add_category("Eau");
    add_category("Assurance voiture");
}

void test()
{
    gen_fake_category();
    save_category();
    load_category();
}

void test2()
{
    load_category();
    gen_fake_category();
    print_categories();
}

int main()
{
    about("Liste des categories");
    int nb = load_category();

    print_categories();
    printf("Nombre de catégories : %d\n", nb);
    free_list(head_cat);
    return 0;
}