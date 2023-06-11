#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;
extern Node *head_cat;
extern Node *head_acc;

#define UP_RIGHT_CORNER "+"   // coin supérieur droit
#define UP_LEFT_CORNER "+"    // coin supérieur gauche
#define DOWN_RIGHT_CORNER "+" // coin inférieur droit
#define DOWN_LEFT_CORNER "+"  // coin supérieur droit
#define LINE "-"              // ligne

#define MAX_X 80
#define MAX_Y 20

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
#define LIGHT_GRAY 248
#define LIGHT_RED 203
#define LIGHT_GREEN 120
#define LIGHT_BLUE 117

#define COLOR_RESET "\033[0m"

void setCursorLocation(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

void set_color(int fg_color, int bg_color)
{
    printf("\033[%d;%dm", bg_color,fg_color);
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
    
    set_color(fg_color, bg_color+10);
    setCursorLocation(x, y);
    printf("%s",message);
} 


int main()
{
    load_keyword();
    load_operation();
    load_category();
    load_account();
    clearScreen();
    
    char *buttons[] = {"Quitter","Charger","Categories","Comptes","Stat"};

    struct termios old, new;
    int ch;

    // Disable terminal echo
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    button(buttons[0],YELLOW,BLUE,0,20);
    button(buttons[1],YELLOW,BLUE,10,20);
    button(buttons[2],YELLOW,BLUE,19,20);
    
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
    free_list(head_kw);
    free_list(head_cat);
    free_list(head_op);
    free_list(head_acc);
    return 0;
}