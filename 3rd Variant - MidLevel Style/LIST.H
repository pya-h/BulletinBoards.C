#ifndef LIST
#define LIST

// List Struct
struct list {
    char name[500];
    struct list *next;
    struct task *my_tasks;
    int tasks_count;
};

#endif