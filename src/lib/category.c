/*
** Cbank 22/05/2023
** fonctions pour la gestion des categories
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_cat;

int save_category()
{
    int nb = 0;
    FILE *file = fopen(CAT_CSV, "w");

    if (file == NULL)
    {
        printf("Failed to create file: %s\n", CAT_CSV);
        exit(1);
    }
    fprintf(file, "id;name\n"); // Ecriture de l'entête

    Node *n = head_cat;
    Category *cat = NULL;
    while (n != NULL)
    {
        cat = n->data;
        fprintf(file, "%d;%s\n", cat->id, cat->name);
        nb++;
        n = n->next;
    }

    fclose(file);
    return nb;
}

int load_category()
{
    int nb = 0;
    FILE *file = fopen(CAT_CSV, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", CAT_CSV);
        exit(1);
    }

    char line[100];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_cat = NULL;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int id;
        char name[CAT_NAME_LENGTH];

        if (sscanf(line, "%d;%[^\n]", &id, name) == 2)
        {
            Category *cat = (Category *)malloc(sizeof(Category));
            cat->id = id;
            strcpy(cat->name, name);
            nb++;
            add_node(&head_cat, cat);
        }
    }

    fclose(file);
    return nb;
}

// retourne le prochain id pour ajouter une nouvelle categorie

int next_category_id(void)
{
    Category *cat = NULL;
    Node *n = head_cat;
    int max = 0;
    while (n != NULL)
    {
        cat = n->data;
        if (max < cat->id)
            max = cat->id;
        n = n->next;
    }
    return ++max;
}

// Ajoute une catégorie

Category *add_category(const char *name)
{
    Category *cat = (Category *)malloc(sizeof(Category));
    cat->id = next_category_id();
    strcpy(cat->name, name);
    add_node(&head_cat, cat);
    return cat;
}


// affichage des categories

void print_categories()
{
    Category* cat=NULL;
    Node *n=head_cat;
    while (n != NULL) {
        cat=n->data;
        printf("%d;%s\n",cat->id,cat->name);
        n = n->next;
    }
}

Category *find_category_by_id(int cat_id)
{
    Category *cat = NULL;
    Node *n = head_cat;

    while (n != NULL)
    {
        cat = n->data;
        if (cat->id==cat_id)
            return cat;
        n = n->next;
    }
    return NULL;
}