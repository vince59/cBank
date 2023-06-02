
#ifndef LIBCBANK__H
#define LIBCBANK__H

typedef unsigned long Id;

/* Listes chainées */

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

void add_node(Node **head, void *data);
void free_list(Node *head);

/* Comptes */

#define ACCOUNT_CSV "./data/account.csv"

#define ACC_ID_LENGTH 6
#define ACC_NAME_LENGTH 51

typedef struct Account
{
    char id[ACC_ID_LENGTH];
    char name[ACC_NAME_LENGTH];
    float balance;
} Account;

int save_account(void);
int load_account(void);
Account *find_acc_by_id(char *acc_id);

/* Catégorie */

#define CAT_CSV "./data/category.csv"

#define CAT_NAME_LENGTH 51

typedef struct Category
{
    int id;
    char name[CAT_NAME_LENGTH];
} Category;

int save_category(void);
int load_category(void);

/* Mots clés */

#define KW_CSV "./data/keyword.csv"

#define KW_NAME_LENGTH 51

typedef struct Keyword
{
    int id;
    int category_id;
    char name[KW_NAME_LENGTH];
} Keyword;

int save_keyword(void);
int load_keyword(void);

/* Opération */

#define OPERATION_CSV "./data/operation.csv"

#define OP_LIB_LENGTH 100

typedef struct Operation
{
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
    int category_id;
    float amount;
} Operation;

int save_operation(void);
int load_operation(void);
void print_operations(void);
Id next_operation_id(void);
int chk_duplicate_operation(Operation *op_curr);

/* Divers */

void about(char *msg);
long cnv_date(char *sdate);
float cnv_float(char *sfloat);
void replaceSubstring(char *str, const char *find, const char *replace);

#endif