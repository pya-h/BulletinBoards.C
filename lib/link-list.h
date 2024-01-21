#ifndef LINK_LIST_H
#define LINK_LIST_H
#include "base.h"
    // actually this is about link list that is defined here and used all over the app
    // not the TaskLists

    // struct that is the base of our link list
    // using void * data-type, we can use this link list for every type in our app.
    typedef struct ListItem
    {
        void *data;
        struct ListItem *next, *prev;
    } ListItem;

    // we define an outer struct, that will actually contain a link list;
    // this way we enhance the performance of the link list operations,
    // by setting the first and last item of the list (instead of finding the last item every time), and some fields [length till now]
    typedef struct List
    {
        ListItem *first, *last;
        char error[MAX_RESPONSE_LENGTH];
        long length;
        // next two fields is like a caching mechanism; it remembers the last item index that is accessed
        // when getting another item of a list, if the target index is close to this lastAccessIndex, we use that to process list
        // useful when using a List_at in a for loop
        long lastAccessedIndex;
        ListItem *lastAccessedItem;
    } List;

    void *resetList(List *list);
    List *newList();
    ListItem *newListItem(void *data);
    void ListItem_dump(ListItem *);
    ListItem *List_add(List *list, void *data);
    void *List_at(List *list, long index);
    short List_delete(List *list, long index);
    string List_getError(List *list);
    // void **List_toArray(List *list, long index);
#endif //LINK_LIST_H