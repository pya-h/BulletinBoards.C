#ifndef tasks_h
#define tasks_h

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

#endif
