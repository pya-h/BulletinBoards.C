#ifndef manager_h
#define manager_h
 // file manager; write and read data models on files.
 
#include "models.h"
#include "stdlib.h"
#include "stdio.h"
#include "windows.h"
#define errno_user_list_not_available -1
#define errno_folder_creation_failed -2

int get_user_folder(char username[]);
int get_next_folder_number();
int register_user(char username[], char password[]);

#endif