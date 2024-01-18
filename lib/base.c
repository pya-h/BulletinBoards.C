#include "base.h"

int prepareFolder(char folder[], short insideData)
{
    // creates a folder if not exists
    if (insideData)
    {
        char path[MAX_FILENAME_LENGTH] = {0};
        sprintf(path, "%s/%s", FOLDER_DATA, folder);
        return !_mkdir(path);
    }

    return !_mkdir(folder);
}