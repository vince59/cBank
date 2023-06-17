#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;
extern Node *head_cat;
extern Node *head_acc;

void add_column(Node* head_col,char * title)
{
    add_node(&head_col, title);
}

int main()
{
    load_keyword();
    load_operation();
    load_category();
    load_account();
    clearScreen();

    char *buttons[] = {"Quitter", "Charger", "Categories", "Comptes", "Stat"};

    struct termios old, new;
    int ch;

    // Disable terminal echo
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    int pos_x = 1;
    for (int i = 0; i < 5; i++)
    {
        button(buttons[i], YELLOW, BLUE, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }

    draw_box(1, 1, 70, 8, GREEN, BLACK, "Liste des catégories");

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