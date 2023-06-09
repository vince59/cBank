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
extern Node *head_cat;

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
    free_list(head_cat);
    free_list(head_op);
    return 0;
}