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

void set_color(int fg_color, int bg_color)
{
    printf("\033[%d;%dm", bg_color + 10, fg_color);
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

void button(const char *message, int fg_color, int bg_color, int x, int y)
{

    set_color(fg_color, bg_color);
    setCursorLocation(x, y);
    printf("%s", message);
}

void draw_hline(int x, int y, int dx, int fg_color, int bg_color)
{
    set_color(fg_color, bg_color);
    setCursorLocation(x, y);
    printf("%s", "+");
    for (int xi = x + 1; xi < x + dx; xi++)
    {
        printf("%s", "*");
    }
    printf("%s", "+");
}

void draw_vline(int x, int y, int dy, int fg_color, int bg_color)
{
    set_color(fg_color, bg_color);
    setCursorLocation(x, y);
    printf("%s", "+");
    for (int yi = y + 1; yi < y + dy; yi++)
    {
        setCursorLocation(x, yi);
        printf("%s", "*");
    }
    setCursorLocation(x, y + dy);
    printf("%s", "+");
}

void draw_box(int x, int y, int dx, int dy, int fg_color, int bg_color, const char *title)
{
    draw_hline(x, y, dx, fg_color, bg_color);
    draw_hline(x, y + dy, dx, fg_color, bg_color);
    draw_vline(x, y, dy, fg_color, bg_color);
    draw_vline(x + dx, y, dy, fg_color, bg_color);
    int l = strlen(title);
    if (l > 0 && l < dx - 2)
    {
        setCursorLocation(x + (dx - l + 2) / 2, y);
        printf(" %s ", title);
    }
}