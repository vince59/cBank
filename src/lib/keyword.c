/*
** Cbank 22/05/2023
** fonctions pour la gestion des mots clés
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;

// Enregistre les mots cle sur le fichier

int save_keyword()
{
    int nb = 0;
    FILE *file = fopen(KW_CSV, "w");

    if (file == NULL)
    {
        printf("Failed to create file: %s\n", KW_CSV);
        exit(1);
    }
    fprintf(file, "id;category_id;name\n"); // Ecriture de l'entête

    Node *n = head_kw;
    Keyword *kw = NULL;
    while (n != NULL)
    {
        kw = n->data;
        fprintf(file, "%d;%d;%s\n", kw->id, kw->category_id, kw->name);
        nb++;
        n = n->next;
    }

    fclose(file);
    return nb;
}

// Charge les mots clés depuis le fichier

int load_keyword()
{
    int nb = 0;
    FILE *file = fopen(KW_CSV, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", KW_CSV);
        exit(1);
    }

    char line[100];
    fscanf(file, "%[^\n]\n", line); // On zap l'entête

    head_kw = NULL;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int id, category_id;
        char name[KW_NAME_LENGTH];

        if (sscanf(line, "%d;%d;%[^\n]", &id, &category_id, name) == 3)
        {
            Keyword *kw = (Keyword *)malloc(sizeof(Keyword));
            kw->id = id;
            kw->category_id = category_id;
            strcpy(kw->name, name);
            nb++;
            add_node(&head_kw, kw);
        }
    }

    fclose(file);
    return nb;
}

// Recherche un mot clé dans une liste de mots cle

Keyword *find_kw_by_name(Node *hkw, char *kw_name)
{
    Keyword *kw = NULL;
    Node *n = hkw;

    while (n != NULL)
    {
        kw = n->data;
        if (strcmp(kw->name, kw_name) == 0)
            return kw;
        n = n->next;
    }
    return NULL;
}

// Extrait les mots clé d'une chaine

Node *extract_kw_str(Node *h_kw, const char *p)
{
    char str[KW_NAME_LENGTH];
    int n = 0;
    for (;;)
    {
        if ((*p == ' ') || ((n + 1) == KW_NAME_LENGTH) || *p == '\0')
        {
            if (n > 0)
            {
                Keyword *kw = (Keyword *)malloc(sizeof(Keyword));
                str[n++] = '\0';

                if (find_kw_by_name(h_kw, str) == NULL)
                {
                    strcpy(kw->name, str);
                    add_node(&h_kw, kw);
                }
                n = 0;
            }
            if (*p == '\0')
                break;
        }
        else
            str[n++] = *p;
        p++;
    }
    return h_kw;
}

// Extrait les mots clé d'une operation

Node *extract_kw_op(Operation *op)
{
    Node *h_kw = NULL;

    h_kw = extract_kw_str(h_kw, op->bank_lib1);
    h_kw = extract_kw_str(h_kw, op->bank_lib2);
    h_kw = extract_kw_str(h_kw, op->bank_info);
    h_kw = extract_kw_str(h_kw, op->bank_ref);
    return h_kw;
}