/*
** Cbank 22/05/2023
** fonctions pour la gestion des mots clés
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_kw;

int save_keyword() {
    int nb=0;
    FILE* file = fopen(KW_CSV, "w");

    if (file == NULL) {
        printf("Failed to create file: %s\n", KW_CSV);
        exit(1);
    }
    fprintf(file, "id;category_id;name\n"); // Ecriture de l'entête
    
    Node *n=head_kw;
    Keyword* kw=NULL;
    while (n != NULL) {
        kw=n->data;
        fprintf(file, "%d;%d;%s\n", kw->id,kw->category_id,kw->name);
        nb++;
        n = n->next;
    }

    fclose(file);
    return nb;
}

int load_keyword() {
    int nb=0;
    FILE* file = fopen(KW_CSV, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", KW_CSV);
        exit(1);
    }

    char line[100];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_kw=NULL;
    while (fgets(line, sizeof(line), file) != NULL) {
        int id,category_id;
        char name[KW_NAME_LENGTH];

        if (sscanf(line, "%d;%d;%[^\n]", &id, &category_id,name) ==3) {
            Keyword *kw = (Keyword*)malloc(sizeof(Keyword));
            kw->id=id;
            kw->category_id=category_id;
            strcpy(kw->name, name);
            nb++;
            add_node(&head_kw, kw);
        }
    }

    fclose(file);
    return nb;
}
