#ifndef models
#define models
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define errno_item_not_found -5
#define errno_board_not_selected -6
#define errno_list_not_selected -7
#define errno_task_not_selected -8
#define errno_task_data_invalid -9
#define errno_task_prioirty_invalid -10

struct date {
    int year;
    int month; 
    int day;
};
// store task data
struct task {
    char name[500];
    int priority; // high priority <=> high number
    struct task *next; // task link list
    struct date *date;
};
// list struct and link list
// store list data and tasks added to that list
struct list {
    char name[500];
    struct list *next;
    struct task *my_tasks;
};
// board struct and link list
// store board data and lists of that board
struct board {
    char name[500];
    struct list *my_lists;
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

int check_task_input(int priority, int year, int month, int day);
struct task *init_tasks(char first_task_name[], int first_task_priority, int year, int month, int day);
int add_task(struct task *my_tasks, char name[], int priority, int year, int month, int day);
struct task *get_task(struct task *my_tasks, int task_number);
void show_tasks(struct task *my_tasks);
void show_single_task(struct task *task, int number);
int remove_task(struct list *list, struct task *task);
int move_task(struct task *task, struct list *source_task, struct list *target_list);
#endif