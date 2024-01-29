#ifndef TASK_H
#define TASK_H
    #include <time.h>
    #include "base.h"
    #include "task-list.h"
    #include <stdio.h>

    #define MAX_TASK_TITLE_LENGTH 256
    #define FOLDER_TASKS "Tasks"
    #define MAX_DATE_LENGTH 16

    typedef struct tm Date;

    typedef enum Priority {
        LOW = 'L',
        MEDIUM = 'M',
        HIGH = 'H'
    } Priority;

    typedef struct
    {
        Long id;

        Long ownerId; // is it used? because .taskList->board->ownerId is the same
        
        char title[MAX_TASK_TITLE_LENGTH], error[MAX_RESPONSE_LENGTH];
        Priority priority;
        Date deadline;
        TaskList *taskList; // this one also contains Board
    } Task;

    void Task_reset(Task *);
    Task *newTask();
    Task *createTask(TaskList *, char [], Priority, string);
    List *getTasks(TaskList *);
    short Tasks_save(List *, Long);
    short Tasks_delete(List *, Task *);

    string Task_getError(Task *);
    void Task_failure(Task *, string);
    void Task_print(Task *);
    Long Task_index(Task *, List *);
    string Priority_toString(Priority);
#endif