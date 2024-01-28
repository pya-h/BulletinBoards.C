#ifndef TASK
#define TASK

struct DEADLINE {
    int Y, M, D;
};

// store task data
struct task {
    char name[500];
    int priority; // high priority <=> high number
    struct task *next; // task link list
    int date_year;
    int date_month;
    int date_day;
    struct DEADLINE Deadline;
};

int check_task_input(int priority, int year, int month, int day);
struct task *init_tasks(char first_task_name[], int first_task_priority, int year, int month, int day);
int add_task(struct task *my_tasks, char name[], int priority, int year, int month, int day);
struct task *get_task(struct task *my_tasks, int task_number);
void show_tasks(struct task *my_tasks);
void show_single_task(struct task *task, int number);
int remove_task(struct list *list, struct task *task);
int move_task(struct task *task, struct list *source_task, struct list *target_list);
struct task* get_new_task(char name[], int priority, int year, int month, int day);
void sort_list(struct list *list, int sort_by);
int date_to_days(int year, int month, int day);
#endif