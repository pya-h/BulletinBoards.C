#ifndef BOARD
#define BOARD
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define errno_item_not_found -5
#define errno_board_not_selected -6
#define errno_list_not_selected -7
#define errno_task_not_selected -8
#define errno_task_data_invalid -9
#define errno_task_prioirty_invalid -10



// board struct and link list
// store board data and lists of that board
struct board {
    char name[500];
    struct list *my_lists;
    int lists_count;
    struct board *next;
};
// store user data
// username and password is required to login and register
// other variables are extra
struct user {
    char username[50];
    char password[30];
    int age;
    int folder_number;
    struct board *my_boards;
    int boards_count;
    int current_year;
    int current_month;
    int current_day;
    // user is connected to board data by this link list
    // then board is connected to lists by *lists link list
    // then *lists is connected to it's tasks by *tasks link list
};
struct board *init_boards(char first_board_name[]);
int add_board(struct board *my_boards, char name[]);
struct board *get_board(struct board *my_boards, int board_number);
void show_boards(struct board *my_boards);
int remove_board(struct user *user, struct board *board);

struct list *init_lists(char first_list_name[]);
int add_list(struct list *my_lists, char name[]);
struct list *get_list(struct list *my_lists, int list_number);
void show_lists(struct list *my_lists);
int remove_list(struct board *board, struct list *list);


#endif