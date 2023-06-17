/*
** Cbank 22/05/2023
** fonctions pour la gestion de l'interface texte
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "libcbank.h"

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

void draw_hline(int x, int y, int dx, Color color)
{
    set_color(color);
    setCursorLocation(x, y);
    printf("%s", I_SEP);
    for (int xi = x + 1; xi < x + dx; xi++)
    {
        printf("%s", H_SEP);
    }
    printf("%s", I_SEP);
}

void draw_vline(int x, int y, int dy, Color color)
{
    set_color(color);
    setCursorLocation(x, y);
    printf("%s", I_SEP);
    for (int yi = y + 1; yi < y + dy; yi++)
    {
        setCursorLocation(x, yi);
        printf("%s", V_SEP);
    }
    setCursorLocation(x, y + dy);
    printf("%s", I_SEP);
}

void draw_box(int x, int y, int dx, int dy, Border border, const char *title)
{
    if (border.border_up == 1)
        draw_hline(x, y, dx, border.color);
    if (border.border_down == 1)
        draw_hline(x, y + dy, dx, border.color);
    if (border.border_left == 1)
        draw_vline(x, y, dy, border.color);
    if (border.border_right == 1)
        draw_vline(x + dx, y, dy, border.color);
    int l = strlen(title);
    if (l > 0 && l < dx - 2)
    {
        setCursorLocation(x + (dx - l + 2) / 2, y);
        printf(" %s ", title);
    }
}