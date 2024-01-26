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