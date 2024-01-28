#include "manager.h"
// First line of csv files must be ignored
FILE* open_file_for_read(char filename[]) {
    FILE* file = fopen(filename, "r");
    char temp_line[100];
    if(file != NULL && !feof(file)) {
        fscanf(file, "%99[^\n]\n", temp_line); // First line ignore. Use filename for temprory input of the line
        return file;
    }
    return NULL;
}
/* Find the folder number of user
In that folder user data is saved */
int get_user_folder(char p_username[]) { 
    FILE* user_list = open_file_for_read("users.csv");
    char username[50];
    int folder_number = EOF, temp;
    if(!user_list)
        return EOF;
    while(!feof(user_list)) {
        fscanf(user_list, "%d,%s\n", &temp, username);
        if(!strcmp(username, p_username)){ //user found
            folder_number = temp;
            break;
        }

    }
    fclose(user_list);
    return folder_number;
}

int get_next_folder_number() {
    FILE* user_list = open_file_for_read("users.csv");
    int users_count = 1; // user folder number starts from 1 as user-1
    // user_count shows the next free folder number
    if(!user_list) { // File not created yet
        _fcloseall(); // Close file as reader. Open for writing:
        user_list = fopen("users.csv", "w");
        if(!user_list) {
            _fcloseall();
            return errno_user_list_not_available;
        }
        fprintf(user_list, "folder,username\n"); // File first line = Column titles
    } else { // Then there are other users so count them
    // because folder names is as "user-[users_count]" 
        char line[100];
        while(!feof(user_list)) { // Read line by line, Each line is a user
            fscanf(user_list, "%99[^\n]\n", line); // skip first line
            users_count++;
        }
    }
    fclose(user_list);
    return users_count;
}

FILE* get_user_file(char *index_filename, char username[], char password[]) {
    FILE* f_user_index_writer = fopen(index_filename, "w");
    if(f_user_index_writer == NULL)
        return NULL;
    fprintf(f_user_index_writer, "username,password\n");
    fprintf(f_user_index_writer, "%s,%s\n", username, password);
    return f_user_index_writer;
}

int register_user(char username[], char password[]) {
    int folder_number = get_next_folder_number();
    char folder_name[20];

    FILE* f_user_list_writer;
    if(folder_number != errno_user_list_not_available) {
        sprintf(folder_name, "user-%d", folder_number);
        f_user_list_writer = fopen("users.csv", "a");
        fprintf(f_user_list_writer, "%d,%s\n", folder_number, username); // Add user folder folder number to user list. because this file helps in finding user
        fclose(f_user_list_writer);

        if(CreateDirectory(folder_name, NULL)) {// Create the user data folder, This file contains user fields, boards and lists and tasks;
            char* index_filename = get_filename(folder_number, "index");
            FILE* f_user_index_writer = get_user_file(index_filename, username, password);
            fclose(f_user_index_writer);
            return folder_number;
        } 
        return errno_folder_creation_failed; // Error Number: 
    }
    return errno_user_list_not_available;
}

struct user *login_user(int folder_number, char username[], char password[]) {
    char index_filename[30];
    FILE* f_user_index_reader;
    struct user *user = (struct user *) calloc(1, sizeof(struct user));
    sprintf(index_filename, "user-%d\\index.csv", folder_number);
    f_user_index_reader = open_file_for_read(index_filename);

    if(f_user_index_reader) {
        fscanf(f_user_index_reader, "%49[^,],%s\n", user->username, user->password);
        fclose(f_user_index_reader);
        if(!strcmp(user->username, username) && !strcmp(user->password, password)) {
            user->folder_number = folder_number;
            // temp init
            user->my_boards = NULL;
            return user;
        }
    }
    return NULL;
}

char* get_filename(int user_folder_number, char* file_type) {
    char* filename = (char*)calloc(30, sizeof(char));
    sprintf(filename, "user-%d\\%s.csv", user_folder_number, file_type);
    return filename;
}

