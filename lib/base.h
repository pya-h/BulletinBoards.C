#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_TITLE_LENGTH 64

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
void removeNextlineCharacter(char *str);
// struct that is the base of our link list
// using void * data-type, we can use this link list for every type in our app.
typedef struct ListItem {
    void *data;
    struct ListItem *next, *prev;
} ListItem;

// we define an outer struct, that will actually contain a link list;
// this way we enhance the performance of the link list operations, 
// by setting the first and last item of the list (instead of finding the last item every time), and some fields [length till now]
typedef struct List {
    ListItem *first, *last;
    long length;
} List;

List *newList();
ListItem *newListItem(void *data);
ListItem *List_add(List *list, void *data);
void *List_at(List *list, long index);
short *List_delete(List *list, long index);
//void **List_toArray(List *list, long index);

#endif // BASE_H