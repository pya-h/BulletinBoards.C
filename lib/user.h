#ifndef USER_H
#define USER_H
    #include "base.h"

    #define FOLDER_USERS "Users"
    #define MAX_USERNAME_LENGTH 32
    #define MAX_PASSWORD_LENGTH 20
    #define MAX_ENCODED_PASS_LENGTH MAX_ENCODED_STRING_LENGTH(MAX_PASSWORD_LENGTH)
    #define MAX_USER_FILE_ROW_LENGTH  MAX_ENCODED_PASS_LENGTH + 20 // 20 is the exyta length containing Id and commos and etc. 

    typedef struct User
    {
        long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char name[MAX_USERNAME_LENGTH];

        // place where this user credentials are stored
        char location[MAX_FILENAME_LENGTH];

        unsigned short loggedIn; // if the user is logged in or logined,
        // it can be checked by this flag

        long currentBoardId, currentListId;

        char error[MAX_RESPONSE_LENGTH];
    } User;

    // prototypes:
    User *newUser();
    User *registerUser(char username[], char password[]); // regitserUser
    User *loginUser(char username[], char password[]);    // loginUser
    string validateRegisterationInput(char inputUser[], char inputPassword[]);
    void resetUser(User *user);
    void throwUserError(User*, string);
#endif // USER_H