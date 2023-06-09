
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
float get_account_balance(Account *acc);
float get_balance(void);
float get_initial_balance(void);

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
Category *add_category(const char *name);
void print_categories(void);
Category *find_category_by_id(int cat_id);
int auto_set_category_ops(void);

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
int print_operations(char info_type);
void print_operation(Operation *op);
void print_operation_light(Operation *op);
Id next_operation_id(void);
int chk_duplicate_operation(Operation *op_curr);
Node *find_ops_by_cat_id(Node *hop,int cat_id);
Operation * get_last_op(void);
Node *sort_operation_by_date(Node *head);

/* Mots clés */

#define KW_CSV "./data/keyword.csv"

#define KW_NAME_LENGTH 51

typedef struct Keyword
{
    int id;
    int category_id;
    char name[KW_NAME_LENGTH];
} Keyword;

typedef struct Scoring
{
    int id;
    int category_id;
    int nb_kw;
    int nb_match;
    float score;
    Node *kw;
} Scoring;

int save_keyword(void);
int load_keyword(void);
Node *extract_kw_str(Node *h_kw, const char *p);
Node *extract_kw_op(Operation *op);
Keyword *find_kw_by_name(Node *hkw, char *kw_name);
int next_list_id(void);
int add_keyword(int list_id, int cat_id, char *str);
Node *struct_kw(void);
void scoring_operation(Node *head_sc, char *kw_name);
void raz_scoring(Node *n);
int best_score(Node *n, int *nb);


/* Stat */

typedef struct Stat
{
    long date;
    int year;
    int month;
    int day;
    int nb;
    float amount;
    Category *cat;
    Node *list;
} Stat;

Node * get_balance_by_month(void);
Stat *find_stat_by_ym(Node *n, int year, int month);
Stat *find_stat_by_cat_id(Node *n, int cat_id);

/* Divers */

void about(char *msg);
int is_file_exist(char *filename);
// date
long cnv_date(char *sdate);
char *fmt_date(long date);
void get_ymd(long date, int *year, int *month, int *day);
char *fmt_int_date(int year, int month, int day);
long fmt_long_date(int year, int month, int day);
char *fmt_fr_date(int year, int month, int day);
void today (int *year, int *month, int *day);
int get_days_in_month(int month, int year);

// convert
float cnv_float(char *sfloat);
void replace_substring(char *str, const char *find, const char *replace);
void remove_last_char(char* str);

#endif