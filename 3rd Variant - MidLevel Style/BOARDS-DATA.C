#include "BOARDS-DATA.H"


void SetImportance(struct TASK *Task)
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
void WriteData(char *UserName, char *Password, struct BOARD Boards[], int End)
{
    FILE *DataCsvFile = fopen("DATA.CSV", "w");

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

    fprintf(DataCsvFile, "USER,%d,%s,%s\n", End, UserName, Password);

    for (int i = 0; i < End; i++)
    {
        // Write a Bakhsh Board ha
        fprintf(DataCsvFile, "BOARD,%d,%s\n", Boards[i].Li, Boards[i].Name);

        //Write a List a har Board
        for(int j = 0; j < Boards[i].Li; j++)
        {
            fprintf(DataCsvFile, "LIST,%d,%s\n", Boards[i].Lists[j].Name);

            // write a TASK haye har LIST
            for(int k = 0; k < Boards[i].Lists[j].Ti; k++)
            {
                fprintf(DataCsvFile, "TASK,0,%s,%s,%d,%d,%d\n", Boards[i].Lists[j].Tasks[k].Name, Boards[i].Lists[j].Tasks[k].Importance,  // TASK ha Sub Item Nadarand.
                    Boards[i].Lists[j].Tasks[k].Deadline.D, Boards[i].Lists[j].Tasks[k].Deadline.M, Boards[i].Lists[j].Tasks[k].Deadline.Y);
            }
        }
    }
    fclose(DataCsvFile);
}