#ifndef USER_H
#define USER_H
#include "base.h"

#define FOLDER_USERS "Users"
#define MAX_USERNAME_LENGTH 64
#define MAX_PASSWORD_LENGTH 20

// #define MAX_ENCRYPTED_PASSWORD_LENGTH 40
// #define PASSWORD_SALT "abXN_H-d!~"

struct User
{
    long id; // will be set by the current time value
    // id will be used for ownership of other items (such as tasks)
    char name[MAX_USERNAME_LENGTH];

    // place where this user credentials are stored
    char location[MAX_FILENAME_LENGTH];

    unsigned short loggedIn; // if the user is logged in or logined,
    // it can be checked by this flag

    long currentBoardId, currentListId;
};

typedef struct User User;

// prototypes:
User *newUser();
User *registerUser(char username[], char password[]); // regitserUser
User *loginUser(char username[], char password[]);    // loginUser
char *validateRegisterationInput(char inputUser[], char inputPassword[]);
void resetUser(User *user);

#endif // USER_H