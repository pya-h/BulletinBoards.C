#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_TITLE_LENGTH 128

// string identifiers
// the two constant/macro(s) below, is used for defining c strings easier, specially when defining dynamic length ones
#define string char*
#define String(length) (char*) calloc(length, sizeof(char))
// write a macro to create all filenames used through the app
// Data related MACROS:
#define FOLDER_DATA "Data"
#define APP_CONFIG_FILE FOLDER_DATA"/app"
#define SET_DATA_FILE(dest, variations, identifier) snprintf(dest, sizeof(dest), FOLDER_DATA"/%s/%ld.csv", variations, identifier)
#define SET_USER_DATA_FILE(dest, variations, identifier) snprintf(dest, sizeof(dest), FOLDER_DATA"/%s/%s.csv", variations, identifier)
#define ENCODE_SALT "abXN_H-d!~"
#define MAX_ENCODED_STRING_LENGTH(MAX_INPUT_LENGTH) MAX_INPUT_LENGTH + 2 * strlen(ENCODE_SALT) + 1
#define ENCODED_STRING_LENGTH(str) strlen(str) + 2 * strlen(ENCODE_SALT) + 1
#define MAX_RESPONSE_LENGTH 64

#define COLUMN_DELIMITER ","
// UI MACROS:
// some macros are just defined, so if they fail in some compiler,
// we can simply be changed to something else
#define CLEAR_SCREEN() system("cls")
#define GET_KEY() _getch() - '0' // _getch() returns an ASCII code,
// substracting it by '0'(=48) will convert it to normal integer.
#define PRESS_KEY_TO_CONTINUE() system("pause")
#define PRINT_DASH_ROW() printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n")

#define ABS(x) x >= 0 ? x : x * -1
#define DIFF(x, y) ABS(x - y)

void Free(short count, ...);
string encodeString(string input);

void getLine(string dest, string inputMessage);
short prepareFolder(char folder[], short insideData);
short fileExists(string filename); 
// read file junk cleaner
void removeNextlineCharacter(string str);
string trimColumnValue(string col);

// struct that is the base of our link list
// using void * data-type, we can use this link list for every type in our app.
typedef struct ListItem
{
    void *data;
    struct ListItem *next, *prev;
} ListItem;

// we define an outer struct, that will actually contain a link list;
// this way we enhance the performance of the link list operations,
// by setting the first and last item of the list (instead of finding the last item every time), and some fields [length till now]
typedef struct List
{
    ListItem *first, *last;
    long length;
    // next two fields is like a caching mechanism; it remembers the last item index that is accessed
    // when getting another item of a list, if the target index is close to this lastAccessIndex, we use that to process list
    // useful when using a List_at in a for loop
    long lastAccessedIndex; ListItem *lastAccessedItem; 
} List;

void *resetList(List *list);
List *newList();
ListItem *newListItem(void *data);
void ListItem_dump(ListItem*);
ListItem *List_add(List *list, void *data);
void *List_at(List *list, long index);
short List_delete(List *list, long index);
// void **List_toArray(List *list, long index);

#define CONFIG_LOGGED_IN_USER 0
typedef struct AppConfig {
    long currentUser;
    // etc...
} AppConfig;    
void updateAppConfig(AppConfig *config);
#endif // BASE_H
