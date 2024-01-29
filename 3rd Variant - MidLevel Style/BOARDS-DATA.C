#include "BOARDS-DATA.H"

void    SetImportance(struct TASK *Task)
{
    // Harf avval ->Importance Ra Check mikonad va Meghdar Motabar daronash Gharar Midahad
    Task->Importance[0] = toupper(Task->Importance[0]);
    if (Task->Importance[0] = 'H')
    {
        strcpy(Task->Importance, "High"); // Kamel kardane matn Importance
    }
    else if (Task->Importance[0] = 'M')
    {
        strcpy(Task->Importance, "Medium"); // Kamel kardane matn Importance
    }
    else if (Task->Importance[0] = 'L')
    {
        strcpy(Task->Importance, "Low"); // Kamel kardane matn Importance
    }
}


void     PrepareBoardsData(struct BOARDS_DATA *BoardsData)
{
        BoardsData->Capacity = 20;
        BoardsData->Bi = 0;
        BoardsData->Boards = (struct BOARD *) malloc(sizeof(struct BOARD) * BoardsData->Capacity);
}

void     PrepareNextBoard(struct BOARDS_DATA *BoardsData)
{
        struct BOARD *Boards = BoardsData->Boards;
        Boards[BoardsData->Bi].Capacity = 100;
        Boards[BoardsData->Bi].Lists = (struct LIST *)malloc(sizeof(struct LIST) * Boards[BoardsData->Bi].Capacity);
        CheckBoardCapacity(BoardsData);
}

void     PrepareNextList(struct BOARD Boards[], int BoardNumber)
{
        Boards[BoardNumber].Lists[Boards[BoardNumber].Li].Ti = 0;
        Boards[BoardNumber].Lists[Boards[BoardNumber].Li].Capacity = 100;
        Boards[BoardNumber].Lists[Boards[BoardNumber].Li].Tasks = (struct TASK *)malloc(sizeof(struct TASK) * Boards[BoardNumber].Lists[Boards[BoardNumber].Li].Capacity);
        CheckBoardCapacityForLists(Boards, BoardNumber);

}

void     CheckBoardCapacity(struct BOARDS_DATA *BoardsData)
{
    if (BoardsData->Bi >= BoardsData->Capacity)
    {
        BoardsData->Capacity += 10;
        BoardsData->Boards = (struct BOARD *)realloc(BoardsData->Boards, sizeof(struct BOARD) * BoardsData->Capacity);
    }
}

void    CheckBoardCapacityForLists(struct BOARD Boards[], int BoardNumber)
{
    if (Boards[BoardNumber].Li >= Boards[BoardNumber].Capacity)
    {
        Boards[BoardNumber].Capacity += 10;
        Boards[BoardNumber].Lists = (struct LIST *)realloc(Boards[BoardNumber].Lists, Boards[BoardNumber].Capacity);
    }
}

void    CheckListCapacityForTasks(struct BOARD Boards[], int BoardNumber, int ListNumber)
{
    if (Boards[BoardNumber].Lists[ListNumber].Ti >= Boards[BoardNumber].Lists[ListNumber].Capacity)
    {
        Boards[BoardNumber].Lists[ListNumber].Capacity += 50;
        Boards[BoardNumber].Lists[ListNumber].Tasks = (struct TASK *)realloc(Boards[BoardNumber].Lists[ListNumber].Tasks, Boards[BoardNumber].Lists[ListNumber].Capacity);
    }
}

