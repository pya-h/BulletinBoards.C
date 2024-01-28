#ifndef manager_h
#define manager_h
 // file manager; write and read data models on files.
 
#include "models.h"
#include "stdlib.h"
#include "stdio.h"
#include "windows.h"
#define errno_user_list_not_available -1
#define errno_folder_creation_failed -2
#define errno_user_files_not_available -3

FILE *get_user_file(char *index_filename, char username[], char password[]) ;
int get_user_folder(char username[]);
int get_next_folder_number();
int register_user(char username[], char password[]);
FILE *open_file_for_read(char filename[]);
char *get_filename(int user_folder_number, char *file_type);
struct user *login_user(int folder_number, char username[], char password[]);
int save(struct user *user);
int load(struct user *user);
void show_reminder(struct user *user);

#endif