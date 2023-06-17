#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "libcbank.h"

Cell *get_cell_at(Node *n, int l, int c)
{
    Cell *cell = NULL;
    while (n != NULL)
    {
        cell = n->data;
        if (cell->l == l && cell->c == c)
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
        cell->nb_char = info.nb_char;
        cell->orientation=info.orientation;
    }
}

void print_border(Cell *cell, int x, int y)
{
    int dx = 2 + cell->nb_char;
    int dy = 2;
    draw_box(x, y, dx, dy, cell->border, "");
}

void print_cell(Cell *cell, int x, int y)
{
    print_border(cell, x, y);
    set_color(cell->color);
    char *str = malloc(sizeof(char) * cell->nb_char + 1);
    strncpy(str, cell->content, cell->nb_char);
    str[cell->nb_char] = '\0';
    if (cell->orientation==CENTER)
        setCursorLocation(x + (cell->nb_char-strlen(str)) / 2, y+1);
    else
        setCursorLocation(x + 1, y + 1);
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
        cell->shift_x += (shift_x + 2);
        shift_x = cell->shift_x;
        n = n->next;
    }
}

void print_array(Array *array, int x, int y)
{
    print_cells(array->header, x, y);
    // print_cells(array->cells, x, y);
}

int main()
{
    clearScreen();
    struct termios old, new;
    int ch;

    // Disable terminal echo
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    Array *array = new_array(15, 3);
    int col_nb_char = 20;
    int nb_col = 1;
    int nb_line = 1;

    Color color1;
    color1.bg_color = GREEN;
    color1.fg_color = BLACK;

    Color color2;
    color2.bg_color = YELLOW;
    color2.fg_color = BLUE;

    Border border1;
    border1.border_down = 1;
    border1.border_left = 1;
    border1.border_right = 1;
    border1.border_up = 1;
    border1.color.bg_color = BLACK;
    border1.color.fg_color = WHITE;

    Border border2;
    border2.border_down = 0;
    border2.border_left = 1;
    border2.border_right = 1;
    border2.border_up = 0;
    border2.color.bg_color = BLACK;
    border2.color.fg_color = WHITE;

    // Entête de colonnes

    set_header(array, nb_col);

    for (int c = 0; c < nb_col; c++)
    {
        Cell cell;
        cell.content = (char *)malloc(sizeof(char) * col_nb_char);
        cell.color = color1;
        cell.border = border1;
        cell.orientation=CENTER;
        sprintf(cell.content, "HEADER %d", c + 1);
        if (c == 1)
            cell.nb_char = 20;
        else
            cell.nb_char = col_nb_char;
        set_cell(array, 0, c + 1, cell);
    }

    prepare_header(array);
    // Lignes

    for (int l = 0; l < nb_line; l++)
    {
        add_row(array);
        for (int c = 0; c < nb_col; c++)
        {
            Cell cell;
            cell.content = (char *)malloc(sizeof(char) * col_nb_char);
            cell.color = color2;
            cell.border = border2;
            cell.orientation=LEFT;
            sprintf(cell.content, "CELL %d %d", l + 1, c + 1);
            set_cell(array, l + 1, c + 1, cell);
        }
    }

    for (int i = 0; i < 20; i++)
        printf("123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    print_array(array, 2, 3);

    // Read input without echoing
    while (1)
    {
        ch = getchar();

        // Check for function keys
        if (ch == 'q')
            break;
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    restoreScreen();
    return 0;
}