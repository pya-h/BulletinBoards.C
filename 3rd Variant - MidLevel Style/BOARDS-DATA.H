#ifndef BOARDS_DATA
#define BOARDS_DATA
// Mayahyajeh Barnameh Ma
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct DEADLINE
{
    int Y, M, D;
};

// TASK ha
struct TASK
{
    char Name[1000];
    char Importance[10];      // high priority <=> high number
    struct DEADLINE Deadline;
};

// List ha
struct LIST
{
    char Name[1000];
    struct TASK *Tasks;
    int Ti;       // shomareye akharin Task
    int Capacity; // Zarfiateh Arayeh Tasks. Dar Sorateh Niua afzayesh Yafteh va .Tasks realloc Shavad
};

// va Board ha be inSorat Tarif Mishavand
struct BOARD
{
    char Name[1000];
    int Li; // Akharin List
    int Capacity; // Hadaksar tedad List Dar Haleh Hazer. Ghabeleh Afzayesh Heyne realloc Mibashad.
    struct LIST *Lists;
};

void     SetImportance(struct TASK*);
void     CheckBoardCapacityForLists(struct BOARD Boards[], int BoardNumber);
void     CheckListCapacityForTasks(struct BOARD Boards[], int BoardNumber, int ListNumber);

void     WriteData(char *UserName, char *Password, struct BOARD Boards[], int End);
// FILE *get_user_file(char *index_filename, char username[], char password[]);
// int get_user_folder(char username[]);
// int get_next_folder_number();
// int register_user(char username[], char password[]);
// FILE *open_file_for_read(char filename[]);
// char *get_filename(int user_folder_number, char *file_type);
// struct user *login_user(int folder_number, char username[], char password[]);
// int save(struct user *user);
// int load(struct user *user);
// void show_reminder(struct user *user);

#endif // Payan Header BOARDS-DATA.h