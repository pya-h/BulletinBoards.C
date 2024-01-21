#ifndef TASK_LIST_H
#define TASK_LIST_H
    #include "base.h"
    #include "link-list.h"
    #include "board.h"
    #define FOLDER_LISTS "Lists"
    #define MAX_LIST_FILE_ROW_LENGTH MAX_TITLE_LENGTH + 20 // 20 as the length of id and some extra characters such as comma(delimiter) or double quatation

    typedef struct TaskList
    {
        long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char title[MAX_TITLE_LENGTH];
        Board *board;
        // place where this board data is stored
        char location[MAX_FILENAME_LENGTH];

        char error[MAX_RESPONSE_LENGTH];
    } TaskList;

    // prototypes:
    TaskList *newTaskList();
    TaskList *createTaskList(Board *board, char title[]);
    List *getTaskLists(Board *board);
    short TaskLists_save(List *TaskLists);
    string TaskList_getError(TaskList *);
    void TaskList_reset(TaskList *TaskList);
    void TaskList_failure(TaskList *board, string msg);
#endif // TASK_LIST_H