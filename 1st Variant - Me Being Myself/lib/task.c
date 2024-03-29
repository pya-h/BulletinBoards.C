#include "task.h"
#include <stdlib.h>
#include <string.h>

void Task_reset(Task *task)
{
    task->title[0] = task->error[0] = '\0'; // a no location, becqause the task is not saved.
    task->id = 0;
    task->taskList = NULL; // identifier of failure
}

Task *newTask()
{
    Task *task = (Task *)calloc(1, sizeof(Task));
    Task_reset(task);
    return task;
}

Task *createTask(TaskList *containerTaskList, char title[], Priority priority, string strDeadline)
{
    Task *task = newTask();
    time_t now = time(NULL);
    task->taskList = containerTaskList;

    if (strlen(title) > MAX_TASK_TITLE_LENGTH)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Title of a task can not exceed %d characters!", MAX_RESPONSE_LENGTH);
        Task_failure(task, err);
        return task;
    }
    strncpy(task->title, title, MAX_TASK_TITLE_LENGTH - 1);
    task->priority = priority;
    if (sscanf(strDeadline, "%d-%d-%d", &(task->deadline.tm_year), &(task->deadline.tm_mon), &(task->deadline.tm_mday)) != 3)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Deadline is in wrong format! Enter it in this format: Year-Month-Day", MAX_RESPONSE_LENGTH);
        Task_failure(task, err);
        return task;
    }

    if (now != -1)
    {
        char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};

        task->id = (Long)now;
        // the tasks created by a user will be stored in a file in Tasks folder, named by the id of the container task list
        SET_DATA_FILE(fileLocation, FOLDER_TASKS, containerTaskList->id); // tasks are saved in Tasks folder under the filename of taskList->id
        if (!fileExists(fileLocation))
        {
            // create tasks file and add the header row
            FILE *taskFile = fopen(fileLocation, "w");
            fprintf(taskFile, "Id%sTask Title%sPriority%sDeadline%sBoard Id%sOwner Id\n",
                    COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER);
            fclose(taskFile);
        }

        // now add new items
        FILE *taskFile = fopen(fileLocation, "a");
        if (taskFile)
        {
            fprintf(taskFile, "%llu%s\"%s\"%s%c%s%d-%d-%d%s%llu%s%llu\n",
                    task->id, COLUMN_DELIMITER, task->title, COLUMN_DELIMITER, task->priority, COLUMN_DELIMITER,    // task data
                    task->deadline.tm_year, task->deadline.tm_mon, task->deadline.tm_mday,                          // task deadline data
                    COLUMN_DELIMITER, task->taskList->board->id, COLUMN_DELIMITER, task->taskList->board->ownerId); // append new task to file
            // owner id and board id are saved just to check data is not modified while reading
            // Its for reassuring purpose
            fclose(taskFile);
        }
        else
        {
            Task_failure(task, "Cannot save Task data!");
        }
    }
    else
    {
        Task_failure(task, "Cannot assign id to this new Task!");
    }
    _fcloseall(); // Just to make sure
    return task;
}

List *getTasks(TaskList *containerTaskList)
{
    // read all the task list from the file
    List *tasks = newList();
    char tasksFilename[MAX_FILENAME_LENGTH] = {'\0'};
    SET_DATA_FILE(tasksFilename, FOLDER_TASKS, containerTaskList->id); // now tasksFile contains the address of the TaskList file that contains desired user TaskList list ata.
    FILE *tasksFile = fopen(tasksFilename, "r");
    if (!tasksFile)
    {
        _fcloseall();
        return tasks; // user has not create any TaskList; but a empty list will be sent back to caller, so that the user can add items in the future
    }
    Task *nextTask = newTask(); // this is used to read each TaskList data
    string row = String(MAX_LIST_FILE_ROW_LENGTH);
    if (!feof(tasksFile) && fgets(row, MAX_LIST_FILE_ROW_LENGTH, tasksFile) != NULL)
    { // the first row are headers(titles); also its a good practice to check that as an insurance that file data is stored correctly.
        for (; !feof(tasksFile) && fgets(row, MAX_LIST_FILE_ROW_LENGTH, tasksFile) != NULL; nextTask = newTask())
        {
            removeNextlineCharacter(row);
            // the second call to fgets will read the credential line
            const string id = (string)strtok(row, COLUMN_DELIMITER); // split the text by COLUMN_DELIMITER[,] character
            string title = (string)strtok(NULL, COLUMN_DELIMITER);   // get the next column
            const string priority = (string)strtok(NULL, COLUMN_DELIMITER);
            const string strDeadline = (string)strtok(NULL, COLUMN_DELIMITER);
            const string boardId = (string)strtok(NULL, COLUMN_DELIMITER); // the boardId is also in containerTaskList->Board->id
            const string ownerId = (string)strtok(NULL, COLUMN_DELIMITER); // the ownerId is also in containerTaskList->Board->ownerId
            Date deadline;
            string conversionError;
            // this one is used just to check values are correct
            title = trimColumnValue(title);
            if (!id || !title || !priority || !strDeadline ||
                sscanf(strDeadline, "%d-%d-%d", &deadline.tm_year, &deadline.tm_mon, &deadline.tm_mday) != 3 || !ownerId || !boardId ||
                strtoull(boardId, &conversionError, 10) != containerTaskList->board->id || strtoull(ownerId, &conversionError, 10) != containerTaskList->board->ownerId)
            {                         // checks all possible errors, and also converts deadline from string to Date(struct tm)
                if (!feof(tasksFile)) // if file is not ended and this condition happended, then the data of this Task is corrupted
                    Task_failure(nextTask, "It seems the data related to this task is corrupted or modified!");
                continue;
            }

            // each task list occupies to lines
            // first line is its id and the second is the title
            strncpy(nextTask->title, title, MAX_TITLE_LENGTH);
            nextTask->id = strtoull(id, &conversionError, 10); // convert read id to Long
            nextTask->priority = priority[0];
            nextTask->deadline = deadline;
            nextTask->taskList = containerTaskList;
            if (!nextTask->id || *conversionError)
            {
                Task_failure(nextTask, "Could not read the id property of this Task successfully!");
                continue; // set the error message of this one and continue reading the next one (cause the file is not ended yet.)
            }

            // nextTask on each step of the loop, will be used to read list data;
            // then the memory it points to will be added as the tasks list item,
            // then nextTask will occupy a new place on memory
            List_add(tasks, nextTask);
        }
    }
    free(nextTask); // at last step of for, a new empty Task is allocated that is not needed

    fclose(tasksFile);
    return tasks;
}

