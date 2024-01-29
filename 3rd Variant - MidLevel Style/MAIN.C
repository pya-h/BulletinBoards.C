#include <stdlib.h>
#include <stdio.h>
#include "BOARDS-DATA.H"
#include <string.h>
#include <ctype.h>
#include <conio.h>

void ShowBoards(struct BOARD Boards[], int End);

void ShowLists(struct BOARD Boards[], int Bn); // Bn = Board Number

void ShowTasks(struct LIST Lists[], int Ln); // Ln = List Number

void ShowOneTask(struct TASK Task);

int main()
{
    char *UserName = NULL; // Agar In Meghdar Begirad Yani Karbar Vorod Movafagh Dashte /.
    char *Password = NULL;

    struct BOARDS_DATA *BoardsData = NULL;
    int CurrentBoardNumber = -1, CurrentListNumber = -1;

    while (1)
    {
        if (BoardsData == NULL)
        { // Agar Login Nashodeh
            int user_folder;
            char InputUser[1000], InputPassword[1000], Input[1000];
            // Dastorateh Login: /.
            // login username InputPassword /.
            // register username InputPassword /.
            puts("Please login or register:");
            scanf("%s %s %s%*c", Input, InputUser, InputPassword);

            if (strcmp(Input, "login") == NULL)
            {
                BoardsData = CheckLogin(InputUser, InputPassword);
                if (BoardsData == NULL)
                    puts("Username or Password Incorrect! Try again!");
                else
                {
                    UserName = InputUser;
                    Password = InputPassword;
                }
            }
            else if (strcmp(Input, "register") == NULL)
            {
                if(NULL != CheckLogin(InputUser, InputPassword)){
                    puts("This Username Is Taken. Try Another!");
                } else {

                    BoardsData = (struct BOARDS_DATA *)malloc(sizeof(struct BOARDS_DATA) * 1);
                    PrepareBoardsData(BoardsData);
                    UserName = InputUser;
                    Password = InputPassword;
                    if (UserName == NULL)
                        puts("Register Error! Try again!\n");
                }
            }
        }
        else
        {
            struct BOARD *Boards = BoardsData->Boards;

            char Selection;
            if (CurrentBoardNumber < 0)
            {
                puts("[S]HOW BOARDS  [A]DD BOARD  [G]OTO BOARD  ?");
                Selection = getche();
            }
            else
            {
                Selection = 'g';
            }
            Selection = tolower(Selection); // Momken ast CAPS LOCK Karbar Roshan Bashad. Intori Dar Har Do Halat Vorodi Motabar Khahad Bood/.
            puts("");
            if (Selection == 'g')
            {
                int Bn = 0;
                if (CurrentBoardNumber < 0) // In Bakhsh barayeh In Ast Ke Agar Karbar MAsalan Be Board 1 om Raft, Dar Marheleye Ba'd Dobareh
                // Be Safheyeh Entekhab Board Bar Nagardad
                // Daghighan CurrentListNumber Hamin Kar Ra Barayeh List ha mikonad
                {
                    ShowBoards(Boards, BoardsData->Bi);
                    printf(" => Select: ");
                    scanf("%d%*c", &Bn);
                    if (Bn < 0 || Bn >= BoardsData->Bi)
                    {
                        CurrentBoardNumber = CurrentListNumber = -1;
                        puts("Board Number Invalid! Try Again Later.");
                        continue; // Bargard Be Menu Asli
                    }
                    CurrentBoardNumber = Bn;
                }
                else
                {
                    Bn = CurrentBoardNumber;
                }

                // Shoroe'e Menu List haye in Board
                // Hamanand Menu Ghabli, Amma Inbar Barayeh Listha
                // system("cls");
                char Selection2;
                if (CurrentListNumber < 0)
                {
                    puts("Current Board: ");
                    puts(Boards[Bn].Name);
                    puts(" => [E]DIT BOARD NAME  [D]ELETE BOARD  [S]HOW LISTS  [A]DD LIST  [G]OTO LIST  [P]revious  ?");
                    Selection2 = tolower(getche());
                    puts("");
                }
                else
                {
                    Selection2 = 'g';
                }

                if (Selection2 == 's')
                {
                    ShowLists(Boards, Bn);
                }
                else if (Selection2 == 'a')
                {
                    puts("Enter List Name:");
                    PrepareNextList(Boards, Bn);
                    gets(Boards[Bn].Lists[Boards[Bn].Li].Name);
                    puts("Done.");
                    Boards->Li++;
                    WriteData(UserName, Password, BoardsData);
                }
                else if (Selection2 == 'e')
                {
                    puts("New Board Name: ");
                    gets(Boards[Bn].Name);
                    puts("Done.");
                    WriteData(UserName, Password, BoardsData);
                }
                else if (Selection2 == 'd')
                {
                    // Hazveh cascade
                }
                else if (Selection2 == 'p')
                {
                    CurrentBoardNumber = -1;
                }
                else if (Selection2 == 'g')
                {
                    int Ln = 0;
                    if (CurrentListNumber < 0)
                    {
                        ShowLists(Boards, Bn);
                        printf("Select: ");
                        scanf("%d%*c", &Ln);
                        if (Ln < 0 || Ln >= Boards[Bn].Li)
                        {
                            puts("List Number Invalid! Try Again Later.\n");
                            continue; // Bargard Be Menu Asli
                        }
                        CurrentListNumber = Ln;
                    }
                    else
                    {
                        Ln = CurrentListNumber;
                    }
                    // Menu Task ha
                    // system("cls");
                    printf("\nList:%s of Board:%s\n", Boards[Bn].Lists[Ln].Name, Boards[Bn].Name);
                    puts(" =>  [E]DIT LIST NAME  [D]ELETE LIST  [S]HOW TASKS  [A]DD TASK  [G]OTO TASK  [P]revious ?");

                    char Selection3 = tolower(getche());
                    puts("");
                    if (Selection3 == 's')
                    {
                        ShowTasks(Boards[Bn].Lists, Ln);
                    }
                    else if (Selection3 == 'a')
                    {
                        CheckListCapacityForTasks(Boards, Bn, Ln);
                        struct TASK *NewTask = &Boards[Bn].Lists[Ln].Tasks[Boards[Bn].Lists[Ln].Ti];
                        puts("Enter Task Name:\n");
                        gets(NewTask->Name);
                        puts("Enter Task Importance: [H]igh [M]edium [L]ow\n");
                        gets(NewTask->Importance);
                        SetImportance(NewTask);
                        puts("Enter Deadline D M Y");
                        scanf("%d %d %d%*c", &NewTask->Deadline.D, &NewTask->Deadline.M, &NewTask->Deadline.Y);
                        puts("Done.");
                        Boards[Bn].Lists[Ln].Ti++;
                        WriteData(UserName, Password, BoardsData);
                    }
                    else if (Selection3 == 'e')
                    {
                        puts("Enter Another List Name: ");
                        gets(Boards[Bn].Lists[Ln].Name);
                        puts("Done.");
                        WriteData(UserName, Password, BoardsData);
                    }
                    else if (Selection3 == 'd')
                    {
                        // Hazveh cascade
                    }
                    else if (Selection3 == 'p')
                    {
                        CurrentListNumber = -1;
                    }
                    else if (Selection3 == 'g')
                    {
                        int Tn = 0; // Task number
                        ShowTasks(Boards[Bn].Lists, Ln);
                        printf("  =>  Select: ");
                        scanf("%d%*c", &Tn);
                        if (Tn < 0 || Tn >= Boards[Bn].Lists[Ln].Ti)
                        {
                            puts("Task Number Invalid! Please Try Again!");
                            continue;
                        }
                        puts(" =>  [E]DIT TASK [D]ELETE TASK  [S]HOW THIS TASK [P]revious ?");

                        char Selection4 = getche(); // get;
                        puts("");
                        if (Selection4 == 'e')
                        {
                            struct TASK *NewTask = &Boards[Bn].Lists[Ln].Tasks[Tn];
                            puts("Edit Task Name:");
                            gets(NewTask->Name);
                            puts("Edit Task Importance: [H]igh [M]edium [L]ow");
                            gets(NewTask->Importance);
                            SetImportance(NewTask);
                            puts("Edit Deadline D M Y");
                            scanf("%d %d %d", &NewTask->Deadline.D, &NewTask->Deadline.M, &NewTask->Deadline.Y);
                            puts("Done.");
                            WriteData(UserName, Password, BoardsData);
                        }
                        else if (Selection4 == 'p')
                        {
                            continue;
                        }
                        else if (Selection4 == 'd')
                        {
                            ShowOneTask(Boards[Bn].Lists[Ln].Tasks[Tn]);
                            puts("Delete This Task? [Y]es ");
                            char answer = tolower(getche());
                            if (answer == 'y')
                            {
                                for (int i = Tn; i < Boards[Bn].Lists[Ln].Ti - 1; i++)
                                {
                                    Boards[Bn].Lists[Ln].Tasks[i].Deadline.D = Boards[Bn].Lists[Ln].Tasks[i + 1].Deadline.D;
                                    Boards[Bn].Lists[Ln].Tasks[i].Deadline.M = Boards[Bn].Lists[Ln].Tasks[i + 1].Deadline.M;
                                    Boards[Bn].Lists[Ln].Tasks[i].Deadline.Y = Boards[Bn].Lists[Ln].Tasks[i + 1].Deadline.Y;

                                    strncpy(Boards[Bn].Lists[Ln].Tasks[i].Importance,  Boards[Bn].Lists[Ln].Tasks[i + 1].Importance, sizeof(Boards[Bn].Lists[Ln].Tasks[i + 1].Importance));
                                    strncpy(Boards[Bn].Lists[Ln].Tasks[i].Name, Boards[Bn].Lists[Ln].Tasks[i + 1].Name, sizeof(Boards[Bn].Lists[Ln].Tasks[i + 1].Name));
                                }
                                Boards[Bn].Lists[Ln].Ti--;
                                free(&Boards[Bn].Lists[Ln].Tasks[Boards[Bn].Lists[Ln].Ti]);
                                WriteData(UserName, Password, BoardsData);
                            }
                        }
                    }
                }
            }
            else if (Selection == 'a')
            {
                puts("Enter Board Name:\n");

                PrepareNextBoard(BoardsData);
                gets(Boards[BoardsData->Bi].Name);
                puts("Done.");
                BoardsData->Bi++;
                WriteData(UserName, Password, BoardsData);
            }
            else if (Selection == 's')
            {
                ShowBoards(Boards, BoardsData->Bi);
            }
        }
    }
}

void ShowBoards(struct BOARD Boards[], int End)
{
    puts("All Boards:");
    for (int i = 0; i < End; i++)
    {
        printf(" %d) %s\n", i, Boards[i].Name);
    }
}

void ShowLists(struct BOARD Boards[], int Bn) // Bn = Board Number
{
    puts("Lists of Selected Board:"); // PrepareNext Line
    for (int i = 0; i < Boards[Bn].Li; i++)
    {
        printf(" %d) %s\n", i, Boards[Bn].Lists[i].Name);
    }
}

void ShowTasks(struct LIST Lists[], int Ln) // Ln = List Number
{
    puts("Tasks of Selected List And Board:");
    for (int i = 0; i < Lists[Ln].Ti; i++)
    {
        printf("  %d) ", i);
        ShowOneTask(Lists[Ln].Tasks[i]);
    }
}

void ShowOneTask(struct TASK Task)
{

    printf("%s, Importance: %s, Deadline: %d/%d/%d\n", Task.Name, Task.Importance,
           Task.Deadline.D, Task.Deadline.D, Task.Deadline.Y);
}