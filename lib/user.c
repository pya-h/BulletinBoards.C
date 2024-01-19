#include "user.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void resetUser(User *user)
{
    user->id = 0;
    user->loggedIn = 0;
    user->currentBoardId = user->currentListId = 0;
}

User *newUser()
{
    User *user = (User *)calloc(1, sizeof(User));
    resetUser(user);
    return user;
}

User *registerUser(char username[], char password[])
{

    User *user = newUser();
    // create the user
    time_t now = time(NULL);

    if (now != -1)
    {
        // time returned successfully
        user->id = (long)now;
        strcpy(user->name, username);

        // Save credentials into a file
        SET_USER_DATA_FILE(user->location, FOLDER_USERS, user->name);

        FILE *userFile = fopen(user->location, "w");
        if (userFile)
        {

            fprintf(userFile, "%s\n%d\n", encodeString(password) , user->id); // save password and id as encoded
            // password will not be stored in user and will only be used for login purposes;
            user->loggedIn = 1;
        }
        else
        {
            // TODO: maybe folder doesnt exist!
            fprintf(stderr, "Cannot save user credentials!");
                resetUser(user);// for making sure user is not logged in without data stored in Data
        }
        // close the file
        fclose(userFile);
    }
    else
    {
        // ERROR:
        resetUser(user);           // identifier of failure
        fprintf(stderr, "Cannot assign an id to you!");
    }

    return user;
}

User *loginUser(char username[], char password[])
{
    User *user = newUser(); // user is loggod out by default
    // create the user
    strcpy(user->name, username);

    SET_USER_DATA_FILE(user->location, FOLDER_USERS, user->name);
    // now check if there is registered user with this name
    FILE *userFile = fopen(user->location, "r");
    if (userFile)
    {
        // if the user exists
        char *savedPassword = (char *)calloc(MAX_ENCODED_STRING_LENGTH(MAX_PASSWORD_LENGTH), sizeof(char));
        fscanf(userFile, "%s %d", savedPassword, &user->id);
        if (!strcmp(encodeString(password), savedPassword))
        {
            // if passwords are equal then login
            user->loggedIn = 1;
        }
    }
    else
    {
        resetUser(user); // identifier of error is an empty user
    }
    // else user will remain logged out ...
    // close the file
    fclose(userFile);
    return user;
}

char *validateRegisterationInput(char inputUser[], char inputPassword[])
{
    // TODO: checks the input, returns the error string or NULL(if ok!)
    return NULL;
}