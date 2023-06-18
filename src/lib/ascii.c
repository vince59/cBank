/*
** Cbank 22/05/2023
** fonctions pour la gestion de l'interface texte
*/

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "libcbank.h"

struct termios old, new;

void init_ascii()
{
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    clearScreen();
    hideCursor();
}

void close_ascii()
{
    clearScreen();
    restoreScreen();
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

void setCursorLocation(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

void set_color(Color color)
{
    printf("\033[%d;%dm", color.bg_color + 10, color.fg_color);
}

// Function to hide the cursor
void hideCursor()
{
    printf("\033[?25l"); // Escape sequence to hide the cursor
}

// Function to show the cursor
void showCursor()
{
    printf("\033[?25h"); // Escape sequence to show the cursor
}

void clearScreen()
{
    printf("\033[%d;%dm", BLACK + 10, WHITE);
    printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and move the cursor to the top-left corner
}

void restoreScreen()
{
    printf("\033[0m"); // ANSI escape sequence to reset any text attributes and colors
    printf("\n");
}

void button(const char *message, Color color, int x, int y)
{

    set_color(color);
    setCursorLocation(x, y);
    printf("%s", message);
}

void draw_hline(int x, int y, int dx, Border border)
{
    set_color(border.color);
    setCursorLocation(x, y);
    printf("%s", I_SEP);
    for (int xi = x + 1; xi < x + dx; xi++)
    {
        printf("%s", H_SEP);
    }
    printf("%s", I_SEP);
}

void draw_vline(int x, int y, int dy, Border border)
{
    set_color(border.color);
    if (border.border_up == 1)
    {
        setCursorLocation(x, y);
        printf("%s", I_SEP);
    }
    for (int yi = y + 1; yi < y + dy; yi++)
    {
        setCursorLocation(x, yi);
        printf("%s", V_SEP);
    }
    if (border.border_up == 1)
    {
        setCursorLocation(x, y + dy);
        printf("%s", I_SEP);
    }
}

void draw_box(int x, int y, int dx, int dy, Border border, const char *title)
{
    if (border.border_up == 1)
        draw_hline(x, y, dx, border);
    if (border.border_down == 1)
        draw_hline(x, y + dy, dx, border);
    if (border.border_left == 1)
        draw_vline(x, y, dy, border);
    if (border.border_right == 1)
        draw_vline(x + dx, y, dy, border);
    int l = strlen(title);
    if (l > 0 && l < dx - 2)
    {
        setCursorLocation(x + (dx - l + 2) / 2, y);
        printf(" %s ", title);
    }
}

Cell *get_cell_at(Node *n, int l, int c)
{
    Cell *cell = NULL;
    while (n != NULL)
    {
        cell = n->data;
        if ((cell->l == l) && (cell->c == c))
            return cell;
        n = n->next;
    }
    return cell;
}

Array *new_array()
{
    Array *array = (Array *)malloc(sizeof(Array));
    array->nb_col = 0;
    array->nb_line = 0;
    array->cells = NULL;
    array->header = NULL;
    return array;
}

void set_header(Array *array, int nb_col)
{
    if (array->header != NULL)
        return;
    for (int c = 0; c < nb_col; c++)
    {
        Cell *cell = (Cell *)malloc(sizeof(Cell));
        cell->l = 1;
        cell->c = c + 1;
        cell->shift_x = 0;
        cell->nb_char = 0;
        array->nb_col++;

        add_node(&array->header, cell);
    }
}

void add_row(Array *array)
{
    array->nb_line++;
    for (int c = 0; c < array->nb_col; c++)
    {
        Cell *cell = (Cell *)malloc(sizeof(Cell));
        cell->l = array->nb_line;
        cell->c = c + 1;
        Cell *h = get_cell_at(array->header, 1, cell->c);
        cell->nb_char = h->nb_char;
        cell->shift_x = h->shift_x;
        cell->shift_y = array->nb_line - 1;
        add_node(&array->cells, cell);
    }
}

void set_cell(Array *array, int l, int c, Cell info)
{

    Cell *cell = NULL;
    if (l == 0) // header
        cell = get_cell_at(array->header, l + 1, c);
    else
        cell = get_cell_at(array->cells, l, c);
    if (cell != NULL)
    {
        cell->border = info.border;
        cell->color = info.color;
        cell->content = info.content;
        if (cell->nb_char == 0)
            cell->nb_char = info.nb_char;
        cell->orientation = info.orientation;
    }
}

void print_border(Cell *cell, int x, int y)
{
    int dx = 2 + cell->nb_char;
    int dy = 2;
    if (cell->border.border_up == 0)
        y--;
    draw_box(x, y, dx, dy, cell->border, "");
}

void print_cell(Cell *cell, int x, int y)
{
    print_border(cell, x, y);
    set_color(cell->color);
    char *str = malloc(sizeof(char) * cell->nb_char + 1);
    strncpy(str, cell->content, cell->nb_char);
    str[cell->nb_char] = '\0';
    switch (cell->orientation)
    {
        case CENTER :
            setCursorLocation(x + 1 + (cell->nb_char - strlen(str)) / 2, cell->border.border_up == 1 ? y + 1 : y);
            break;
        case RIGHT :
            setCursorLocation(x + 1 + (cell->nb_char - strlen(str)), cell->border.border_up == 1 ? y + 1 : y);
            break;
        case LEFT :
            setCursorLocation(x + 1, cell->border.border_up == 1 ? y + 1 : y);
            break;
        default :
            setCursorLocation(x + 1, cell->border.border_up == 1 ? y + 1 : y);
            break;
    }        
    printf("%s", str);
    free(str);
}

void print_cells(Node *cells, int x, int y)
{
    Cell *cell = NULL;
    Node *n = cells;
    while (n != NULL)
    {
        cell = n->data;
        print_cell(cell, x + cell->shift_x, y + cell->shift_y);
        n = n->next;
    }
}

void prepare_header(Array *array)
{
    Cell *cell = NULL;
    Node *n = array->header;
    int shift_x = 0;
    while (n != NULL)
    {
        cell = n->data;
        cell->shift_x += shift_x;
        shift_x += cell->nb_char + 2;
        cell->shift_y=0;
        n = n->next;
    }
}

void print_array(Array *array, int x, int y)
{
    print_cells(array->header, x, y);
    print_cells(array->cells, x, y + 3);
}

int wait_until(const char *s)
{
    int ch;
    while (1)
    {
        ch = getchar();
        if (find_char(s, ch)>-1)
            return ch;
    }
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