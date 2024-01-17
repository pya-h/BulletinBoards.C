#include "user.h"
#include <string.h>
#include <stdio.h>
#include <time.h>


User *registerUser(char username[], char password[]) {
    User *user;
    // create the user
    time_t now = time(NULL);
    if(now != -1) 
        // time returned successfully
        user->id = (long)now;
    else {
        // ERROR:
        fprintf(stderr, "Cannot assign an id to you!");
    }

    strcpy(user->name, username);
    strcpy(user->password, password);
    // Save credentials into a file
    SET_DATA_FILE(user->location, FOLDER_USERS, user->id);
    FILE *userFile = fopen(user->location, "w");
    fprintf(userFile, "%s\n%s", user->name, user->password);
    // close the file
    fclose(userFile);

    return user;
}
