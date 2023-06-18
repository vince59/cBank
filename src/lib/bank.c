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
    char *buttons[] = {"Catégories","Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 2; i++)
    {
        button(buttons[i], color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
}

void draw_array_button()
{
    Color color = {GREEN, BLACK};
    char *buttons[] = {"Suivant","Précédent","Début","Fin","Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 5; i++)
    {
        button(buttons[i], color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
}

void draw_welcome(const char *msg)
{
    Border border = {1,1,1,1,{YELLOW, BLACK}};
    draw_box(1, 1, 79, 18, border, msg);
}