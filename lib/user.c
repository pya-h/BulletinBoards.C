#include "user.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

User *newUser() {
    User *user = (User *) calloc(sizeof(User), 1);
    user->loggedIn = 0;
    user->currentBoardId = user->currentListId = 0;

    return user;
}

User *registerUser(char username[], char password[]) {

    User *user = newUser();
    // create the user
    time_t now = time(NULL);

    if(now != -1) {
        // time returned successfully
        user->id = (long)now;
    }
    else {
        // ERROR:
        fprintf(stderr, "Cannot assign an id to you!");
    }
    strcpy(user->name, username);
    // Save credentials into a file

    SET_USER_DATA_FILE(user->location, FOLDER_USERS, user->name);
    // TODO: Create Data folders if not exist
    
    FILE *userFile = fopen(user->location, "w");
    if(userFile) {

        fprintf(userFile, "%s\n%ld", password, user->id); // username is the filename
        // password will not be stored in user and will only be used for login purposes;
        user->loggedIn = 1;
    } else {
        // TODO: maybe folder doesnt exist!
        fprintf(stderr, "Cannot save user credentials!");
        user->loggedIn = 0; // for making sure user is not logged in without data stored in Data
    }
    // close the file
    fclose(userFile);
    return user;
}


User *loginUser(char username[], char password[]) {
    User *user = newUser(); // user is loggod out by default
    // create the user
    strcpy(user->name, username);

    SET_USER_DATA_FILE(user->location, FOLDER_USERS, user->name);
    // now check if there is registered user with this name
    FILE *userFile = fopen(user->location, "r");
    if(userFile) {
        // if the user exists
        char actualPassword[MAX_PASSWORD_LENGTH];
        fscanf(userFile, "%s %ld", actualPassword, &user->id);
        if(!strcmp(password, actualPassword)) {
            // if passwords are equal then login
            user->loggedIn = 1;
        }
    }
    // else user will remain logged out ...
    // close the file
    fclose(userFile);
    return user;
}

char *validateRegisterationInput(char inputUser[], char inputPassword[]) {
    // TODO: checks the input, returns the error string or NULL(if ok!)
    return NULL;
}