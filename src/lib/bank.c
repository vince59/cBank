/*
** Cbank 22/05/2023
** fonctions pour la gestion banquaire
*/

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;
extern Node *head_cat;
extern Node *head_acc;

void load_db()
{
    load_keyword();
    load_operation();
    load_category();
    load_account();
}

void close_db()
{
    free_list(head_kw);
    free_list(head_cat);
    free_list(head_op);
    free_list(head_acc);
}

void draw_main_button()
{
    Color color = {GREEN, BLACK};
    char *buttons[] = {"Soldes", "Catégories", "Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 3; i++)
    {
        button(buttons[i], color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
}

void draw_array_button()
{
    Color color = {GREEN, BLACK};
    char *buttons[] = {"Premier", "+ Suivant", "- Précédent", "Dernier", "Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 5; i++)
    {
        button(buttons[i], color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
}

void draw_welcome(const char *msg)
{
    Border border = {1, 1, 1, 1, {YELLOW, BLACK}};
    draw_box(1, 1, 79, 18, border, msg);
}

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

Array *array_account(long int start, int max_line)
{
    Array *array = new_array();
    int nb_col = 3;
    Color header_color = {GREEN, BLACK};
    Color line_color = {WHITE, BLACK};
    Border header_border = {1, 1, 1, 1, {CYAN, BLACK}};
    Border line_border = {0, 0, 1, 1, {CYAN, BLACK}};

    set_header(array, nb_col);

    for (int c = 0; c < nb_col; c++)
    {
        int col_nb_char;
        Cell cell;
        switch (c)
        {
        case 0:
            col_nb_char = 40;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            sprintf(cell.content, "Compte");
            break;
        case 1:
            col_nb_char = 10;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            sprintf(cell.content, "Solde deb");
            break;
        case 2:
            col_nb_char = 10;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            sprintf(cell.content, "Solde fin");
            break;
        }
        cell.color = header_color;
        cell.border = header_border;
        cell.orientation = CENTER;
        cell.nb_char = col_nb_char;
        set_cell(array, 0, c + 1, cell);
    }

    prepare_header(array);
    int l = 0;
    Account *acc = NULL;
    Node *n = head_acc;
    float global_balance = 0;
    while (n != NULL)
    {
        acc = n->data;
        float balance = get_account_balance(acc);
        add_row(array);
        for (int c = 0; c < nb_col; c++)
        {
            Cell cell;
            cell.color = line_color;
            cell.border = line_border;
            cell.orientation = LEFT;
            int col_nb_char;
            switch (c)
            {
            case 0:
                col_nb_char = 32;
                cell.content = (char *)malloc(sizeof(char) * col_nb_char);
                sprintf(cell.content, "%s - %s", acc->id, acc->name);
                break;
            case 1:
                col_nb_char = 10;
                cell.content = (char *)malloc(sizeof(char) * col_nb_char);
                sprintf(cell.content, "%.2f", acc->balance);
                cell.orientation = RIGHT;
                break;
            case 2:
                col_nb_char = 10;
                cell.content = (char *)malloc(sizeof(char) * col_nb_char);
                sprintf(cell.content, "%.2f", balance);
                cell.orientation = RIGHT;
                break;
            }
            set_cell(array, l + 1, c + 1, cell);
        }
        l++;
        global_balance += balance;

        n = n->next;
    }
    add_row(array);
    for (int c = 0; c < nb_col; c++)
    {
        Cell cell;
        cell.color = line_color;
        cell.border = line_border;
        cell.orientation = RIGHT;
        int col_nb_char;
        switch (c)
        {
        case 0:
            col_nb_char = 32;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            sprintf(cell.content, "Total des liquidités");
            break;
        case 1:
            col_nb_char = 10;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            sprintf(cell.content, " ");
            break;
        case 2:
            col_nb_char = 10;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            cell.color.fg_color=RED;
            sprintf(cell.content, "%.2f", global_balance);
            break;
        }
        set_cell(array, l + 1, c + 1, cell);
    }
    return array;
}

void free_array(Array *array)
{
    free_cells(array->header);
    free_cells(array->cells);
    free_list(array->header);
    free_list(array->cells);
    free(array);
}

void init_dsp_array(const char *title)
{
    clearScreen();
    draw_welcome(title);
    draw_array_button();
}

void dsp_array(Array *(*func_array)(long int, int), const char *title, long total, int nb, int x, int y)
{
    init_dsp_array(title);
    long start = 1;
    Array *array = func_array(start, nb);
    print_array(array, x, y);
    for (;;)
    {
        int ch = wait_until("p+-dq");
        switch (ch)
        {
        case '+':
            if (array->nb_line >= nb)
                start += nb;
            break;
        case '-':
            start -= nb;
            if (start < 0)
                start = 1;
            break;
        case 'p':
            start = 1;
            break;
        case 'd':
            start = total - nb + 1;
            break;
        }
        free_array(array);
        if (ch == 'q')
            break;
        init_dsp_array(title);
        array = func_array(start, nb);
        print_array(array, x, y);
    }
}

void dsp_cat()
{
    long nb_cat = nb_node(head_cat);
    dsp_array(array_categories, " Liste des catégories ", nb_cat, 14, 2, 2);
}

void dsp_acc()
{
    long nb_acc = nb_node(head_acc);
    dsp_array(array_account, " Solde des comptes ", nb_acc, 14, 2, 2);
}

void init_dsp_main()
{
    clearScreen();
    draw_welcome("   C_BANK 1.1   ");
    draw_main_button();
}