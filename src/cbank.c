#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;
extern Node *head_cat;
extern Node *head_acc;

Array *array_categories(long int start, int max_line)
{
    Array *array = new_array();
    int nb_col = 2;
    Color header_color = {GREEN, BLACK};
    Color line_color = {WHITE, BLACK};
    Border header_border = {1, 1, 1, 1, {CYAN, BLACK}};
    Border line_border = {0, 0, 1, 1, {CYAN, BLACK}};

    set_header(array, nb_col);

    for (int c = 0; c < nb_col; c++)
    {
        int col_nb_char = (c == 0 ? 4 : 20);

        Cell cell;
        cell.content = (char *)malloc(sizeof(char) * col_nb_char);
        cell.color = header_color;
        cell.border = header_border;
        cell.orientation = CENTER;
        sprintf(cell.content, c == 0 ? "ID" : "Libellé");
        cell.nb_char = col_nb_char;
        set_cell(array, 0, c + 1, cell);
    }

    prepare_header(array);

    Category *cat = NULL;
    Node *n = head_cat;
    int l = 0;
    long int i = 0;
    while (n != NULL)
    {
        cat = n->data;
        if (i >= start - 1)
        {
            add_row(array);
            for (int c = 0; c < nb_col; c++)
            {
                Cell cell;
                cell.content = (char *)malloc(sizeof(char) * (c == 0 ? 5 : strlen(cat->name)));
                cell.color = line_color;
                cell.border = line_border;
                cell.orientation = LEFT;
                if (c == 0)
                    sprintf(cell.content, "%d", cat->id);
                else
                    sprintf(cell.content, "%s", cat->name);
                set_cell(array, l + 1, c + 1, cell);
            }
            l++;
        }
        i++;
        if (l == max_line)
            break;
        n = n->next;
    }
    return array;
}

void free_cells(Node *n)
{
    Cell *cell = NULL;
    while (n != NULL)
    {
        cell = n->data;
        free(cell->content);
        n = n->next;
    }
}

void free_array(Array *array)
{
    free_cells(array->header);
    free_cells(array->cells);
    free_list(array->header);
    free_list(array->cells);
    free(array);
}

void init_dsp_cat()
{
    clearScreen();
    draw_welcome(" Liste des catégories ");
    draw_array_button();
}

void dsp_cat()
{
    init_dsp_cat();
    long int nb_cat = nb_node(head_cat);
    Array *array_cat;
    long start = 1;
    int nb = 14;
    array_cat = array_categories(start, nb);
    print_array(array_cat, 2, 2);
    for (;;)
    {
        int ch = wait_until("spdfq");
        switch (ch)
        {
        case 's':
            start += nb;
            if (start > nb_cat)
                start = nb_cat - nb;
            break;
        case 'p':
            start -= nb;
            if (start < 0)
                start = 1;
            break;
        case 'd':
            start = 1;
            break;
        case 'f':
            start = nb_cat - nb+1;
            break;
        }
        free_array(array_cat);
        if (ch == 'q')
            break;
        init_dsp_cat();
        array_cat = array_categories(start, nb);
        print_array(array_cat, 2, 2);
    }
}

void init_dsp_main()
{
    clearScreen();
    draw_welcome("   C_BANK 1.1   ");
    draw_main_button();
}

int main()
{
    load_db();
    init_ascii();
    for (;;)
    {
        init_dsp_main();
        int ch = wait_until("qc");
        if (ch == 'c')
        {
            dsp_cat();
        }
        if (ch == 'q')
            break;
    }
    close_db();
    close_ascii();
    return 0;
}