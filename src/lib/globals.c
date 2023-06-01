#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libcbank.h"

Node * head_acc; // liste des comptes
Node * head_op; // liste des opérations

void about(char *msg)
{
    printf("Cbank V1.0\n%s\n",msg);
}

// passe de la chaine "12/01/2023" à l'entier long 20230112

long cnv_date(char * sdate) 
{
    char d[3],m[3],y[5];
    char result[10];

    sscanf(sdate,"%2[^/]/%2[^/]/%4s", d, m, y);
     
    strcpy(result, y);     
    strcat(result, m); 
    strcat(result, d);

    return atol(result);
}

// passe de la chaine "45,15" au flotant 45.15

float cnv_float(char * sfloat) 
{
char* decimalPointPos = strchr(sfloat, ',');
    if (decimalPointPos != NULL) {
        *decimalPointPos = '.'; // On remplace par un point
    }
    return atof(sfloat); 
}

// Remplace une sous chaine par une autre

void replaceSubstring(char* str, const char* find, const char* replace) {
    int findLen = strlen(find);
    int replaceLen = strlen(replace);

    char* found = strstr(str, find);
    while (found != NULL) {
        memmove(found + replaceLen, found + findLen, strlen(found + findLen) + 1);
        memcpy(found, replace, replaceLen);

        found = strstr(found + replaceLen, find);
    }
}