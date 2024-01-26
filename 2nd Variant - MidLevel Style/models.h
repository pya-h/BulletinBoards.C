#ifndef models
#define models
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

struct date {
    int year, month, day;
};
// store task data
struct task {
    char name[500];
    int priority; // high priority <=> high number
    struct date date;
    struct task *next; // task link list
};
// list struct and link list
// store list data and tasks added to that list
struct list {
    char name[500];
    struct task *my_tasks;
    struct list *next;
    int tasks_file_start; // in file
};
// board struct and link list
// store board data and lists of that board
struct board {
    char name[500];
    struct list *my_lists;
    struct board *next;
    int lists_file_start; // in file
};
// store user data
// username and password is required to login and register
// other variables are extra
struct user {
    char username[50],
        password[30],
        first_name[50],
        last_name[50],
        email[200];
    int gender, age;
    struct board *my_boards;
    // user is connected to board data by this link list
    // then board is connected to lists by *lists link list
    // then *lists is connected to it's tasks by *tasks link list
};

#endif