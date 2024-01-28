#include "CSVFILE.h"
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
    FILE* f_user_index_writer =  get_user_file(index_filename, user->username, user->password);
    FILE* f_list_writer;
    FILE* f_task_writer;
    struct board *board = user->my_boards;
    struct list *list;
    struct task *task;
    int tasks_saved = 0;
    int board_number = 1;
    int list_number = 1;
    int lists_saved = 0;
    int task_number = 1;
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
    fclose(f_list_writer);
    fclose(f_task_writer);

    while(board != NULL && board_number <= user->boards_count){
        // printf("Board: %s\n", board->name);
        fprintf(f_user_index_writer, "%d,%s\n", board_number, board->name); // Index file containes board data too
        list = board->my_lists;
        list_number = 1;
        f_list_writer = fopen(list_filename, "a");
        list_number = 1;
        while(list != NULL && list_number <= board->lists_count) { // write lists
            // printf("\tList: %s\n", list->name);
            fprintf(f_list_writer, "%d,%d,%s\n", board_number, list_number, list->name); // Index file containes board data too
            task = list->my_tasks;
            task_number = 1;
            while(task != NULL && task_number <= list->tasks_count) { // Write lists
                struct task tx = *task;
                f_task_writer = fopen(task_filename, "a"); 
                // printf("\t\t\tName: %s  \n", tx.name);
                // printf("\t\t\tPri: %d  \n", tx.priority);
                // printf("\t\t\tyear: %d  \n", tx.date_year);
                // printf("\t\t\tmonth: %d  \n", tx.date_month);
                // printf("\t\t\tday: %d  \n", tx.date_day);

                fprintf(f_task_writer, "%d,%d,%d,", board_number, list_number, tx.priority);
                fprintf(f_task_writer, "%d,%d,%d,", tx.date_year, tx.date_month, tx.date_day);
                fprintf(f_task_writer, "%s\n", tx.name);

                tasks_saved++;
                task_number++;
                task = task->next;
                // printf("done\n");
                fclose(f_task_writer);
            }
            task = NULL;
            list_number++;
            lists_saved++;
            // printf("\tList: %s done\n", list->name);
            list = list->next;
        }
        fclose(f_list_writer);
        board_number++;
        // printf("Board: %s done\n", board->name);
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
    FILE* f_list_reader;
    FILE* f_task_reader;
    struct board *board = user->my_boards;
    struct list *list = NULL;
    struct task *task = NULL;
    int tasks_loaded = 0;
    int board_number = 1;
    int list_number = 1;
    int lists_loaded = 0;
    int boards_loaded = 0;

    char temp_line[100];
    int next_board_number;
    int next_list_number;
    char next_name[500];
    if(!f_user_index_reader)
        return errno_user_files_not_available;    
    for(int i = 0; i < 2 && !feof(f_user_index_reader); i++) {
        fscanf(f_user_index_reader, "%99[^\n]\n", temp_line); // ignore line
    } // ignore username and password line and board header line

    
    while(!feof(f_user_index_reader)) {

        fscanf(f_user_index_reader, "%d,%499[^\n]\n", &board_number, next_name); // Index file containes board data too
        if(user->my_boards != NULL) {
            board->next = (struct board *) calloc(1, sizeof(struct board));
            board = board->next;
            sprintf(board->name, "%s", next_name);
        } else {
            user->my_boards = init_boards(next_name);
            board = user->my_boards;
        }
        boards_loaded++;
        user->boards_count++;
    }
    fclose(f_user_index_reader);
    if(board == NULL)
        return 0;
    board->next = NULL;

    f_list_reader = open_file_for_read(list_filename);
    if(!f_list_reader) {
        fclose(f_list_reader);
        return errno_user_files_not_available;
    }
    // now load lists of each board
    board = user->my_boards;
    list = board->my_lists = NULL;
    board_number = next_board_number = 1;
    while(!feof(f_list_reader)) {
        fscanf(f_list_reader, "%d,%d,%499[^\n]\n", &next_board_number, &list_number, next_name);
        if(next_board_number != board_number) {
            board = board->next;
            board_number = next_board_number;
            list_number = 1;
        }
        if(board->my_lists != NULL) {
            list->next = (struct list *) calloc(1, sizeof(struct list));
            list = list->next;
            sprintf(list->name, "%s", next_name);
            board->lists_count++;
        } else {
            board->my_lists = init_lists(next_name);
            board->lists_count = 1;
            list = board->my_lists;
        }
        lists_loaded ++;
    }
    fclose(f_list_reader);
    
    if(list == NULL)
        return 0;
    list->next = NULL;
    // now load tasks of each list
    board = user->my_boards;
    if(board != NULL) {
        list = board->my_lists;
        if(list != NULL) {
            task = list->my_tasks = NULL;
            board_number = 1;
            list_number = 1;
            next_board_number = next_list_number = 1;
            f_task_reader = open_file_for_read(task_filename);

            if(!f_task_reader) {  
                fclose(f_task_reader);
                return errno_user_files_not_available;
            }
            while(!feof(f_task_reader)) {
                int priority;
                int year = 0, month = 0, day = 0;
                fscanf(f_task_reader, "%d,%d,%d,%d,%d,%d,%499[^\n]\n", &next_board_number, &next_list_number, &priority, &year, &month, &day, next_name); 
                if(next_list_number != list_number) {
                    list = list->next;
                    list_number = next_list_number;
                }

                if(next_board_number != board_number) {
                    board = board->next;
                    board_number = next_board_number;
                    list_number = next_list_number = 1;
                    list = board->my_lists;
                }

                if(list->my_tasks != NULL) {
                    task->next = get_new_task(next_name, priority, year, month, day);
                    task = task->next;
                    list->tasks_count++;
                } else {
                    list->my_tasks = init_tasks(next_name, priority, year, month, day);
                    task = list->my_tasks;
                    list->tasks_count = 1;
                }
                list->tasks_count++;
                tasks_loaded++;
            }
            if(task != NULL)
                task->next = NULL;
            fclose(f_task_reader);
        }
    }
    printf("Total Result:\n");
    printf("%d boards loaded.\n", boards_loaded);
    printf("%d lists loaded.\n", lists_loaded);
    printf("%d tasks loaded.\n", tasks_loaded);

    return 1;
}

void show_reminder(struct user *user) {
    // find 3 next days tasks
    struct board *board = user->my_boards;
    int diff = 0;
    printf("Reminders:\n");
    for(int i = 0, count = 0; i < user->boards_count && board != NULL && i <= user->boards_count; i++) {
        struct list *list = board->my_lists;
        for(int j = 0; list != NULL && j <= board->lists_count; j++) { // write lists
            struct task *task = list->my_tasks;
            for(int k = 0; k < list->tasks_count && task != NULL; k++) { // Write lists
                diff = date_to_days(task->date_year, task->date_month, task->date_day) -  date_to_days(user->current_year, user->current_month, user->current_day);
                if(diff >= 0 && diff <= 3) {
                    count++;
                    printf(" %d: Board: %s,   List: %s\n  ", count, board->name, list->name);
                    show_single_task(task, 0);
                }
                task = task->next;
            }
            task = NULL;

            list = list->next;
        }
        board = board->next;
    }
}
