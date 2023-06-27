#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libcbank.h"

Node *head_acc; // liste des comptes
Node *head_op;  // liste des opérations
Node *head_cat; // liste des categories
Node *head_kw;  // liste des mots clés

void about(char *msg)
{
    printf("Cbank V1.0 --- %s\n", msg);
}

// passe de la chaine "12/01/2023" à l'entier long 20230112

long cnv_date(char *sdate)
{
    char d[3], m[3], y[5];
    char result[10];

    sscanf(sdate, "%2[^/]/%2[^/]/%4s", d, m, y);

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

char *fmt_date(long date)
{
    char *str = malloc(sizeof(char) * 11);
    int year, month, day;
    get_ymd(date, &year, &month, &day);
    sprintf(str, "%02d/%02d/%04d", day, month, year);
    return str;
}

// converti d m y en 12/01/2023

char *fmt_int_date(int year, int month, int day)
{
    char *str = malloc(sizeof(char) * 11);
    sprintf(str, "%02d/%02d/%04d", day, month, year);
    return str;
}

// converti m y en 01/2023

char *fmt_int_month_year(int year, int month)
{
    char *str = malloc(sizeof(char) * 8);
    sprintf(str, "%02d/%04d", month, year);
    return str;
}

// converti d m y en 12012023

char *fmt_fr_date(int year, int month, int day)
{
    char *str = malloc(sizeof(char) * 9);
    sprintf(str, "%02d%02d%04d", day, month, year);
    return str;
}

// converti d m y en un entier long 20230112

long fmt_long_date(int year, int month, int day)
{
    long date;

    date = ((year * 100) + month) * 100 + day;
    return date;
}

// passe de la chaine "45,15" au flotant 45.15

float cnv_float(char *sfloat)
{
    char *decimalPointPos = strchr(sfloat, ',');
    if (decimalPointPos != NULL)
    {
        *decimalPointPos = '.'; // On remplace par un point
    }
    return atof(sfloat);
}

// Donne la date du jour sous la forme 01122023

void today(int *year, int *month, int *day)
{
    // Get the current time
    time_t currentTime = time(NULL);

    // Convert the current time to a struct tm
    struct tm *localTime = localtime(&currentTime);

    // Extract the date components from the struct tm
    *year = localTime->tm_year + 1900; // Years since 1900
    *month = localTime->tm_mon + 1;    // Months since January (0-11)
    *day = localTime->tm_mday;         // Day of the month (1-31)
}

// Remplace une sous chaine par une autre

void replace_substring(char *str, const char *find, const char *replace)
{
    int findLen = strlen(find);
    int replaceLen = strlen(replace);

    char *found = strstr(str, find);
    while (found != NULL)
    {
        memmove(found + replaceLen, found + findLen, strlen(found + findLen) + 1);
        memcpy(found, replace, replaceLen);

        found = strstr(found + replaceLen, find);
    }
}

// Retire le dernier caractère d'une chaine

void remove_last_char(char *str)
{
    int length = strlen(str);
    if (length > 0)
    {
        str[length - 1] = '\0';
    }
}

// Teste si un fichier existe ou non

int is_file_exist(char *filename)
{
    if (access(filename, F_OK) != -1)
        return 1; // exist
    return 0;     // not exist
}

// Donne le nombre de jour dans le mois

int get_days_in_month(int month, int year)
{
    // Array of days in each month (considering leap years)
    int daysInMonth[] = {31, 28 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Validate the month argument
    if (month < 1 || month > 12)
    {
        printf("Invalid month!\n");
        return -1; // Return an error value
    }

    // Get the number of days in the given month
    int days = daysInMonth[month - 1];

    return days;
}

// Vérifie si un caractère se trouve dans la chaine

int find_char(const char *str, int target)
{
    if (str == NULL)
        return -1;

    const char *ptr = str;
    int index = 0;

    while (*ptr != '\0')
    {
        if (*ptr == target)
            return index;
        ptr++;
        index++;
    }
    return -1; // Character not found
}

long int divide_rounded_up(long int a, long int b)
{
    long int result = a / b;
    if (a % b != 0)
        result++;
    return result;
}