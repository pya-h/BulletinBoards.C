#ifndef USER_H
#define USER_H
#include "base.h"

#define FOLDER_USERS "users"
#define MAX_USERNAME_LENGTH 64
#define MAX_PASSWORD_LENGTH 20


// #define MAX_ENCRYPTED_PASSWORD_LENGTH 40
// #define PASSWORD_SALT "abXN_H-d!~"

struct User {
    long id; // will be set by the current time value

    char name[MAX_USERNAME_LENGTH];
    // char password[MAX_ENCRYPTED_PASSWORD_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    // place where this user credentials are stored
    char location[MAX_FILENAME_LENGTH];
};

typedef struct User User;

// prototypes:
User *registerUser(char username[], char password[]); // regitserUser 
#endif // USER_H