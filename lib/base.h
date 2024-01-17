#ifndef SHARED_H
#define SHARED_H
#include <stdio.h>

#define MAX_FILENAME_LENGTH 256
// write a macro to create all filenames used through the app
#define FOLDER_DATA "Data"
#define SET_DATA_FILE(dest, variations, identifier) snprintf(dest, sizeof(dest), "./%s/%s/%d", FOLDER_DATA, variations, identifier)
#endif // SHARED_H