void    WriteData(char *UserName, char *Password, struct BOARDS_DATA *BoardsData)
{
        FILE *DataCsvFile = fopen("DATA.CSV", "w");
        struct BOARD *Boards = BoardsData->Boards;
        if (DataCsvFile == NULL)
        {
            puts("Error Writing Data. Cannot Open CSV File!");
            return;
        }

        fprintf(DataCsvFile, "ROW_TYPE,SUB_ITEMS,NAME,PASS/IMPORANCE,DEADLINE_DAY,DEADLINE_MONTH,DEADLINE_YEAR\n");
        // SUB_ITEMS Yani field marboteh che tedad Zir Majmoe Darad. Chon Sakhtar File Ingoneh Ast:
        // USER     2     user1   pass1     // yani in user 2 Board darad
        // BOARD    2       board1          // yani in board ham 2 ZirMajmoe(List) Darad
        //  LIST    0       list1           // In List Taski nadarad
        //  LIST    1       list2
        //  TASK    0       task1       High    3   5   2024
        // ...
        // USER     user2   pass2       // Va Ela Akhar

        fprintf(DataCsvFile, "USER,%d,%s,%s\n", BoardsData->Bi, UserName, Password);

        for (int i = 0; i < BoardsData->Bi; i++)
        {
            // Write a Bakhsh Board ha
            fprintf(DataCsvFile, "BOARD,%d,%s\n", Boards[i].Li, Boards[i].Name);

            // Write a List a har Board
            for (int j = 0; j < Boards[i].Li; j++)
            {
                fprintf(DataCsvFile, "LIST,%d,%s\n", Boards[i].Lists[j].Ti, Boards[i].Lists[j].Name);

                // write a TASK haye har LIST
                for (int k = 0; k < Boards[i].Lists[j].Ti; k++)
                {
                    fprintf(DataCsvFile, "TASK,0,%s,%s,%d,%d,%d\n", Boards[i].Lists[j].Tasks[k].Name, Boards[i].Lists[j].Tasks[k].Importance, // TASK ha Sub Item Nadarand.
                            Boards[i].Lists[j].Tasks[k].Deadline.D, Boards[i].Lists[j].Tasks[k].Deadline.M, Boards[i].Lists[j].Tasks[k].Deadline.Y);
                }
            }
        }
        fclose(DataCsvFile);
}

int      IsColumnSeparator(char c)
{
    return c == NULL || c == ',' || c == '\n' || c == EOF;
}

void     ReadStringColumn(char *Destination, FILE *SourceCsvFile)
{
        // character be character mikhanim
        if(SourceCsvFile != NULL)
        {
            char *S = Destination;
            char FileCharacter = getc(SourceCsvFile);
            while(IsColumnSeparator(FileCharacter))
                FileCharacter = getc(SourceCsvFile);
            while(IsColumnSeparator(FileCharacter))
                FileCharacter = getc(SourceCsvFile);
            int NotReachedMaximumLength = 999;
            while(!feof(SourceCsvFile) && !IsColumnSeparator(FileCharacter) && NotReachedMaximumLength)
            {
                *S = FileCharacter;
                S++;
                FileCharacter = getc(SourceCsvFile);
                NotReachedMaximumLength--;
            }
            *S = NULL;
            //while(!feof(SourceCsvFile) && !IsColumnSeparator(getc(SourceCsvFile))) { } // Rad kardan kameleh soton
        }
}

int     ReadIntegerColumn(FILE *CsvFile)
{
        int NumberColumn = 0;
        if(CsvFile != NULL)
        {
            char FileCharacter = getc(CsvFile); 
            while(IsColumnSeparator(FileCharacter))
                FileCharacter = getc(CsvFile);
            while(!feof(CsvFile) && !IsColumnSeparator(FileCharacter))
            {
                if(FileCharacter < '0' && FileCharacter > '9')
                {

                    return -1; // Manfi Yek Ettela Midahad Ke In Soton GheyreAddadi ast
                }
                NumberColumn = NumberColumn * 10 + (FileCharacter - 48);
                FileCharacter = getc(CsvFile); 
            }
        }
        return NumberColumn;
}

int     ReadCheckString(FILE *CsvFile,  char *CompareString)
{
        // character be character mikhanim
        // In Tabe Tarkib strcmp va Column Read Ast./
        // return 1 Yani Matn Dakhele File Bozorgtar Ast
        // return 0 Yani Mosavi
        // return -1 Yani Matn Column Kochectar Ast (Az Lahaz Chinesh Dictioary)
        if(CsvFile != NULL)
        {
            char FileCharacter = getc(CsvFile); 
            while(IsColumnSeparator(FileCharacter))
                FileCharacter = getc(CsvFile);
            char *TargetCharacter = CompareString;
            while(!feof(CsvFile) && !IsColumnSeparator(FileCharacter) && !IsColumnSeparator(*TargetCharacter) && *TargetCharacter == FileCharacter)
            {
                FileCharacter = getc(CsvFile); 
                TargetCharacter++;
            }
            char Temp = FileCharacter;
            while(!feof(CsvFile) && !IsColumnSeparator(Temp)) {
                Temp = getc(CsvFile);
            } // Rad kardan kameleh soton
            if(IsColumnSeparator(*TargetCharacter) && IsColumnSeparator(FileCharacter))  // Yani Ta akhar Jolo Raftan Pas Mosavi Bodan
                return 0; // Yani Ta payan Khandan Tamam Kalamat Mosavi Bood.
            if(FileCharacter < *TargetCharacter)
                return -1;
            return 1;
        }
        return -2; // File eror
}
struct  BOARDS_DATA *ReadNextUser(FILE *DataCsvFile, char UserName[], char Password[])
{
    char TYPES_USER[] = "USER";
    char TYPES_BOARD[] = "BOARD";
    char TYPES_LIST[] = "LIST";
    char TYPES_TASK[] = "TASK";
    struct BOARDS_DATA *NextUserData= (struct BOARDS_DATA *) malloc(sizeof(struct BOARDS_DATA) * 1);
    PrepareBoardsData(NextUserData);
    struct BOARD *Boards = NextUserData->Boards;

