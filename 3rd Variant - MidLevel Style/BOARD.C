#include "BOARD.h"

struct board *init_boards(char first_board_name[]) {
    // this is for init user->myboards
    struct board* my_boards = (struct board *) calloc(1, sizeof(struct board));
    sprintf(my_boards->name, "%s", first_board_name);
    my_boards->next = NULL;
    my_boards->my_lists = NULL;
    my_boards->lists_count = 0;
    return my_boards;
}

int add_board(struct board *my_boards, char name[]) {
    // Because if the my_boards is empty, it needs to update its address
    struct board* b = my_boards;
    struct board* new_board = (struct board*) calloc(1, sizeof(struct user));
    sprintf(new_board->name, "%s", name);
    new_board->next = NULL;
    new_board->my_lists = NULL; // New board has no lists.
    new_board->lists_count = 0;
    new_board->next = NULL;
    int board_number = 1;
    while(b->next != NULL) {
        b = b->next;
        board_number++;
    }
    b->next = new_board;

    return board_number;
}

struct board *get_board(struct board *my_boards, int board_number) {
    struct board* board = my_boards;
    int i;
    if(board_number <= 0)
        return NULL;
    for(i = 1; board != NULL && i < board_number; i++)
        board = board->next;
    return board;
}

void show_boards(struct board *my_boards) {
    struct board* b = my_boards;
    int count = 0;
    while(b != NULL) {
        count++;
        printf("%d: %s\n", count, b->name);
        b = b->next;
    }
    printf("Total count: %d\n", count);
}

int remove_board(struct user *user, struct board *board) {
    struct board *previous_board = user->my_boards;
    struct list *list, *temp;
    if(board == NULL)
        return errno_board_not_selected;

    if(board == user->my_boards) { // If its the first item, update my_lists pointer
        user->my_boards = user->my_boards->next;
    } else {
        while(previous_board != NULL &&  previous_board != board && previous_board->next != board)
            previous_board = previous_board->next;
        if(previous_board == NULL)
            return errno_item_not_found;
        previous_board->next = board->next; // Remove board from Link list
    }
    list = board->my_lists; // remove board lists too.
    while(list != NULL) {
        temp = list->next;
        remove_list(board, list);
        list = temp;
    }
    free(board);
    return 1;
}

struct list *init_lists(char first_list_name[]) {
    // this is for init user->mylists
    struct list* my_lists = (struct list *) calloc(1, sizeof(struct list));
    sprintf(my_lists->name, "%s", first_list_name);
    my_lists->next = NULL;
    my_lists->my_tasks = NULL; // New list has no tasks
    my_lists->tasks_count = 0;
    return my_lists;
}

int add_list(struct list *my_lists, char name[]) {
    // Because if the my_lists is empty, it needs to update its address
    struct list* lst = my_lists;
    struct list* new_list = (struct list*) calloc(1, sizeof(struct user));
    sprintf(new_list->name, "%s", name);
    new_list->tasks_count = 0;
    new_list->next = NULL;
    new_list->my_tasks = NULL; // New list has no tasks

    int list_number = 1;
    while(lst->next != NULL) {
        lst = lst->next;
        list_number++;
    }
    lst->next = new_list;
    return list_number;
}

struct list *get_list(struct list *my_lists, int list_number) {
    // Get the list by list_number
    // Go next in link list list_number-1 times
    struct list* list = my_lists;
    int i;
    if(list_number <= 0)
        return NULL;
    for(i = 1; list != NULL && i < list_number; i++)
        list = list->next;
    return list;
}

void show_lists(struct list *my_lists) {
    struct list* lst = my_lists;
    int count = 0;
    while(lst != NULL) {
        count++;
        printf("%d: %s\n", count, lst->name);
        lst = lst->next;
    }
    printf("Total count: %d\n", count);
}

int remove_list(struct board *board, struct list *list) {
    struct list *previous_list = board->my_lists;
    struct task *task, *temp;
    if(list == NULL)
        return errno_list_not_selected;
    if(list == board->my_lists) {
        board->my_lists = board->my_lists->next; // update my_lists pointer because the first item is being freed
    } else {
        while(previous_list != NULL && previous_list != list && previous_list->next != list)
            previous_list = previous_list->next;
        if(previous_list == NULL)
            return errno_item_not_found;
        previous_list->next = list->next; // Remove list from Link list

    }
    // Remove list tasks:
    task = list->my_tasks;
    while(task != NULL) {
        temp = task; // Removing task removes task->next pointer too!
        task = task->next;
        free(temp); // Remove its task one by one
    }
    free(list);
    return 1;
}