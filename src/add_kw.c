/*
** Cbank 22/05/2023
** Ajout d'une liste de mot clés
** add_kw : ADD_KewWord
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_cat;

void usage()
{
    printf("usage : \nadd_kw id_cat mot1 mot2 mot3 ...\n");
    printf("id_cat est l'id de la categorie\n");
}

int main(int argc, char *argv[])
{
    about("Ajout d'une liste de mots clé");
    if (argc < 3)
    {
        usage();
        return 0;
    }

    load_keyword();
    load_category();
    int list_id = 0;
    int cat_id = atoi(argv[1]);
    Category *cat = find_category_by_id(cat_id);
    if (cat != NULL)
    {
        for (int i = 2; i < argc; i++)
        {
            char *kw = malloc(sizeof(char) * strlen(argv[i]));
            strcpy(kw, argv[i]);
            list_id = add_keyword(list_id, cat->id, kw);
        }
        printf("\n%d Mot(s) cle ajouté pour la catégorie %s, id de la nouvelle liste : %d\n", argc - 2, cat->name, list_id);
        save_keyword();
    }
    else
        printf("Id de catégorie %d non trouvé, aucun mot cle ajouté\n", cat_id);
    free_list(head_kw);
    free_list(head_cat);
    return 0;
}