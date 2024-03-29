#include "models.h"

struct board *init_boards(char first_board_name[]) {
    // this is for init user->myboards
    struct board* my_boards = (struct board *) calloc(1, sizeof(struct board));
    sprintf(my_boards->name, "%s", first_board_name);
    my_boards->next = NULL;
    my_boards->my_lists = NULL;
    my_boards->lists_count = 0;
    return my_boards;
}

int add_board(struct board *my_boards, char name[]) {
    // Because if the my_boards is empty, it needs to update its address
    struct board* b = my_boards;
    struct board* new_board = (struct board*) calloc(1, sizeof(struct user));
    sprintf(new_board->name, "%s", name);
    new_board->next = NULL;
    new_board->my_lists = NULL; // New board has no lists.
    new_board->lists_count = 0;
    new_board->next = NULL;
    int board_number = 1;
    while(b->next != NULL) {
        b = b->next;
        board_number++;
    }
    b->next = new_board;

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
        count++;
        printf("%d: %s\n", count, b->name);
        b = b->next;
    }
    printf("Total count: %d\n", count);
}

int remove_board(struct user *user, struct board *board) {
    struct board *previous_board = user->my_boards;
    struct list *list, *temp;
    if(board == NULL)
        return errno_board_not_selected;

    if(board == user->my_boards) { // If its the first item, update my_lists pointer
        user->my_boards = user->my_boards->next;
    } else {
        while(previous_board != NULL &&  previous_board != board && previous_board->next != board)
            previous_board = previous_board->next;
        if(previous_board == NULL)
            return errno_item_not_found;
        previous_board->next = board->next; // Remove board from Link list
    }
    list = board->my_lists; // remove board lists too.
    while(list != NULL) {
        temp = list->next;
        remove_list(board, list);
        list = temp;
    }
    free(board);
    return 1;
}

struct list *init_lists(char first_list_name[]) {
    // this is for init user->mylists
    struct list* my_lists = (struct list *) calloc(1, sizeof(struct list));
    sprintf(my_lists->name, "%s", first_list_name);
    my_lists->next = NULL;
    my_lists->my_tasks = NULL; // New list has no tasks
    my_lists->tasks_count = 0;
    return my_lists;
}

int add_list(struct list *my_lists, char name[]) {
    // Because if the my_lists is empty, it needs to update its address
    struct list* lst = my_lists;
    struct list* new_list = (struct list*) calloc(1, sizeof(struct user));
    sprintf(new_list->name, "%s", name);
    new_list->tasks_count = 0;
    new_list->next = NULL;
    new_list->my_tasks = NULL; // New list has no tasks

    int list_number = 1;
    while(lst->next != NULL) {
        lst = lst->next;
        list_number++;
    }
    lst->next = new_list;
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
        count++;
        printf("%d: %s\n", count, lst->name);
        lst = lst->next;
    }
    printf("Total count: %d\n", count);
}

int remove_list(struct board *board, struct list *list) {
    struct list *previous_list = board->my_lists;
    struct task *task, *temp;
    if(list == NULL)
        return errno_list_not_selected;
    if(list == board->my_lists) {
        board->my_lists = board->my_lists->next; // update my_lists pointer because the first item is being freed
    } else {
        while(previous_list != NULL && previous_list != list && previous_list->next != list)
            previous_list = previous_list->next;
        if(previous_list == NULL)
            return errno_item_not_found;
        previous_list->next = list->next; // Remove list from Link list

    }
    // Remove list tasks:
    task = list->my_tasks;
    while(task != NULL) {
        temp = task; // Removing task removes task->next pointer too!
        task = task->next;
        free(temp); // Remove its task one by one
    }
    free(list);
    return 1;
}

int check_task_input(int priority, int year, int month, int day) {
    if(priority < 0 || priority > 2)
        return errno_task_prioirty_invalid;
    if(year <= 0 || month <= 0 || day <= 0 || month > 12 || day > 31)
        return errno_task_data_invalid;
    return 0;
}

struct task* get_new_task(char name[], int priority, int year, int month, int day) {
    struct task* new_task = (struct task*) calloc(1, sizeof(struct user));
    sprintf(new_task->name, "%s", name);
    new_task->priority = priority;
    new_task->date_year = year;
    new_task->date_month = month;
    new_task->date_day = day;

    new_task->next = NULL;
    return new_task;
}

