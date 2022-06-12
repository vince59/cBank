/*
** Cbank 12/06/2022
*/

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int load_file(char *file_name)
{
	int c;
	FILE *file;
	file = fopen(file_name, "r");
	
	if (file)
	{
		while ((c = getc(file)) != EOF)
		{
            printf("%c",c);
		}
		fclose(file);
	}
	else
	{
		puts("Error when loding map, check map file path\n");
		return FALSE;
	}
    return TRUE;
}

int main(int argc, char *argv[])
{
    about();
    if (!load_file("/home/vincent/Téléchargements/bank_account_details.csv"))
     return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
