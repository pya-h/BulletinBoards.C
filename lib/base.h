#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

#define MAX_FILENAME_LENGTH 256
// write a macro to create all filenames used through the app
// Data related MACROS:
#define FOLDER_DATA "Data"
#define SET_DATA_FILE(dest, variations, identifier) snprintf(dest, sizeof(dest), "%s/%s/%ld", FOLDER_DATA, variations, identifier)
#define SET_USER_DATA_FILE(dest, variations, identifier) snprintf(dest, sizeof(dest), "%s/%s/%s", FOLDER_DATA, variations, identifier)

// UI MACROS:
// some macros are just defined, so if they fail in some compiler,
// we can simply be changed to something else
#define CLEAR_SCREEN() system("cls")
#define GET_KEY() _getch() - '0' // _getch() returns an ASCII code,
// substracting it by '0'(=48) will convert it to normal integer.
#define PRESS_KEY_TO_CONTINUE() system("pause")
#define PRINT_DASH_ROW() printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n")

int prepareFolder(char folder[], short insideData);

#endif // BASE_H