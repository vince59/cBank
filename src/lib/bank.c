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

Color header_color = {GREEN, BLACK};
Color line_color = {WHITE, BLACK};
Border header_border = {1, 1, 1, 1, {CYAN, BLACK}};
Border line_border = {0, 0, 1, 1, {CYAN, BLACK}};
Color menu_color = {GREEN, BLACK};
Color red_black = {RED, BLACK};

/****************************
 *
 *  CHARGEMENT DU FICHIER BANCAIRE BANQUE POP DU NORD
 *
 */

void load(char *acc_id, char *file_name, int x, int *y)
{
    int nb_dup;
    int nb_bpn_op = load_bpn_csv(acc_id, file_name, &nb_dup);
    char str[50];
    sprintf(str, "\tNombre de nouvelles opérations : %d\n", nb_bpn_op);
    print(str, red_black, x, (*y)++);
    sprintf(str, "\tNombre de doublons             : %d\n", nb_dup);
    print(str, red_black, x, (*y)++);
}

void load_bpn()
{
    clearScreen();
    draw_welcome(" Chargement fichier BPN ");

    int year, month, day;
    today(&year, &month, &day);
    char *date = fmt_fr_date(year, month, day);
    char ccbpn[200], liva[200], ldd[200], str[400];
    sprintf(ccbpn, "/home/vincent/Téléchargements/%s_85391.csv", date);
    sprintf(ldd, "/home/vincent/Téléchargements/%s_85392.csv", date);
    sprintf(liva, "/home/vincent/Téléchargements/%s_85394.csv", date);

    int x = 2, y = 4;

    sprintf(str, "Traitement du fichier %s (CCBPN)\n", ccbpn);
    print(str, line_color, x, y++);
    if (is_file_exist(ccbpn))
        load("CCBPN", ccbpn, x, &y);
    else
        print("\tFichier non trouvé !", header_color, x, y++);

    sprintf(str, "Traitement du fichier %s (LIVA)\n", liva);
    print(str, line_color, x, y++);
    if (is_file_exist(liva))
        load("LIVA", liva, x, &y);
    else
        print("\tFichier non trouvé !", header_color, x, y++);

    sprintf(str, "Traitement du fichier %s (LDD)\n", ldd);
    print(str, line_color, x, y++);
    if (is_file_exist(ldd))
        load("LDD", ldd, x, &y);
    else
        print("\tFichier non trouvé !", header_color, x, y++);

    save_operation();
    button("Quitter", menu_color, 1, 20);
    wait_until("q");
}

/****************************
 *
 *  STATISTIQUES
 *
 */

void dsp_stat()
{
    long nb = nb_node(head_op);
    float balance = get_initial_balance();
    float dispo = get_account_balance(find_acc_by_id("CCBPN"));
    int year, month, day;
    today(&year, &month, &day);
    int nb_days = get_days_in_month(month, year);
    char str[50];

    clearScreen();
    draw_welcome(" Statistiques ");

    sprintf(str, "Nombre d'opérations           : %lu\n", nb);
    print(str, line_color, 10, 3);
    sprintf(str, "Date de la dernière opération : %s\n", fmt_date(get_last_op()->date));
    print(str, line_color, 10, 4);
    sprintf(str, "Solde départ au 31/12/2022    : %.2f\n", balance);
    print(str, line_color, 10, 5);
    sprintf(str, "Somme des liquidités          : %.2f\n", get_balance());
    print(str, line_color, 10, 6);
    sprintf(str, "Dispo par jour                : %.2f\n", dispo / (nb_days - day));
    print(str, line_color, 10, 7);

    button("Quitter", menu_color, 1, 20);
    wait_until("q");
}

/****************************
 *
 *  TABLEAU DES CATEGORIES
 *
 */

void dsp_cat()
{
    Array *array = new_array(" Liste des catégories ");
    int nb_col = 2;

    // constitution du header du tableau

    set_header(array, nb_col);
    Cell cell = get_header_style_cell();
    for (int c = 0; c < nb_col; c++)
    {
        int col_nb_char = (c == 0 ? 4 : 20);
        cell.content = (char *)malloc(sizeof(char) * col_nb_char);
        sprintf(cell.content, c == 0 ? "ID" : "Libellé");
        cell.nb_char = col_nb_char;
        set_cell(array, 0, c + 1, cell);
    }

    prepare_header(array);

    // alimentation des lignes du tableau

    Category *cat = NULL;
    Node *n = head_cat;
    int l = 0;
    cell = get_line_style_cell();
    while (n != NULL)
    {
        cat = n->data;
        add_row(array);
        for (int c = 0; c < nb_col; c++)
        {
            cell.content = (char *)malloc(sizeof(char) * (c == 0 ? 5 : strlen(cat->name)));
            if (c == 0)
                sprintf(cell.content, "%d", cat->id);
            else
                sprintf(cell.content, "%s", cat->name);
            set_cell(array, l + 1, c + 1, cell);
        }
        l++;
        n = n->next;
    }
    dsp_array(array);
}

/****************************
 *
 *  TABLEAU DES COMPTES
 *
 */

