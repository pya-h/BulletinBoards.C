#include <stdio.h>
#include "lib/user.h"

#define LOGIN_ATTEMPT 1
#define REGISTERATION_ATTEMPT 2
User *authenticationAttempt(short);
short authenticationMenu();
short appMenu();

int main() {
    User *u = NewUser(); // U as for 'you'/username
    short choice;
    while(1) {
        CLEAR_SCREEN();
        if(u->loggedIn) {
            choice = appMenu();
        } else {
            choice = authenticationMenu();
            u = authenticationAttempt(choice); // choice is matched with LOGIN_ATTEMPT/REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if(!u->loggedIn) {
                printf("Username or password is wrong! Try again ...\n");
            }
        }
        PRESS_KEY_TO_CONTINUE();
    }
    if(u && u->loggedIn) {
        printf("Welcome %s, your id is: %ld!\n", u->name, u->id);
    }
}

short authenticationMenu() {
    CLEAR_SCREEN();
    printf("\nHello there! How can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t1 Login\n\t2 Register ");
    return GET_KEY();
}

User *authenticationAttempt(short attemptType) {
    // attemptType: 0 login
    // attemptType: 1 registeration
    printf("\n%s:\n", attemptType == REGISTERATION_ATTEMPT ? "Registeration" : "Login");
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("\tUsername: ");
    gets(username);
    printf("\tPassword: ");
    gets(password);
    if(attemptType == REGISTERATION_ATTEMPT) {
        char *validatonResult = validateRegisterationInput(username, password);
        if(validateRegisterationInput) {
            // if not null means wrong input:
        }
    }
    return attemptType == REGISTERATION_ATTEMPT ? registerUser(username, password) : loginUser(username, password);
}

short appMenu() {
    printf("Welcome! For Now! \n");
}