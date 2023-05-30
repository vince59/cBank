#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libcbank.h"

Node * head_acc; // liste des comptes
Node * head_op; // liste des opérations

void about(void)
{
    printf("Cbank V1.0\n");
}

// passe de la chaine "12/01/2023" à l'entier long 20230112

long cnv_date(char * sdate) 
{
    long date;
    char d[3],m[3],y[5];
    char result[10];

    sscanf(sdate,"%2[^/]/%2[^/]/%4s", d, m, y);
     
    strcpy(result, y);     
    strcat(result, m); 
    strcat(result, d);

    date = atol(result);
    
    return date; 
}