    if (DataCsvFile == NULL)
    {
        puts("Error Reading Data. Cannot Open CSV File!");
        return NULL;
    }
    char _Temp[1000]; // Rad kardaneh header
    while(!feof(DataCsvFile) && ReadCheckString(DataCsvFile, TYPES_USER) != 0) { }// Info Har User Ba Yek Row Type a USER Aghaz Mishe. 

    // Hala Ya File Payan Yafteh
    // Ya Barnameh Be :USER: rerside /.

    int NumberOfBoards = ReadIntegerColumn(DataCsvFile);
    int IsUserPassValid = ReadCheckString(DataCsvFile, UserName) == 0 && ReadCheckString(DataCsvFile, Password) == 0;
    // Agar Valid Ham Nabod, Barnameh Bayad Info Bekhanad Ta Az In Karbar Rad Shavad Va be Badi Beread (Jahat Login)
    for (int i = 0; !feof(DataCsvFile) && i < NumberOfBoards && ReadCheckString(DataCsvFile, TYPES_BOARD) == 0; i++, NextUserData->Bi++)
    {
        PrepareNextBoard(NextUserData);

        int NumberOfLists = ReadIntegerColumn(DataCsvFile);
        ReadStringColumn(NextUserData->Boards[i].Name, DataCsvFile); // read Board Name
        // Write a List a har Board
        for (int j = 0; !feof(DataCsvFile) && j < NumberOfLists && ReadCheckString(DataCsvFile, TYPES_LIST) == 0; j++, NextUserData->Boards[i].Li++)
        {
            PrepareNextList(NextUserData->Boards, i);
            int NumberOfTasks = ReadIntegerColumn(DataCsvFile);
            ReadStringColumn(NextUserData->Boards[i].Lists[j].Name, DataCsvFile); // read Board Name

            // write a TASK haye har LIST
            for (int k = 0;!feof(DataCsvFile) && k < NumberOfTasks && ReadCheckString(DataCsvFile, TYPES_TASK) == 0; k++, NextUserData->Boards[i].Lists[j].Ti++)
            {
                CheckListCapacityForTasks(Boards, i, j);
                ReadIntegerColumn(DataCsvFile); // skip SUB_ITEMs for Tasks ; Chon Hamisheh 0 ast
                ReadStringColumn(NextUserData->Boards[i].Lists[j].Tasks[k].Name, DataCsvFile);
                ReadStringColumn(NextUserData->Boards[i].Lists[j].Tasks[k].Importance, DataCsvFile);
                NextUserData->Boards[i].Lists[j].Tasks[k].Deadline.D = ReadIntegerColumn(DataCsvFile);
                NextUserData->Boards[i].Lists[j].Tasks[k].Deadline.M = ReadIntegerColumn(DataCsvFile);
                NextUserData->Boards[i].Lists[j].Tasks[k].Deadline.Y = ReadIntegerColumn(DataCsvFile);
            }
        }
    }

    return IsUserPassValid ? NextUserData : NULL;
}

struct BOARDS_DATA   *CheckLogin(char *UserName, char *Password) {
    FILE *CsvFile = fopen("DATA.CSV", "r");
    struct BOARDS_DATA *Result = NULL;
    while(!feof(CsvFile) && CsvFile != NULL && Result == NULL)
    {
        Result = ReadNextUser(CsvFile, UserName, Password);
        fclose(CsvFile);
        return Result;
    }
    fclose(CsvFile);
    return Result;
}