short Tasks_save(List *tasks, Long containerTaskListId)
{
    // save the list of tasks
    // this is used when a task is modified; we clear the file and write all tasks again
    // this way we dont need to search the file and modify the middle of the file, which is hard and slow in performance
    // this is used when a task list is modified or deleted
    // then the app should remove tasklist file data and replace its data with the updated data
    // that is stored in taskLists list
    char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};
    FILE *taskFile; // file related to the selected list of a selected board
    Task *task;
    SET_DATA_FILE(fileLocation, FOLDER_TASKS, containerTaskListId);
    // create the file and add the header row
    taskFile = fopen(fileLocation, "w");
    if (!taskFile)
    {
        List_failure(tasks, "Cannot save Tasks data!");
        return 0; // error happend
    }
    fprintf(taskFile, "Id%sTask Title%sPriority%sDeadline%sBoard Id%sOwner Id\n",
            COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER);

    for (Long i = 0; i < tasks->length; i++)
    {
        task = (Task *)List_at(tasks, i);
        fprintf(taskFile, "%llu%s\"%s\"%s%c%s%d-%d-%d%s%llu%s%llu\n",
                task->id, COLUMN_DELIMITER, task->title, COLUMN_DELIMITER, task->priority, COLUMN_DELIMITER,    // task data
                task->deadline.tm_year, task->deadline.tm_mon, task->deadline.tm_mday,                          // task deadline data
                COLUMN_DELIMITER, task->taskList->board->id, COLUMN_DELIMITER, task->taskList->board->ownerId); // append new task to file
    }
    fclose(taskFile);
    return 1;
}

string Task_getError(Task *task)
{
    if (task && task->error[0])
        // if error string is not empty, return it directly
        return task->error;
    if (!task)
        return "An UnknownError detected. Trying to restart the app may help resolve or identify the issue ...";

    if (!task->id || !task->taskList || !task->taskList->id || !task->taskList->board || !task->taskList->board->id)
        return "An UnknownError detected and it appears to be related to tasks database.\n\tTrying to restart the app may help resolve or identify the issue ...\n";
    return NULL;
}

void Task_failure(Task *task, string msg)
{
    Task_reset(task);
    sprintf(task->error, "Operation Failed:\t%s\n", msg);
}

string Priority_toString(Priority priority)
{

    return (string)(priority == HIGH     ? "High"
                    : priority == MEDIUM ? "Medium"
                    : priority == LOW    ? "Low"
                                         : "Unknown!");
}

short Task_displace(Task *task, TaskList *distionationList, List *tasksOfSourceList)
{
    // List *sourceTasksOfThisTaskList =
    ListItem *taskItem = List_unlinkItem(tasksOfSourceList, task); // unlink &  extract the task item to link it to another item list
    task->taskList = distionationList; // declare destinationTaskList as the owner of this task
    List *tasksOfDestinationList = getTasks(distionationList);
    // List_add(tasksOfDestinationList, task);
    if (tasksOfDestinationList->length) { // if note empty
        tasksOfDestinationList->last->next = taskItem;
        taskItem->prev = tasksOfDestinationList->last;
        tasksOfDestinationList->last = taskItem;
    }
    else
    {
        tasksOfDestinationList->first = tasksOfDestinationList->last = taskItem;
        tasksOfDestinationList->first->prev = NULL;
    }
    tasksOfDestinationList->length++;
    tasksOfDestinationList->last->next = NULL;
    //TaskLists_save(tasksOfSourceList) => saved in main:
    Tasks_save(tasksOfDestinationList, distionationList->id);

    // TODO: Free memory
}

void Task_print(Task *task)
{
    printf("Your selected Task:\n\n    Id%9s\t\tPriority\t\tDeadline%2s\t\tTitle\n", " ", " ");
    PRINT_DASH_ROW();
    printf("%15llu\t\t%8s\t\t%04d-%02d-%02d\t\t%s\n", task->id, Priority_toString(task->priority),
           task->deadline.tm_year, task->deadline.tm_mon, task->deadline.tm_mday, task->title);
}