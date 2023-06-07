#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libcbank.h"

Node * head_acc; // liste des comptes
Node * head_op; // liste des opérations
Node * head_cat; // liste des categories
Node * head_kw; // liste des mots clés

void about(char *msg)
{
    printf("Cbank V1.0 --- %s\n",msg);
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

// extrait l'année le mois et le jour de la date

void get_ymd(long date, int *year, int *month, int *day)
{
    *day = date % 100;
    *month = (date / 100) % 100;
    *year = date / 10000;
}

// converti l'entier long 20230112 en 12/01/2023

char *fmt_date(long date) {
    char *str=malloc(sizeof(char)*11);
    int year, month, day;
    get_ymd(date, &year, &month, &day);
    sprintf(str, "%02d/%02d/%04d", day, month, year);
    return str;
}

// converti d m y en 12/01/2023

char *fmt_int_date(int year, int month, int day) {
    char *str=malloc(sizeof(char)*11);
    sprintf(str, "%02d/%02d/%04d", day, month, year);
    return str;
}

// converti d m y en un entier long 20230112 

long fmt_long_date(int year, int month, int day) {
    long date;

    date=((year*100)+month)*100+day;
    return date;
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

void replace_substring(char* str, const char* find, const char* replace) {
    int findLen = strlen(find);
    int replaceLen = strlen(replace);

    char* found = strstr(str, find);
    while (found != NULL) {
        memmove(found + replaceLen, found + findLen, strlen(found + findLen) + 1);
        memcpy(found, replace, replaceLen);

        found = strstr(found + replaceLen, find);
    }
}

// Retire le dernier caractère d'une chaine

void remove_last_char(char* str) {
    int length = strlen(str);
    if (length > 0) {
        str[length - 1] = '\0';
    }
}