int save(struct user *user) {
    /* User contains its all data by pointers
     this function will sav
     e all. */
    char* index_filename = get_filename(user->folder_number, "index");
    char* list_filename = get_filename(user->folder_number, "list");
    char* task_filename = get_filename(user->folder_number, "task");
    FILE* f_user_index_writer = get_user_
    FILE* f_list_writer;
    FILE* f_task_writer;
    struct board *board = user->my_boards;
    struct list *list;
    struct task *task;
    int tasks_saved = 0;
    int board_number = 1;
    int list_number = 1;
    int lists_saved = 0;

    if(!f_user_index_writer)
        return errno_user_files_not_available;
    
    fprintf(f_user_index_writer, "board_number,board_name\n"); // Index file containes board data too
    f_list_writer = fopen(list_filename, "w"); //open list file for write all board lists

    if(!f_list_writer) {
        fclose(f_list_writer);
        fclose(f_user_index_writer);
        return errno_user_files_not_available;
    }

    fprintf(f_list_writer, "board_number,list_number,name\n");
    f_task_writer = fopen(task_filename, "w"); //open task file for write task board lists
    if(!f_task_writer) {
        fclose(f_task_writer);
        fclose(f_list_writer);
        fclose(f_user_index_writer);
        return errno_user_files_not_available;
    }
    fprintf(f_task_writer, "board_number,list_number,priority,year,month,day,name\n");
    while(board != NULL){
        fprintf(f_user_index_writer, "%d,%s\n", board_number, board->name); // Index file containes board data too
        list = board->my_lists;
        list_number = 1;
        
        while(list != NULL) { // write lists
            // printf("List #%d savinng...\n", list_number);
            fprintf(f_list_writer, "%d,%d,%s\n", board_number, list_number, list->name); // Index file containes board data too
            task = list->my_tasks;
            while(task != NULL) { // Write lists
                fprintf(f_task_writer, "%d,%d,%d,%d,%d,%d,%s\n", board_number, list_number, task->priority, task->date->year, task->date->month, task->date->day, task->name);
                tasks_saved++;
                task = task->next;
            }
            task = NULL;
            list_number++;
            lists_saved++;
            list = list->next;
        }

        board_number++;
        board = board->next;
    }
    fclose(f_task_writer);
    fclose(f_list_writer);
    fclose(f_user_index_writer);
    printf("Total Result:\n");
    printf("%d boards saved.\n", board_number-1);
    printf("%d lists saved.\n", lists_saved);
    printf("%d tasks saved.\n", tasks_saved);

    return 1;
}

int load(struct user *user) {
    /* User contains its all data by pointers
     this function will sav
     e all. */
    char* index_filename = get_filename(user->folder_number, "index");
    char* list_filename = get_filename(user->folder_number, "list");
    char* task_filename = get_filename(user->folder_number, "task");
    FILE* f_user_index_reader = open_file_for_read(index_filename);
    FILE* f_list_reader = open_file_for_read(list_filename);
    FILE* f_task_reader = open_file_for_read(task_filename);
    struct board *board = user->my_boards;
    struct list *list = NULL;
    struct task *task = NULL;
    int tasks_loaded = 0;
    int board_number = 1;
    int list_number = 1;
    int lists_loaded = 0;

    if(!f_user_index_reader)
        return errno_user_files_not_available;    

    char temp_line[100];
    for(int i = 0; i < 2 && !feof(f_user_index_reader); i++) {
        fscanf(f_user_index_reader, "%99[^\n]\n", temp_line); // ignore line
    } // ignore username and password line and board header line

    f_list_reader = fopen(list_filename, "r"); //open list file for write all board lists

    if(!f_list_reader) {
        fclose(f_list_reader);
        fclose(f_user_index_reader);
        return errno_user_files_not_available;
    }

    f_task_reader = fopen(task_filename, "r"); //open task file for write task board lists
    if(!f_task_reader) {
        fclose(f_task_reader);
        fclose(f_list_reader);
        fclose(f_user_index_reader);
        return errno_user_files_not_available;
    }

    while(!feof(f_user_index_reader){
        fprintf(f_user_index_reader, "%d,%s\n", board_number, board->name); // Index file containes board data too
        list = board->my_lists;
        list_number = 1;
        
        while(!feof(f_list_reader)) { // write lists
            // printf("List #%d savinng...\n", list_number);
            fprintf(f_list_reader, "%d,%d,%s\n", board_number, list_number, list->name); // Index file containes board data too
            task = list->my_tasks;
            while(!feof(f_task_reader)) { // Write lists
                fprintf(f_task_reader, "%d,%d,%d,%d,%d,%d,%s\n", board_number, list_number, task->priority, task->date->year, task->date->month, task->date->day, task->name);
                tasks_loaded++;
                task = task->next;
            }
            task = NULL;
            list_number++;
            lists_loaded++;
            list = list->next;
        }

        board_number++;
        board = board->next;
    }
    fclose(f_task_reader);
    fclose(f_list_reader);
    fclose(f_user_index_reader);
    printf("Total Result:\n");
    printf("%d boards saved.\n", board_number-1);
    printf("%d lists saved.\n", lists_loaded);
    printf("%d tasks saved.\n", tasks_loaded);

    return 1;
}