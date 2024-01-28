#include"TASK.H"

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