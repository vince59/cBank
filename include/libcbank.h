
#ifndef LIBCBANK__H
#define LIBCBANK__H

/* Listes chainées */

typedef struct Node {
    void *data;
    struct Node* next;
} Node;

void add_node(Node** head, void* data);
void free_list(Node* head);

/* Comptes */

#define ACCOUNT_CSV "./data/account.csv"

#define ID_LENGTH 6
#define NAME_LENGTH 51

typedef struct Account {
    char id[ID_LENGTH];
    char name[NAME_LENGTH];
    float balance;
} Account;

void save_account();
void load_account();

/* Divers */

void about(void);

#endif