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


int main()
{
    about("Statistiques");
    load_account();
    int n=load_operation();
    printf("\n");
    printf("Nombre d'opérations           : %d\n",n);
    printf("Date de la dernière opération : %s\n",fmt_date(get_last_op()->date));
    printf("Somme des liquidités          : %.2f\n",get_balance());

    free_list(head_acc);
    return 0;
}