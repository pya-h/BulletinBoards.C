#include <stdlib.h>
#include <stdio.h>
#include "BOARDS-DATA.H"
#include <string.h>
#include <ctype.h>
#include <conio.h>

void ShowBoards(struct BOARD Boards[], int End)
{
    puts("\n");
    for (int i = 0; i < End; i++)
    {
        printf(" %d) %s\n", i, Boards[i].Name);
    }
}

void ShowLists(struct BOARD Boards[], int Bn) // Bn = Board Number
{
    puts(""); // Next Line
    for (int i = 0; i < Boards[Bn].Li; i++)
    {
        printf(" %d) %s\n", i, Boards[Bn].Lists[i].Name);
    }
}

void ShowTasks(struct LIST Lists[], int Ln) // Ln = List Number
{
    puts("");
    for (int i = 0; i < Lists[Ln].Ti; i++)
    {
        printf(" %d) %s, Importance: %s, Deadline: %d/%d/%d\n", i, Lists[Ln].Tasks[i].Name, Lists[Ln].Tasks[i].Importance,
               Lists[Ln].Tasks[i].Deadline.D, Lists[Ln].Tasks[i].Deadline.D, Lists[Ln].Tasks[i].Deadline.Y);
    }
}

int main()
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
            puts("[S]HOW BOARDS  [A]DD BOARD  [G]OTO BOARD  ?");
            char Input[1000];
            gets(Input);
            if (Input[1] != NULL)
            {
                puts("Invalid Selection. Please Try Again...\n");
                continue;
            }
            char Selection = Input[0];
            Selection = tolower(Selection); // Momken ast CAPS LOCK Karbar Roshan Bashad. Intori Dar Har Do Halat Vorodi Motabar Khahad Bood/.
            if (Selection == 'a')
            {
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
            else if (Selection == 'g')
            {
                int Bn = 0;
                ShowBoards(Boards, Bi);
                puts("Select: ");
                scanf("%d%*c", &Bn);
                if (Bn < 0 || Bn >= Bi)
                {

                    puts("Board Number Invalid! Try Again Later.\n");
                    continue; // Bargard Be Menu Asli
                }

                // Shoroe'e Menu List haye in Board
                // Hamanand Menu Ghabli, Amma Inbar Barayeh Listha
                // system("cls");
                puts("\nCurrent Board: ");
                puts(Boards[Bn].Name);
                puts(" => [E]DIT BOARD NAME  [D]ELETE BOARD  [S]HOW LISTS  [A]DD LIST  [G]OTO LIST  ?");

                gets(Input);
                if (Input[1] != NULL)
                {
                    puts("Invalid Selection. Please Try Again...\n");
                    continue;
                }
                char Selection2 = Input[0];
                Selection2 = tolower(Selection2);
                if (Selection2 == 's')
                {
                    ShowLists(Boards, Bn);
                }
                else if (Selection2 == 'a')
                {
                    puts("Enter List Name:");
                    gets(Boards[Bn].Lists[Boards[Bn].Li].Name);
                    Boards[Bn].Lists[Boards[Bn].Li].Ti = 0;
                    Boards[Bn].Lists[Boards[Bn].Li].Capacity = 100;
                    Boards[Bn].Lists[Boards[Bn].Li].Tasks = (struct TASK *)malloc(sizeof(struct TASK) * Boards[Bn].Lists[Boards[Bn].Li].Capacity);
                    puts("Done.");
                    Boards[Bn].Li++;
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
                else if (Selection2 == 'g')
                {
                    int Ln = 0;
                    ShowLists(Boards, Ln);
                    puts("Select: ");
                    scanf("%d%*c", &Ln);
                    if (Ln < 0 || Ln >= Boards[Bn].Li)
                    {
                        puts("List Number Invalid! Try Again Later.\n");
                        continue; // Bargard Be Menu Asli
                    }

                    // Menu Task ha
                    // system("cls");
                    printf("\nList:%s of Board:%s\n", Boards[Bn].Lists[Ln].Name, Boards[Bn].Name);
                    puts(" =>  [E]DIT LIST NAME  [D]ELETE LIST  [S]HOW TASKS  [A]DD TASK  [G]OTO TASK  ?");

                    gets(Input);
                    if (Input[1] != NULL)
                    {
                        puts("Invalid Selection. Please Try Again...\n");
                        continue;
                    }
                    char Selection3 = Input[0];
                    Selection3 = tolower(Selection3);

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
                        struct TASK *NewTask = &Boards[Bn].Lists[Ln].Tasks[Boards[Bn].Lists[Ln].Ti];
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
                    else if (Selection3 == 'd')
                    {
                        // Hazveh cascade
                    }
                }
            }
        }
    }
}