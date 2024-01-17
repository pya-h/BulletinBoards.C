#include <stdio.h>
#include "lib/user.h"

// authentication options
#define MENU_LOGIN_ATTEMPT 1
#define MENU_REGISTERATION_ATTEMPT 2

// boards options
#define MENU_VIEW_BOARDS 1
#define MENU_CREATE_BOARD 2
#define MENU_DELETE_BOARD 2

User *authenticationAttempt(short);
short authenticationMenu();
short boardsMenu();

int main() {
    User *u = newUser(); // U as for 'you'/username
    short choice;
    while(1) {
        CLEAR_SCREEN();
        if(u->loggedIn) {
            if(!u->currentBoardId) {
                choice = boardsMenu();
                switch(choice) {
                    case MENU_CREATE_BOARD:

                        break;
                }
            } else {
                // TODO:
                // if user has selected a board
            }
        } else {
            choice = authenticationMenu();
            u = authenticationAttempt(choice); // choice is matched with MENU_LOGIN_ATTEMPT/MENU_REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if(!u->loggedIn) {
                printf("Username or password is wrong! Try again ...\n");
            } else {
                printf("Hi %s, Welcome! \n", u->name);
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
    printf("\t%d Login\n\t%d Register ", MENU_LOGIN_ATTEMPT, MENU_REGISTERATION_ATTEMPT);
    return GET_KEY();
}

User *authenticationAttempt(short attemptType) {
    // attemptType: 0 login
    // attemptType: 1 registeration
    printf("\n%s:\n", attemptType == MENU_REGISTERATION_ATTEMPT ? "Registeration" : "Login");
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("\tUsername: ");
    gets(username);
    printf("\tPassword: ");
    gets(password);
    if(attemptType == MENU_REGISTERATION_ATTEMPT) {
        char *validatonResult = validateRegisterationInput(username, password);
        if(validateRegisterationInput) {
            // if not null means wrong input:
        }
    }
    return attemptType == MENU_REGISTERATION_ATTEMPT ? registerUser(username, password) : loginUser(username, password);
}

short boardsMenu() {
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d View Boards\n\t%d Create New Board\n\t%d Delete Board ", MENU_VIEW_BOARDS, MENU_CREATE_BOARD, MENU_DELETE_BOARD);
    return GET_KEY();
}