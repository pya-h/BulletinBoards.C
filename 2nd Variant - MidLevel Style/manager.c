#include "manager.h"

/* Find the folder number of user
In that folder user data is saved */
int get_user_folder(char p_username[]) { 
    FILE *user_list = fopen("users.csv", "r");
    char username[50];
    int folder_number = EOF, temp;
    if(!user_list)
        return EOF;
    fscanf(user_list, "%s,%s\n", username, username); // skip first line
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
    FILE *user_list = fopen("users.csv", "r");
    int users_count = 0;
    if(!user_list) { // File not created yet
        _fcloseall(); // Close file as reader. Open for writing:
        user_list = fopen("users.csv", "w");
        if(!user_list) {
            _fcloseall();
            return errno_user_list_not_available;
        }
        fprintf(user_list, "folder,username\n"); // File first line = Column titles
        users_count++; // user folder number starts from 1 as user-1
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
    FILE *user_list;
    if(folder_number != errno_user_list_not_available) {
        user_list = fopen("users.csv", "a");
        fprintf(user_list, "%d,%s\n", folder_number, username); // Add user folder folder number to user list. because this file helps in finding user
        fclose(user_list);
        
        sprintf(folder_name, "user-%d", folder_number);
        sprintf(index_filename, "%s\\index.csv", folder_name);
        if(CreateDirectory(folder_name, NULL)) {// Create the user data folder, This file contains user fields, boards and lists and tasks;
            FILE *user_index = fopen(index_filename, "w");
            fprintf(user_index, "username,password,first_name,last_name,email,age\n");
            fprintf(user_index, "%s,%s\n", username, password);
            fclose(user_index);
            return folder_number;
        } 
        return errno_folder_creation_failed; // Error Number: 
    }
    return errno_user_list_not_available;
}