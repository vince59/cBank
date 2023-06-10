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
extern Node *head_op;

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

void usage(void)
{
    printf("Usage : ls_cat [-v]\n");
    printf("-v : affichage des opérations de la catégorie\n");
}

// groupe les opérations par catégorie

Node *get_operation_by_category()
{
    Operation *op = NULL;
    Node *n = head_op;
    Node *head_stat = NULL;
    Stat *stat = NULL;

    while (n != NULL)
    {
        op = n->data;
        if (op->category_id > 0)
        {
            stat = find_stat_by_cat_id(head_stat, op->category_id);
            if (stat == NULL)
            {
                stat = (Stat *)malloc(sizeof(Stat));
                stat->cat = find_category_by_id(op->category_id);
                stat->amount = op->amount;
                stat->nb = 1;
                stat->list=NULL;
                add_node(&head_stat, stat);
                add_node(&stat->list, op);
            }
            else
            {
                stat->amount += op->amount;
                stat->nb++;
                add_node(&stat->list, op);
            }
        }
        n = n->next;
    }

    n = head_stat;
    while (n != NULL)
    {
        stat = n->data;
        stat->list = sort_operation_by_date(stat->list);
        n = n->next;
    }
    return head_stat;
}

void print_stat(Node *n)
{
    Stat *stat = NULL;
    Operation *op = NULL;
    Node *n2 = NULL;

    while (n != NULL)
    {
        stat = n->data;
        printf("%s\n", stat->cat->name);
        n2 = stat->list;
        float amount=0;
        while (n2 != NULL)
        {
            op = n2->data;
            printf("\t");
            print_operation_light(op);
            amount+=op->amount;
            n2 = n2->next;
        }
        printf("Total : %.0f\n", amount);
        n = n->next;
    }
}

int main(int argc, char *argv[])
{
    about("Liste des categories");
    int nb = load_category();
    load_operation();

    if (argc == 2)
    {
        if (strcmp("-v", argv[1]) == 0)
        {
            Node *stat = get_operation_by_category();
            print_stat(stat);
        }
        else
            usage();
    }
    if (argc == 1)
    {
        print_categories();
    }
    else
        usage();
    printf("Nombre de catégories : %d\n", nb);
    free_list(head_cat);
    return 0;
}