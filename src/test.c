#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcbank.h"

#define V_SEP "|"
#define H_SEP "-"
#define I_SEP "+"

int main()
{
    clearScreen();

    Array *array=(Array *)malloc(sizeof(Array));
    int col_nb_char = 20;
    int nb_col = 3;
    int nb_line = 15;
    array->cells=NULL;
    for (int l = 0; l < nb_line; l++)
    {
        for (int c = 0; c < nb_col; c++)
        {
            Cell *cell = (Cell *)malloc(sizeof(Cell));
            cell->l = l + 1;
            cell->c = c + 1;
            cell->content = (char *)malloc(sizeof(char) * col_nb_char);
            if (cell->l==1)
                sprintf(cell->content,"HEADER %d",cell->c);
            else
                sprintf(cell->content,"CELL %d %d",cell->l, cell->c);
            add_node(&array->cells, cell);
        }
    }

    Cell *cell = NULL;
    Node *n = array->cells;
    while (n != NULL)
    {
        cell = n->data;
        setCursorLocation(cell->c==1?1:(cell->c-1)*col_nb_char, cell->l);
        set_color(cell->fg_color, cell->bg_color);
        printf("%s %s",V_SEP,cell->content);
        n = n->next;
    }
    restoreScreen();
    return 0;
}