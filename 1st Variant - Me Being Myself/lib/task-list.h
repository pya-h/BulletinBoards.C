#ifndef TASK_LIST_H
#define TASK_LIST_H
    #include "base.h"
    #include "link-list.h"
    #include "board.h"
    #define FOLDER_LISTS "Lists"
    #define MAX_LIST_FILE_ROW_LENGTH MAX_TITLE_LENGTH + MAX_LONG_NUMBER_LENGTH*2 + 10 // 50 as the length of id, ownerId and some extra characters such as comma(delimiter) or double quatation

    typedef struct TaskList
    {
        Long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char title[MAX_TITLE_LENGTH], 
            error[MAX_RESPONSE_LENGTH];
        Board *board; // the board that contains the list
    } TaskList;

    // prototypes:
    TaskList *newTaskList();
    TaskList *createTaskList(Board *, char []);
    List *getTaskLists(Board *);
    short TaskLists_save(List *, Long);
    string TaskList_getError(TaskList *);
    void TaskList_reset(TaskList *);
    void TaskList_failure(TaskList *, string);
    void TaskList_print(TaskList *);
#endif // TASK_LIST_H