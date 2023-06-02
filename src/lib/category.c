/*
** Cbank 22/05/2023
** fonctions pour la gestion des categories
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_cat;

int save_category() {
    int nb=0;
    FILE* file = fopen(CAT_CSV, "w");

    if (file == NULL) {
        printf("Failed to create file: %s\n", CAT_CSV);
        exit(1);
    }
    fprintf(file, "id;name\n"); // Ecriture de l'entête
    
    Node *n=head_cat;
    Category* cat=NULL;
    while (n != NULL) {
        cat=n->data;
        fprintf(file, "%d;%s\n", cat->id,cat->name);
        nb++;
        n = n->next;
    }

    fclose(file);
    return nb;
}

int load_category() {
    int nb=0;
    FILE* file = fopen(CAT_CSV, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", CAT_CSV);
        exit(1);
    }

    char line[100];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_cat=NULL;
    while (fgets(line, sizeof(line), file) != NULL) {
        int id;
        char name[CAT_NAME_LENGTH];

        if (sscanf(line, "%d;%[^\n]", &id, name) == 2) {
            Category *cat = (Category*)malloc(sizeof(Category));
            cat->id=id;
            strcpy(cat->name, name);
            nb++;
            add_node(&head_cat, cat);
        }
    }

    fclose(file);
    return nb;
}
