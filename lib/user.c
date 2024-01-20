#include "user.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void resetUser(User *user)
{
    user->id = 0;
    user->loggedIn = 0;
    user->name[0] = user->error[0] = '\0';
    user->currentBoardId = user->currentListId = 0;
}

User *newUser()
{
    User *user = (User *)calloc(1, sizeof(User));
    resetUser(user);
    return user;
}

void throwUserError(User *user, string msg) 
{
    resetUser(user);
    sprintf(user->error, "Operation Failure:\n\t%s\n", msg);
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
        if(fileExists(user->location))
        {
            throwUserError(user, "The username is taken before!");
            return user;
        }
        FILE *userFile = fopen(user->location, "w");
        if (userFile)
        {

            fprintf(userFile, "User Id%sUser Encoded Credentials\n%ld%s\"%s\"\n", COLUMN_DELIMITER, user->id, COLUMN_DELIMITER, encodeString(password)); // save password and id as encoded
            // password will not be stored in user and will only be used for login purposes;
            user->loggedIn = 1;
        }
        else
        {
            // TODO: maybe folder doesnt exist!
            throwUserError(user, "Cannot save user credentials!"); // for making sure user is not logged in without data stored in Data
        }
        // close the file
        fclose(userFile);
    }
    else
    {
        // ERROR:
        throwUserError(user, "Cannot assign an id to you!");
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
        string row = String(MAX_USER_FILE_ROW_LENGTH);

        if (!feof(userFile) && fgets(row, MAX_USER_FILE_ROW_LENGTH, userFile) != NULL // the first row are headers(titles); also its a good practice to check that as an insurance that file data is stored correctly.
            && !feof(userFile) && fgets(row, MAX_USER_FILE_ROW_LENGTH, userFile) != NULL) // reading the actual line that contains the credentials and id
        { 
            removeNextlineCharacter(row);
            // the second call to fgets will read the credential line
            const string id = (string)strtok(row, COLUMN_DELIMITER); // split the text by COLUMN_DELIMITER[,] character
            string credential = (string)strtok(NULL, COLUMN_DELIMITER); // get the next column
            credential = trimColumnValue(credential);

            if (id != NULL && !strcmp(encodeString(password), credential))
            {
                user->id = atol(id);
                // if passwords are equal then login
                user->loggedIn = 1;
                fclose(userFile);
                return user;
            }

        }
    }

    throwUserError(user, "Username or password is wrong! Please try again ..."); // identifier of error is an empty user
    
    // else user will remain logged out ...
    // close the file
    fclose(userFile);
    return user;
}

string validateRegisterationInput(char inputUser[], char inputPassword[])
{
    // TODO: checks the input, returns the error string or NULL(if ok!)
    return NULL;
}