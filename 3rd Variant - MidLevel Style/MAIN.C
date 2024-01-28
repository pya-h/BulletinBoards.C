#include <stdlib.h>
#include <stdio.h>
#include "BOARDS-DATA.H"
#include <string.h>
#include <ctype.h>
#include <conio.h>

void    ShowBoards(struct BOARD Boards[], int End);

void    ShowLists(struct BOARD Boards[], int Bn); // Bn = Board Number

void    ShowTasks(struct LIST Lists[], int Ln); // Ln = List Number

void    ShowOneTask(struct TASK Task);

int     main()
{
    char UserName[] = "test"; // Agar In Meghdar Begirad Yani Karbar Vorod Movafagh Dashte /.
    char Password[] = "test";
    int BoardCapacity = 100;
    int Bi = 0;                                                                          // Neshangare Akharin Board /.
    struct BOARD *Boards = (struct BOARD *)malloc(sizeof(struct BOARD) * BoardCapacity); // arayei Az Boards ba tool Avalieh 100 /.
    int CurrentBoardNumber = -1, CurrentListNumber = -1;
    while (1)
    {
        if (UserName == NULL)
        { // Agar Login Nashodeh
            int user_folder;
            char InputUser[1000], InputPassword[1000], Input[1000];
            // Dastorateh Login: /.
            // login username InputPassword /.
            // register username InputPassword /.
            puts("Please login or register:");
            scanf("%s %s %s", Input, InputUser, InputPassword);

            if (strcmp(Input, "login") == NULL)
            {
                // UserName = login_user(InputUser, Password);
                if (UserName == NULL)
                    puts("Username or Password Incorrect! Try again!\n");
            }
            else if (strcmp(Input, "register") == NULL)
            {
                // UserName = register_user(username, password);
                if (UserName == NULL)
                    puts("Register Error! Try again!\n");
            }
        }
        else
        {
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
                    ShowBoards(Boards, Bi);
                    printf(" => Select: ");
                    scanf("%d%*c", &Bn);
                    if (Bn < 0 || Bn >= Bi)
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
                    CheckBoardCapacityForLists(Boards, Bn);
                    gets(Boards[Bn].Lists[Boards[Bn].Li].Name);
                    Boards[Bn].Lists[Boards[Bn].Li].Ti = 0;
                    Boards[Bn].Lists[Boards[Bn].Li].Capacity = 100;
                    Boards[Bn].Lists[Boards[Bn].Li].Tasks = (struct TASK *)malloc(sizeof(struct TASK) * Boards[Bn].Lists[Boards[Bn].Li].Capacity);
                    puts("Done.");
                    Boards[Bn].Li++;
                    WriteData(UserName, Password, Boards, Bi);
                }
                else if (Selection2 == 'e')
                {
                    puts("New Board Name: ");
                    gets(Boards[Bn].Name);
                    puts("Done.");
                    WriteData(UserName, Password, Boards, Bi);
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
                        WriteData(UserName, Password, Boards, Bi);
                    }
                    else if (Selection3 == 'e')
                    {
                        puts("Enter Another List Name: ");
                        gets(Boards[Bn].Lists[Ln].Name);
                        puts("Done.");
                        WriteData(UserName, Password, Boards, Bi);
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
                            gets(NewTask->Name);
                            puts("Done.");
                            WriteData(UserName, Password, Boards, Bi);
                        }
                        else if (Selection4 == 'p')
                        {
                            continue;
                        }
                    }
                }
            }
            else if (Selection == 'a')
            {
                if (Bi >= BoardCapacity)
                {
                    BoardCapacity += 10;
                    Boards = (struct BOARD *)realloc(Boards, sizeof(struct BOARD) * BoardCapacity);
                }
                puts("Enter Board Name:\n");
                gets(Boards[Bi].Name);
                Boards[Bi].Capacity = 100;
                Boards[Bi].Lists = (struct LIST *)malloc(sizeof(struct LIST) * Boards[Bi].Capacity);
                puts("Done.");
                Bi++;
                WriteData(UserName, Password, Boards, Bi);
            }
            else if (Selection == 's')
            {
                ShowBoards(Boards, Bi);
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
    puts("Lists of Selected Board:"); // Next Line
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