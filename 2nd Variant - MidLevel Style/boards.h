#ifndef boards_h
#define boards_h
#include <lists.h>

// board struct and link list
// store board data and lists of that board
struct board
{
    char name[500];
    struct list *lists;
    struct board *next;

};

#endif
