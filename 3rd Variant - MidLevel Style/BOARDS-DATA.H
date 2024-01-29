#ifndef BOARDS_DATA_H
#define BOARDS_DATA_H
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

struct BOARDS_DATA
{
    struct BOARD *Boards;
    int Capacity;
    int Bi;
};

void     PrepareBoardsData(struct BOARDS_DATA *BoardsData);
void     PrepareNextBoard(struct BOARDS_DATA *BoardsData);
void     PrepareNextList(struct BOARD Board[], int BoardNumber);

void     SetImportance(struct TASK*);
void     CheckBoardCapacity(struct BOARDS_DATA *BoardsData);
void     CheckBoardCapacityForLists(struct BOARD Boards[], int BoardNumber);
void     CheckListCapacityForTasks(struct BOARD Boards[], int BoardNumber, int ListNumber);

void     WriteData(char *UserName, char *Password, struct BOARDS_DATA *BoardsData);
struct  BOARDS_DATA*     ReadNextUser(FILE *DataCsvFile, char UserName[], char Password[]);
int     ReadIntegerColumn(FILE *CsvFile);
void     ReadStringColumn(char *Destination, FILE *SourceCsvFile);
int     ReadCheckString(FILE *CsvFile, char *CompareString);
int      IsColumnSeparator(char c);
struct BOARDS_DATA   *CheckLogin(char *UserName, char *Password);

#endif // Payan Header BOARDS-DATA.h