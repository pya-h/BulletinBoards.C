#include "manager.h"
// First line of csv files must be ignored
FILE *open_file_for_read(char filename[]) {
    FILE *file = fopen(filename, "r");
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
    FILE *user_list = open_file_for_read("users.csv");
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
    FILE *user_list = open_file_for_read("users.csv");
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

int register_user(char username[], char password[]) {
    int folder_number = get_next_folder_number();
    char folder_name[20], index_filename[30];
    FILE *f_user_list_writer;
    if(folder_number != errno_user_list_not_available) {
        f_user_list_writer = fopen("users.csv", "a");
        fprintf(f_user_list_writer, "%d,%s\n", folder_number, username); // Add user folder folder number to user list. because this file helps in finding user
        fclose(f_user_list_writer);
        
        sprintf(folder_name, "user-%d", folder_number);
        sprintf(index_filename, "%s\\index.csv", folder_name);
        if(CreateDirectory(folder_name, NULL)) {// Create the user data folder, This file contains user fields, boards and lists and tasks;
            FILE *f_user_index_writer = fopen(index_filename, "w");
            fprintf(f_user_index_writer, "username,password\n");
            fprintf(f_user_index_writer, "%s,%s\n", username, password);
            fclose(f_user_index_writer);
            return folder_number;
        } 
        return errno_folder_creation_failed; // Error Number: 
    }
    return errno_user_list_not_available;
}

struct user *login_user(int folder_number, char username[], char password[]) {
    char index_filename[30];
    FILE *f_user_index_reader;
    struct user *user = NULL;
    sprintf(index_filename, "user-%d\\index.csv", folder_number);
    printf("Index file: %s\n", index_filename);
    f_user_index_reader = open_file_for_read(index_filename);

    if(f_user_index_reader) {
        struct user target_user;
        printf("Index file: %s Opened\n", index_filename);
        fscanf(f_user_index_reader, "%49[^,] %s\n", target_user.username, target_user.password);
        printf("user in file: %s,%s\n", target_user.username, target_user.password);
        fclose(f_user_index_reader);
        if(!strcmp(target_user.username, username) && !strcmp(target_user.password, password)) {
            user = &target_user;
            return user;
        }
    }
    return NULL;
}