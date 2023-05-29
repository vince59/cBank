
#ifndef LIBCBANK__H
#define LIBCBANK__H

typedef unsigned long Id; 

/* Listes chainées */

typedef struct Node {
    void *data;
    struct Node* next;
} Node;

void add_node(Node** head, void* data);
void free_list(Node* head);

/* Comptes */

#define ACCOUNT_CSV "./data/account.csv"

#define ACC_ID_LENGTH 6
#define ACC_NAME_LENGTH 51

typedef struct Account {
    char id[ACC_ID_LENGTH];
    char name[ACC_NAME_LENGTH];
    float balance;
} Account;

void save_account();
void load_account();

/* Opération */

#define OPERATION_CSV "./data/operation.csv"

#define OP_LIB_LENGTH 100

typedef struct Operation {
    Id id;
    char account_id[ACC_ID_LENGTH];
    long date;
    char bank_lib1[OP_LIB_LENGTH];
    char bank_lib2[OP_LIB_LENGTH];
    char bank_ref[OP_LIB_LENGTH];
    char bank_info[OP_LIB_LENGTH];
    char bank_type[OP_LIB_LENGTH];
    char bank_category[OP_LIB_LENGTH];
    char bank_sub_category[OP_LIB_LENGTH];
    Id category_id;
    float amount;
} Operation;

void save_operation();
void load_operation();

/* Divers */

void about(void);

#endif