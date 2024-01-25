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
        Long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char name[MAX_USERNAME_LENGTH];

        // place where this user credentials are stored
        char location[MAX_FILENAME_LENGTH];

        unsigned short loggedIn; // if the user is logged in or logined,
        // it can be checked by this flag

        char error[MAX_RESPONSE_LENGTH];
    } User; // we use typedef to make shortcuts, so we use for example 'User' instead of 'struct User'

    // prototypes:
    User *newUser();
    User *registerUser(char username[], char password[]); // regitserUser
    User *loginUser(char username[], char password[]);    // loginUser
    string validateRegisterationInput(char inputUser[], char inputPassword[]);
    void User_reset(User *user);
    void User_failure(User*, string);

    string User_getError(User *user);
#endif // USER_H