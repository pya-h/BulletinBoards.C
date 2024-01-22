#ifndef TASK_LIST_H
#define TASK_LIST_H
    #include "base.h"
    #include "link-list.h"
    #include "board.h"
    #define FOLDER_LISTS "Lists"
    #define MAX_LIST_FILE_ROW_LENGTH MAX_TITLE_LENGTH + 30 // 30 as the length of id, ownerId and some extra characters such as comma(delimiter) or double quatation

    typedef struct TaskList
    {
        long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char title[MAX_TITLE_LENGTH];
        Board *board; // the board that contains the list

        char error[MAX_RESPONSE_LENGTH];
    } TaskList;

    // prototypes:
    TaskList *newTaskList();
    TaskList *createTaskList(Board *containerBoard, char title[]);
    List *getTaskLists(Board *containerBoard);
    short TaskLists_save(List *TaskLists);
    string TaskList_getError(TaskList *);
    void TaskList_reset(TaskList *TaskList);
    void TaskList_failure(TaskList *taskList, string msg);
    void TaskList_print(TaskList *taskList);
#endif // TASK_LIST_H