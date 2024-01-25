#ifndef lists_h
#define lists_h

// list struct and link list
// store list data and tasks added to that list
struct list
{
    char name[500];
    struct task *tasks;
    struct list *next;
};

#endif
