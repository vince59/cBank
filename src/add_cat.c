/*
** Cbank 22/05/2023
** Ajout d'une categorie
** add_cat : LiST_CATegory
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_cat;

void usage()
{
    printf("usage : \nadd_cat \"nom de la catégorie à ajouter\"\n");
}

int main(int argc, char *argv[])
{
    about("Ajout d'une catégorie");
    if (argc!=2) {
        usage();
        return 0;
    }
        
    int nb=load_category();
    //remove_last_char(argv[1]);
    add_category(argv[1]);
    printf("\nCategorie \"%s\" ajoutée.\n\n",argv[1]);
    save_category();
    print_categories();
    nb=load_category();
    printf("Nombre de catégories : %d\n",nb);
    free_list(head_cat);
    return 0;
}