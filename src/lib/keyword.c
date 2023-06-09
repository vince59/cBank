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
        if ((*p == ' ' || *p == '-') || ((n + 1) == KW_NAME_LENGTH) || *p == '\0')
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

// Retourne le prochain id de liste

int next_list_id(void)
{
    Keyword *kw = NULL;
    Node *n = head_kw;
    int max = 0;
    while (n != NULL)
    {
        kw = n->data;
        if (max < kw->id)
            max = kw->id;
        n = n->next;
    }
    return ++max;
}

// Ajoute un mot cle dans une liste

int add_keyword(int list_id, int cat_id, char *str)
{
    if (list_id == 0)
        list_id = next_list_id();
    Keyword *kw = (Keyword *)malloc(sizeof(Keyword));
    kw->id = list_id;
    kw->category_id = cat_id;
    strcpy(kw->name, str);
    add_node(&head_kw, kw);
    return list_id;
}

// Structure les mots clé par liste pour être plus facilement analysé

Node *struct_kw(void)
{
    Node *head_sc = NULL;
    Node *n = head_kw;
    Keyword *kw = NULL;
    int lst_curr = 0;
    Scoring *sc;
    while (n != NULL)
    {
        kw = n->data;
        if (lst_curr != kw->id)
        {
            lst_curr = kw->id;
            sc = (Scoring *)malloc(sizeof(Scoring));
            sc->id = lst_curr;
            sc->category_id = kw->category_id;
            sc->nb_kw = 1;
            sc->nb_match = 0;
            sc->kw = NULL;
            add_node(&head_sc, sc);
            add_node(&(sc->kw), kw);
        }
        else
        {
            add_node(&(sc->kw), kw);
            sc->nb_kw++;
        }
        n = n->next;
    }
    return head_sc;
}

// Mise à jour du score des listes pour ce mot clé

void scoring_operation(Node *head_sc, char *kw_name)
{
    Node *n = head_sc;
    Scoring *sc = NULL;
    while (n != NULL)
    {
        sc = n->data;
        Node *n2 = sc->kw;
        Keyword *kw = NULL;
        while (n2 != NULL)
        {
            kw = n2->data;
            if (strcmp(kw->name, kw_name) == 0)
            {
                sc->nb_match++;
            }
            n2 = n2->next;
        }
        sc->score = (sc->nb_match / sc->nb_kw) * 100;
        n = n->next;
    }
}

void raz_scoring(Node *n)
{
    Scoring *sc = NULL;
    while (n != NULL)
    {
        sc = n->data;
        sc->nb_match = 0;
        sc->score = 0;
        n = n->next;
    }
}

int best_score(Node *n, int *nb)
{
    Scoring *sc = NULL;
    *nb = 0;
    int category_id = 0;
    while (n != NULL)
    {
        sc = n->data;
        if (sc->score == 100)
        {
            category_id = sc->category_id;
            (*nb)++;
        }
        n = n->next;
    }
    return category_id;
}