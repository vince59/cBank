
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
long int nb_node(Node* head);

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

/* Ascii */

#define V_SEP "|"
#define H_SEP "-"
#define I_SEP "+"

#define MAX_X 80
#define MAX_Y 20

#define BLACK 49
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

#define COLOR_RESET "\033[0m"

//Orientation

#define LEFT 1
#define CENTER 2
#define RIGHT 3

typedef struct Color
{
    int fg_color, bg_color;
} Color; 

typedef struct Border
{
    int border_up, border_down, border_left, border_right;
    Color color;
} Border;

typedef struct Cell
{
    char *content;
    int l,c;
    Color color;
    int nb_char;
    Border border;
    int shift_x, shift_y;
    int orientation;
} Cell;

typedef struct Array
{
    Node *cells;
    Node *header;
    int nb_line;
    int nb_col;
} Array;

void setCursorLocation(int x, int y);
void set_color(Color color);
void hideCursor();
void showCursor();
void clearScreen();
void restoreScreen();
void button(const char *message, Color color, int x, int y);
void draw_hline(int x, int y, int dx, Border border);
void draw_vline(int x, int y, int dy, Border border);
void draw_box(int x, int y, int dx, int dy, Border border, const char *title);
Cell *get_cell_at(Node *n, int l, int c);
Array *new_array();
void set_header(Array *array, int nb_col);
void add_row(Array *array);
void set_cell(Array *array, int l, int c, Cell info);
void print_border(Cell *cell, int x, int y);
void print_cell(Cell *cell, int x, int y);
void print_cells(Node *cells, int x, int y);
void prepare_header(Array *array);
void print_array(Array *array, int x, int y);
void init_ascii();
void close_ascii();
int wait_until(const char *s);
void free_cells(Node *n);

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

// string
float cnv_float(char *sfloat);
void replace_substring(char *str, const char *find, const char *replace);
void remove_last_char(char* str);
int find_char(const char *str, int target);

// Cbank
void load_db();
void close_db();
void draw_main_button();
void draw_main_menu();
void draw_array_button();
void draw_welcome(const char *msg);
Array *array_categories(long int start, int max_line);
void free_array(Array *array);
void init_dsp_array(const char *title);
void dsp_cat();
void init_dsp_main();
void dsp_acc();
void dsp_stat();
#endif