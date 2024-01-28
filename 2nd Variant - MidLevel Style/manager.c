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
    struct user *user = (struct user *) malloc(sizeof(struct user));
    sprintf(index_filename, "user-%d\\index.csv", folder_number);
    f_user_index_reader = open_file_for_read(index_filename);

    if(f_user_index_reader) {
        fscanf(f_user_index_reader, "%49[^,],%s\n", user->username, user->password);
        fscanf(f_user_index_reader, "%49[^,]\n", user->username);
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
    char* filename = (char*)malloc(sizeof(char) * 30);
    sprintf(filename, "user-%d\\%s.csv", user_folder_number, file_type);
    return filename;
}

int save(struct user *user) {
    /* User contains its all data by pointers
     this function will save all. */
    char* index_filename = get_filename(user->folder_number, "index");
    char* list_filename = get_filename(user->folder_number, "list");
    char* task_filename = get_filename(user->folder_number, "task");
    FILE* f_user_index_writer = get_user_file(index_filename, user->username, user->password);
    FILE* f_list_writer;
    FILE* f_task_writer;
    struct board *board = user->my_boards;
    int board_number = 1, list_number = 1;
    if(!f_user_index_writer)
        return errno_user_files_not_available;
    
    fprintf(f_user_index_writer, "board_number,board_name\n"); // Index file containes board data too
    f_list_writer = fopen(list_filename, "w"); //open list file for write all board lists
    if(!f_list_writer) {
        fclose(f_list_writer);
        fclose(f_user_index_writer);
        return errno_user_files_not_available;
    }

    fprintf(f_list_writer, "board_number,list_number,list_name\n");
    f_task_writer = fopen(task_filename, "w"); //open task file for write task board lists
    if(!f_task_writer) {
        fclose(f_task_writer);
        fclose(f_list_writer);
        fclose(f_user_index_writer);
        return errno_user_files_not_available;
    }
    fprintf(f_task_writer, "board_number,list_number,task_name,task_priority,task_year,task_month,task_day\n");

    while(board != NULL){
        struct list *list = board->my_lists;
        fprintf(f_user_index_writer, "%d,%s\n", board_number, board->name); // Index file containes board data too
        list_number = 1;
        
        while(list != NULL) { // write lists
            struct task *task = list->my_tasks;
            fprintf(f_list_writer, "%d,%d,%s\n", board_number, list_number, list->name); // Index file containes board data too
            while(task != NULL) { // write lists
                fprintf(f_task_writer, "%d,%d,%s,%d,%d,%d,%d\n", board_number, list_number, task->name, task->priority, task->date.year, task->date.month, task->date.day); // Index file containes board data too
                task = task->next;
            }
            list_number++;
            list = list->next;
        }

        board_number++;
        board = board->next;
    }
    fclose(f_task_writer);
    fclose(f_list_writer);
    fclose(f_user_index_writer);
    return 1;
}