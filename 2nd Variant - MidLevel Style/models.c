#include "models.h"

struct board *init_boards(char first_board_name[]) {
    // this is for init user->myboards
    struct board* my_boards = (struct board *) malloc(sizeof(struct board));
    sprintf(my_boards->name, "%s", first_board_name);
    my_boards->next = NULL;
    my_boards->my_lists = NULL;
    return my_boards;
}

int add_board(struct board *my_boards, char name[]) {
    // Because if the my_boards is empty, it needs to update its address
    struct board* b = my_boards;
    struct board* new_board = (struct board*) malloc(sizeof(struct user));
    sprintf(new_board->name, "%s", name);
    int board_number = 1;
    while(b->next != NULL) {
        b = b->next;
        board_number++;
    }
    b->next = new_board;
    b->next->my_lists = NULL; // New board has no lists.
    b->next->next = NULL;
    return board_number;
}

struct board *get_board(struct board *my_boards, int board_number) {
    struct board* board = my_boards;
    int i;
    if(board_number <= 0)
        return NULL;
    for(i = 1; board != NULL && i < board_number; i++)
        board = board->next;
    return board;
}
void show_boards(struct board *my_boards) {
    struct board* b = my_boards;
    int count = 0;
    while(b != NULL) {
        printf("%s\n", b->name);
        b = b->next;
        count++;
    }
    printf("Total count: %d\n", count);
}

struct list *init_lists(char first_list_name[]) {
    // this is for init user->mylists
    struct list* my_lists = (struct list *) malloc(sizeof(struct list));
    sprintf(my_lists->name, "%s", first_list_name);
    my_lists->next = NULL;
    my_lists->my_tasks = NULL; // New list has no tasks
    return my_lists;
}

int add_list(struct list *my_lists, char name[]) {
    // Because if the my_lists is empty, it needs to update its address
    struct list* lst = my_lists;
    struct list* new_list = (struct list*) malloc(sizeof(struct user));
    sprintf(new_list->name, "%s", name);
    int list_number = 1;
    while(lst->next != NULL) {
        lst = lst->next;
        list_number++;
    }
    lst->next = new_list;
    lst->next->my_tasks = NULL; // New list has no tasks
    lst->next->next = NULL;
    return list_number;
}

struct list *get_list(struct list *my_lists, int list_number) {
    // Get the list by list_number
    // Go next in link list list_number-1 times
    struct list* list = my_lists;
    int i;
    if(list_number <= 0)
        return NULL;
    for(i = 1; list != NULL && i < list_number; i++)
        list = list->next;
    return list;
}

void show_lists(struct list *my_lists) {
    struct list* lst = my_lists;
    int count = 0;
    while(lst != NULL) {
        printf("%s\n", lst->name);
        lst = lst->next;
        count++;
    }
    printf("Total count: %d\n", count);
}

int check_task_input(int priority, int year, int month, int day) {
    if(priority < 0 || priority > 2)
        return errno_task_prioirty_invalid;
    if(year <= 0 || month <= 0 || day <= 0 || month > 12 || day > 31)
        return errno_task_data_invalid;
    return 1;
}

struct task *init_tasks(char first_task_name[], int first_task_priority, int year, int month, int day) {
    // this is for init user->mytasks
    struct task* my_tasks = (struct task *) malloc(sizeof(struct task));
    sprintf(my_tasks->name, "%s", first_task_name);
    my_tasks->priority = first_task_priority;
    my_tasks->date.year = year;
    my_tasks->date.month = month;
    my_tasks->date.day = day;
    my_tasks->next = NULL;
    return my_tasks;
}

int add_task(struct task *my_tasks, char name[], int priority, int year, int month, int day) {
    // Because if the my_tasks is empty, it needs to update its address
    struct task* task = my_tasks;
    struct task* new_task = (struct task*) malloc(sizeof(struct user));
    sprintf(new_task->name, "%s", name);
    int task_number = 1;
    while(task->next != NULL) {
        task = task->next;
        task_number++;
    }
    task->next = new_task;
    task->next->next = NULL;
    task->next->priority = priority;
    task->next->date.year = year;
    task->next->date.month = month;
    task->next->date.day = day;
    return task_number;
}

struct task *get_task(struct task *my_tasks, int task_number) {
    // Get the task by task_number
    // Go next in link task task_number-1 times
    struct task* task = my_tasks;
    int i;
    if(task_number <= 0)
        return NULL;
    for(i = 1; task != NULL && i < task_number; i++)
        task = task->next;
    return task;
}

void show_tasks(struct task *my_tasks) {
    struct task* task = my_tasks;
    int count = 0;
    char *priority;
    while(task != NULL) {
        if(task->priority == 2)
            priority = "High";
        else if(task->priority == 1)
            priority = "Medium";
        else
            priority = "Low";
        printf("%s, %s, %d/%d/%d\n", task->name, priority, task->date.year, task->date.month, task->date.day);
        task = task->next;
        count++;
    }
    printf("Total count: %d\n", count);
}