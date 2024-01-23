#ifndef TASK_H
#define TASK_H
    #include <time.h>
    #include "base.h"
    #include "task-list.h"

    #define MAX_TASK_TITLE_LENGTH 256
    #define FOLDER_TASKS "Tasks"
    
    typedef struct tm Date;

    typedef enum Priority {
        LOW = 'L',
        MEDIUM = 'M',
        HIGH = 'H'
    } Priority;

    typedef struct Task {
        Long id;
        char title[MAX_TASK_TITLE_LENGTH];
        Priority priority;
        Date deadline;
        Long ownerId;
        TaskList *taskList; // this one also contains Board
        char error[MAX_RESPONSE_LENGTH];

    } Task;

    void Task_reset(Task *);
    Task *newTask();
    Task *createTask(TaskList *, char []);
    List *getTasks(TaskList *containerList);
    short Task_save(List *tasks);

    string Task_getError(Task *task);
    void Task_failure(Task *, string);
    void Task_print(Task *task);

#endif