struct task *init_tasks(char first_task_name[], int first_task_priority, int year, int month, int day) {
    // this is for init user->mytasks
    struct task* my_tasks = get_new_task(first_task_name, first_task_priority, year, month, day);
    return my_tasks;
}

int add_task(struct task *my_tasks, char name[], int priority, int year, int month, int day) {
    // Because if the my_tasks is empty, it needs to update its address
    struct task* task = my_tasks;
    struct task* new_task = get_new_task(name, priority, year, month, day);
    int task_number = 1;
    while(task->next != NULL) {
        task = task->next;
        task_number++;
    }
    task->next = new_task;
    task->next->next = NULL;

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
    while(task != NULL) {
        count++;
        show_single_task(task, count);
        task = task->next;
    }
    printf("Total count: %d\n", count);
}

void show_single_task(struct task *task, int number) {
    char *priority;
    if(task->priority == 2)
        priority = "High";
    else if(task->priority == 1)
        priority = "Medium";
    else
        priority = "Low";
    if(number > 0) // when show_tasks call this function, it provides an task number
        printf("%d: ", number);
    printf("%s, %s, %d/%d/%d\n", task->name, priority, task->date_year, task->date_month, task->date_day);
}

int remove_task(struct list *list, struct task *task) {
    struct task *previous_task = list->my_tasks;
    if(task == NULL)
        return errno_task_not_selected;
    if(task == list->my_tasks) {
        list->my_tasks = list->my_tasks->next;
        free(task);
        return 1;
    }
    while(previous_task != NULL && previous_task->next != task)
        previous_task = previous_task->next;
    if(previous_task == NULL)
        return errno_item_not_found;
    previous_task->next = task->next; // Remove task from Link task
    free(task);
    return 1;
}

int move_task(struct task *task, struct list *source_list, struct list *target_list) {
    struct task *previous_task = source_list->my_tasks;
    struct task *tl;
    if(task == NULL)
        return errno_task_not_selected;
    if(source_list->my_tasks == task)
        source_list->my_tasks = source_list->my_tasks->next;
    else {
        while(previous_task != NULL && source_list->my_tasks != task &&  previous_task->next != task)
            previous_task = previous_task->next;
        if(previous_task == NULL)
            return errno_item_not_found;
            
        previous_task->next = task->next; // Remove task from Link task
    }
    // Add task to target_list
    tl = target_list->my_tasks;
    task->next = NULL;
    if(tl != NULL) {
        while(tl->next != NULL) {
            tl = tl->next;
        }
        tl->next = task; // target list tasks linked to task
    } else {
        target_list->my_tasks = task;
    }
    return 1;
}

int date_to_days(int year, int month, int day) {
    int days = year * 365 + day;
    
    for(int m = 1; m < month; m++) {
        if(m == 2)
            days += 29;
        else if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
            days += 31;
        else days += 30;
    }

    return days;
}

void sort_list(struct list *list, int sort_by) {
    if(list != NULL)
    {
        struct task* task = list->my_tasks;
        for(int i = 0; i < list->tasks_count && task != NULL; i++) {
            struct task *next_task = task->next;
            for(int j = i + 1; next_task != NULL &&  j < list->tasks_count && task != NULL; j++) {
                int swap = 0;
                if(sort_by == 0) {
                    swap = next_task->priority > task->priority;
                } else if(sort_by == 1) {
                    if(next_task->date_year < task->date_year) {
                        swap = 1;
                    } else if(next_task->date_year == task->date_year) {
                        if(next_task->date_month < task->date_month)
                            swap = 1;
                        else if(next_task->date_month == task->date_month && next_task->date_day < task->date_day)
                            swap = 1;
                    }
                }
                if(swap == 1) { //Swap to sort
                    int t_year, t_month, t_day, t_priority;
                    char t_name[500];
                    sprintf(t_name, "%s", task->name);
                    t_year = task->date_year;
                    t_month = task->date_month;
                    t_day = task->date_day;
                    t_priority = task->priority;
                    
                    sprintf(task->name, "%s", next_task->name);
                    task->date_year = next_task->date_year;
                    task->date_month = next_task->date_month;
                    task->date_day = next_task->date_day;
                    task->priority = next_task->priority;

                    sprintf(next_task->name, "%s", t_name);
                    next_task->date_year = t_year;
                    next_task->date_month = t_month;
                    next_task->date_day = t_day;
                    next_task->priority = t_priority;
                }
            }
        }
    }

}