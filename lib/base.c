#include "base.h"
#include <string.h>

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

void getLine(char *dest, char *inputMessage)
{
    // sometimes the app may has been executed a scanf before gets
    // the ['\n'] next line character may remain in input stream
    // since gets will store next line character too, ['\n'] is a valid string input for gets and
    // it results in gets input skip!
    dest[0] = '\0';
    while (!dest[0])
    {
        printf("\n%s", inputMessage);
        gets(dest);
        removeNextlineCharacter(dest); // remove next line from end of string
    }
}

void removeNextlineCharacter(char *str)
{
    // remove the next line character ['\n'] at the end of string (if exists)
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0'; // Replace '\n' with '\0'
    }
}

char *encodeString(char *input)
{

    // create a string exactly as the size needed
    char *encoded = (char *)calloc(ENCODED_STRING_LENGTH(input), sizeof(char));
    int current = 0;
    // add salt to start of string
    for (char *salt = ENCODE_SALT; *salt; encoded[current++] = *(salt++))
        ;

    // encode password by performing not on each char
    for (; *input; encoded[current++] = ~*(input++))
        ;

    // append salt to the end of string too
    for (char *salt = ENCODE_SALT; *salt; encoded[current++] = ~*(salt++))
        ;
    encoded[current] = '\0';
    return encoded;
}

ListItem *newListItem(void *data)
{
    ListItem *item = (ListItem *)calloc(1, sizeof(ListItem));
    item->data = data;
    item->next = item->prev = NULL;
    return item;
}

List *newList()
{
    List *list = (List *)calloc(1, sizeof(List));
    resetList(list);
    return list;
}

void *resetList(List *list)
{
    list->first = list->last = NULL;
    list->length = 0;
    list->lastAccessedIndex = -1;
    list->lastAccessedItem = NULL;
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
    short found = 0;

    if (index < 0) // negative index means start from end
        index += list->length;

    // these 3 conditions may seem redundant; but when accessing wrong item, or first/last item, it prevents unnessary calculations and condition checks.
    if (index < 0 || // means the negative index is way greater than the actual length
        index >= list->length)
        return NULL; // Not found
    if (!index)
        return list->first->data;
    if (index == list->length - 1)
        return list->last->data;

    // just in a lucky case that a item is access again rapidly, or is near previous accessed item, use this section
    if (list->lastAccessedIndex >= 0 && list->lastAccessedItem // if there was a cash index, use it if it helps to find item fast
        && list->lastAccessedIndex < list->length) // this condition is just for double checking that lastItemIndex value is correct
    {
        if (index == list->lastAccessedIndex)
            return list->lastAccessedItem->data;
        const long indexDistanceFromLastAccessdIndex = DIFF(index, list->lastAccessedIndex); // just for preventing calculating this twice [cause next if statement uses this value twice and has two conditions]
        if (indexDistanceFromLastAccessdIndex < DIFF(0, index) && indexDistanceFromLastAccessdIndex < DIFF(list->length - 1, index))
        {
            if (list->lastAccessedIndex <= index) // process the nodes, by going to next pointer of each node, until it reaches to the desired item
                for (item = list->lastAccessedItem, i = list->lastAccessedIndex; i < index; i++, item = item->next)
                    ;
            else // process the nodes, by going to next pointer of each node, until it reaches to the desired item
                for (item = list->lastAccessedItem, i = list->lastAccessedIndex; i > index; i--, item = item->prev)
                    ;

            found = 1;
        }
    }
    // normal approach
    if (!found)
        if (index < (float)list->length / 2.0)
            for (item = list->first, i = 0; i < index; i++, item = item->next)
                ; // process the nodes, by going to next pointer of each node, until it reaches to the desired item
        else
            for (item = list->last, i = list->length - 1; i > index; i--, item = item->prev)
                ; // process the nodes, by going to next pointer of each node, until it reaches to the desired item

    list->lastAccessedItem = item;
    list->lastAccessedIndex = index;

    return item->data;
}

void ListItem_dump(ListItem *trash)
{
    trash->next = trash->prev = NULL;
    free(trash);
}

short *List_delete(List *list, long index)
{
    // TODO:
    if (!List_at(list, index)) // updates .lastAccessedItem and .lastAccessedIdex
        return 0;              // item was not found so dont do anything.
    ListItem *trash = list->lastAccessedItem;
    list->lastAccessedItem = NULL;
    list->lastAccessedIndex = -1; // last accessed item is deleted and no longer in the list; so reset these two values related to it

    if (trash == list->first)
    { // or
        if (list->length == 1)
        {
            // if the list has only one element:
            resetList(list);
            ListItem_dump(trash);
            return 1;
        }
        
        list->first = list->first->next;
        list->first->prev = NULL;
    }
    else if (trash == list->last)
    {
        list->last = list->last->prev;
        list->last->next = NULL;
    }
    else
    {
        // if the item is in the middle
        trash->prev->next = trash->next;
        trash->next->prev = trash->prev;
    }
    ListItem_dump(trash);
    list->length--;
    return 1; // item found and deleted successfully
}