void dsp_acc()
{
    Array *array = new_array(" Liste des comptes ");
    int nb_col = 3;

    // constitution du header du tableau

    set_header(array, nb_col);
    Cell cell = get_header_style_cell();
    for (int c = 0; c < nb_col; c++)
    {
        int col_nb_char;
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
        cell.nb_char = col_nb_char;
        set_cell(array, 0, c + 1, cell);
    }

    prepare_header(array);

    // alimentation des lignes du tableau

    int l = 0;
    Account *acc = NULL;
    Node *n = head_acc;
    float global_balance = 0;
    cell = get_line_style_cell();
    while (n != NULL)
    {
        acc = n->data;
        float balance = get_account_balance(acc);
        add_row(array);
        for (int c = 0; c < nb_col; c++)
        {
            int col_nb_char;
            switch (c)
            {
            case 0:
                col_nb_char = 32;
                cell.content = (char *)malloc(sizeof(char) * col_nb_char);
                sprintf(cell.content, "%s - %s", acc->id, acc->name);
                cell.orientation = LEFT;
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

    // Alimentation du footer

    add_row(array);
    cell.orientation = RIGHT;
    for (int c = 0; c < nb_col; c++)
    {
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
            cell.color.fg_color = RED;
            sprintf(cell.content, "%.2f", global_balance);
            break;
        }
        set_cell(array, l + 1, c + 1, cell);
    }
    dsp_array(array);
}

/****************************
 *
 *  FONCTION GENERIQUES GESTION TABLEAU
 *
 */

void free_array(Array *array)
{
    free_cells(array->header);
    free_cells(array->cells);
    free_list(array->header);
    free_list(array->cells);
    free(array->title);
    free(array);
}

void init_dsp_array(Array *array, long page)
{
    clearScreen();
    draw_welcome(array->title);
    draw_array_button(array, page);
}

void draw_array_button(Array *array, long page)
{
    Color color = {GREEN, BLACK};
    char *buttons[] = {"Premier", "+ Suivant", "- Précédent", "Dernier", "Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 5; i++)
    {
        button(buttons[i], color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
    char mess[10];
    sprintf(mess, "%lu / %lu", page, divide_rounded_up(array->nb_line, array->line_page));
    print(mess, color, pos_x, 20);
}

void dsp_array(Array *array)
{
    long page = 1;

    print_array(array, page);
    for (;;)
    {
        int ch = wait_until("p+-dq");
        switch (ch)
        {
        case '+':
            if (array->nb_line >= (page * array->line_page))
                page++;
            break;
        case '-':
            if (page - 1 > 0)
                page--;
            break;
        case 'p':
            page = 1;
            break;
        case 'd':
            page = divide_rounded_up(array->nb_line, array->line_page);
            break;
        }
        if (ch == 'q')
            break;
        print_array(array, page);
    }
    free_array(array);
}

/****************************
 *
 *  FONCTION GENERIQUES BDD et AFFICHAGE
 *
 */

void init_dsp_main()
{
    clearScreen();
    draw_welcome("   C_BANK 1.1   ");
    draw_main_button();
    draw_main_menu();
}

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

void draw_main_menu()
{
    char *menu[] = {"1 - Statistiques", "2 - Soldes", "3 - Catégories", "4 - Chargement"};
    for (int i = 0; i < 4; i++)
    {
        button(menu[i], menu_color, 3, i + 3);
    }
}

void draw_main_button()
{
    char *buttons[] = {"Quitter"};
    int pos_x = 1;
    for (int i = 0; i < 1; i++)
    {
        button(buttons[i], menu_color, pos_x, 20);
        pos_x += strlen(buttons[i]) + 2;
    }
}

void draw_welcome(const char *msg)
{
    Border border = {1, 1, 1, 1, {YELLOW, BLACK}};
    draw_box(1, 1, 79, 18, border, msg);
}

Cell get_header_style_cell()
{
    Cell cell;
    cell.color = header_color;
    cell.border = header_border;
    cell.orientation = CENTER;
    return cell;
}

Cell get_line_style_cell()
{
    Cell cell;
    cell.color = line_color;
    cell.border = line_border;
    cell.orientation = LEFT;
    return cell;
}

void print_header(Array *array)
{
    Cell *cell = NULL;
    Node *n = array->header;
    while (n != NULL)
    {
        cell = n->data;
        print_cell(cell, array->x + cell->shift_x, array->y);
        n = n->next;
    }
}

void print_line(Array *array, long page)
{
    Cell *cell = NULL;
    Node *n = array->cells;
    int i = -1;
    long last_l = 0;
    while (n != NULL)
    {
        cell = n->data;
        if (cell->l > (page - 1) * array->line_page && cell->l <= page * array->line_page)
        {
            if (cell->l != last_l)
            {
                i++;
                last_l = cell->l;
            }
            print_cell(cell, array->x + cell->shift_x, array->y + 3 + i);
        }
        n = n->next;
    }
}

void print_array(Array *array, long page)
{
    init_dsp_array(array, page);
    print_header(array);
    print_line(array, page);
}
