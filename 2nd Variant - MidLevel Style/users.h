#ifndef users_h
#define users_h
#include <boards.h>

// store user data
// username and password is required to login and register
// other variables are extra
struct user
{
    char username[50],
        password[30],
        first_name[50],
        last_name[50],
        email[200];
    int gender, age;
    struct board *my_boards;
    // user is connected to board data by this link list
    // then board is connected to lists by *lists link list
    // then *lists is connected to it's tasks by *tasks link list

};

#endif
