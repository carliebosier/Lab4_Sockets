#ifndef LIST_H
#define LIST_H

typedef struct node {
    int val;
    struct node *next;
} node_t;

typedef struct {
    node_t *head;
    int length;
} list_t;

// Allocation and freeing
list_t* list_alloc();
void list_free(list_t *list);

// Adding elements
void list_add_to_front(list_t *list, int val);
void list_add_to_back(list_t *list, int val);
void list_add_at_index(list_t *list, int val, int idx);

// Removing elements
int list_remove_from_front(list_t *list);
int list_remove_from_back(list_t *list);
int list_remove_at_index(list_t *list, int idx);

// Accessing elements
int list_get_elem_at(list_t *list, int idx);
int list_length(list_t *list);

// Utilities
char* listToString(list_t *list);

#endif
