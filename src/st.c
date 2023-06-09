/*
** Cbank 22/05/2023
** Affiche des stats sur les opérations
** st : StaTisque
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

extern Node * head_acc;
extern Node * head_op;

int main()
{
    about("Statistiques");
    load_account();
    int nb=load_operation();
        float balance = get_initial_balance();
    
    printf("\n");
    printf("Nombre d'opérations           : %d\n",nb);
    printf("Date de la dernière opération : %s\n",fmt_date(get_last_op()->date));
    printf("Solde départ au 31/12/2022    : %.2f\n", balance);
    printf("Somme des liquidités          : %.2f\n",get_balance());
    printf("\n");
    Node *head_stat=get_balance_by_month();

    Stat *stat = NULL;
    Node *n=head_stat;

    while (n != NULL)
    {
        stat = n->data;
        balance+=stat->amount;
        printf("%s : %10.2f %10.2f\n",fmt_int_date(stat->year, stat->month, stat->day),stat->amount,balance);
        n = n->next;
    }

    free_list(head_stat);
    free_list(head_acc);
    free_list(head_op);
    return 0;
}