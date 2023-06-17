#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "libcbank.h"

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
    int nb_col = 5;
    int nb_line = 2;

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
        cell.orientation = CENTER;
        if (c == 0 || c == 4)
        {
            sprintf(cell.content, "N°");
            cell.nb_char = 3;
        }
        else
        {
            sprintf(cell.content, "Header %d", c + 1);
            cell.nb_char = col_nb_char;
        }

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
            cell.orientation = LEFT;
            sprintf(cell.content, "CELL %d %d", l + 1, c + 1);
            set_cell(array, l + 1, c + 1, cell);
        }
    }

   // for (int i = 0; i < 20; i++)
   //     printf("123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    print_array(array, 1, 1);

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