/*
** Cbank 12/06/2022
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

int main(int argc, char *argv[])
{
    about();
    if (argc>=2 && strcmp("-v",argv[1])==0)
        printf("Hello\n");
    return 0;
}