/*
** Cbank 22/05/2023
** Classification automatique d'une opération
** cla_op : CLAssify_OPeration
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;

void print_list(Node *n)
{
    Keyword *kw = NULL;
    while (n != NULL)
    {
        kw = n->data;
        printf("%d;%d;%s\n", kw->id, kw->category_id, kw->name);
        n = n->next;
    }
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

void print_scoring(Node *n)
{
    Scoring *sc = NULL;
    int nb = 0;
    while (n != NULL)
    {
        sc = n->data;
        Node *n2 = sc->kw;
        Keyword *kw = NULL;
        if (sc->score > 0)
        {
            printf("Liste %d score %.2f %% %d %d :\n", sc->id, sc->score, sc->nb_kw, sc->nb_match);
            nb++;
            while (n2 != NULL)
            {
                kw = n2->data;
                printf("%s ", kw->name);
                n2 = n2->next;
            }
            printf("\n");
        }

        n = n->next;
    }
    if (nb == 0)
        printf("Aucune catégorie n'a été deterrminée automatiquement\n");
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

int auto_set_category_ops(void)
{
    Node *head_sc = struct_kw();
    Operation *op = NULL;
    Node *n = head_op;
    int nb = 0;
    while (n != NULL) // Parcours des opérations
    {
        op = n->data;
        if (op->category_id == 0) // non classées
        {
            Node *kws = extract_kw_op(op);
            Keyword *kw = NULL;
            Node *n2 = kws;

            while (n2 != NULL) // Parcours des mots clés
            {
                kw = n2->data;
                scoring_operation(head_sc, kw->name); // Mise à jour du score des listes pour ce mot clé
                n2 = n2->next;
            }
            int nb_db;
            op->category_id = best_score(head_sc, &nb_db);
            if (op->category_id > 0)
            {
                nb++;
                print_operation(op);
                printf("Opération classée automatiquement -> %s\n", find_category_by_id(op->category_id)->name);
                if (nb_db > 1)
                    printf("Attention il y a deux catégories corrspondant aux mots clé !!\n");
                printf("---\n");
            }
            raz_scoring(head_sc);
        }
        n = n->next;
    }
    free_list(head_sc);
    return nb;
}

int main()
{
    about("Classification automatique des opérations\n");

    load_keyword();
    load_operation();
    load_category();
    int nb = auto_set_category_ops();

    printf("%d opération(s) classée(s) automatiquement\n", nb);
    save_operation();
    free_list(head_kw);
    free_list(head_op);
    return 0;
}