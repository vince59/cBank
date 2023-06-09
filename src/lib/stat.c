/*
** Cbank 22/05/2023
** fonctions pour le calculstat
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_acc;
extern Node *head_op;

Stat *find_stat_by_ym(Node *n, int year, int month)
{
    Stat *stat = NULL;
    while (n != NULL)
    {
        stat = n->data;

        if ((stat->year == year) && (stat->month == month))
            return stat;
        n = n->next;
    }
    return NULL;
}

Stat *find_stat_by_cat_id(Node *n, int cat_id)
{
    Stat *stat = NULL;
    while (n != NULL)
    {
        stat = n->data;

        if (stat->cat->id == cat_id)
            return stat;
        n = n->next;
    }
    return NULL;
}

Node* sort_stat(Node* head)
{
    if (head == NULL || head->next == NULL) {
        // Empty list or single node, already sorted
        return head;
    }

    // Perform a simple bubble sort on the linked list
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            Stat * stat1=ptr1->data;
            Stat * stat2=ptr1->next->data;
            if (stat1->date > stat2->date) {
                // Swap the nodes
                Node* temp = ptr1->next;
                ptr1->next = ptr1->next->next;
                temp->next = ptr1;
                if (ptr1 == head) {
                    head = temp;
                } else {
                    Node* prev = head;
                    while (prev->next != ptr1) {
                        prev = prev->next;
                    }
                    prev->next = temp;
                }
                ptr1 = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}


// donne le solde mois pas mois tout comptes confondus

Node *get_balance_by_month()
{
    Operation *op = NULL;
    Node *n = head_op;
    Node *head_stat = NULL;
    Stat *stat = NULL;
    int year, month, day;

    while (n != NULL)
    {
        op = n->data;
        get_ymd(op->date, &year, &month, &day);
        stat = find_stat_by_ym(head_stat, year, month);
        if (stat == NULL)
        {
            stat = (Stat *)malloc(sizeof(Stat));
            stat->amount = op->amount;
            stat->year = year;
            stat->month = month;
            stat->day = 1;
            stat->date = fmt_long_date(stat->year, stat->month, stat->day);
            add_node(&head_stat, stat);
        }
        else
        {
            stat->amount += op->amount;
        }
        n = n->next;
    }
    return sort_stat(head_stat);
}
