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

void removeNextlineCharacter(char *str)
{
    // remove the next line character ['\n'] at the end of string (if exists)
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0'; // Replace '\n' with '\0'
    }
}

ListItem *newListItem(void *data)
{
    ListItem *item = (ListItem *)calloc(sizeof(ListItem), 1);
    item->data = data;
    item->next = item->prev = NULL;
    return item;
}

List *newList()
{
    List *list = (List *)calloc(sizeof(List), 1);
    list->first = list->last = NULL;
    list->length = 0;
    return list;
}

ListItem *List_add(List *list, void *data)
{
    ListItem *item = newListItem(data);
    if (list->first)
    {
        // if there's at list one item in the list, just allocate the ListItem and update the last pointer
        list->last->next = item;
        item->prev = list->last;
        list->last = item;
        list->length++;
    }
    else
    {
        list->first = list->last = item;
        list->length++;
    }
    return list->last;
}

void *List_at(List *list, long index)
{
    // as we have first and last nodes in the list, and the length of the list
    // we can determine which way is faster to process until we reach the desired item: processing from first to index,
    // or process from last to index (backwards)
    ListItem *item;
    int i = 0;
    
    if(index < 0) // negative index means start from end
        index += list->length;

    if(index >= list->length)
        return NULL; // Not found
    if (index < (float)list->length / 2.0)
    {
        for (item = list->first, i = 0; i < index; i++, item = item->next) ; // process the nodes, by going to next pointer of each node, until it reaches to the desired item
    }
    else
    {
        for (item = list->last, i = list->length - 1; i > index; i++, item = item->prev) ; // process the nodes, by going to next pointer of each node, until it reaches to the desired item
    }
    return item->data;
}

short *List_delete(List *list, long index)
{
    // TODO:
}