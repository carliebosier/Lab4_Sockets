#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Allocate a new list
list_t* list_alloc() {
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    list->length = 0;
    return list;
}

// Free the list
void list_free(list_t *list) {
    node_t *cur = list->head;
    while (cur) {
        node_t *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(list);
}

// Add to front
void list_add_to_front(list_t *list, int val) {
    node_t *newNode = malloc(sizeof(node_t));
    newNode->val = val;
    newNode->next = list->head;
    list->head = newNode;
    list->length++;
}

// Add to back
void list_add_to_back(list_t *list, int val) {
    node_t *newNode = malloc(sizeof(node_t));
    newNode->val = val;
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
    } else {
        node_t *cur = list->head;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }
    list->length++;
}

// Add at index
void list_add_at_index(list_t *list, int val, int idx) {
    if (idx <= 0) {
        list_add_to_front(list, val);
        return;
    }
    if (idx >= list->length) {
        list_add_to_back(list, val);
        return;
    }

    node_t *cur = list->head;
    for (int i = 0; i < idx - 1; i++) cur = cur->next;

    node_t *newNode = malloc(sizeof(node_t));
    newNode->val = val;
    newNode->next = cur->next;
    cur->next = newNode;
    list->length++;
}

// Remove from front
int list_remove_from_front(list_t *list) {
    if (!list->head) return -1;
    node_t *tmp = list->head;
    int val = tmp->val;
    list->head = tmp->next;
    free(tmp);
    list->length--;
    return val;
}

// Remove from back
int list_remove_from_back(list_t *list) {
    if (!list->head) return -1;
    if (!list->head->next) return list_remove_from_front(list);

    node_t *cur = list->head;
    while (cur->next->next) cur = cur->next;

    int val = cur->next->val;
    free(cur->next);
    cur->next = NULL;
    list->length--;
    return val;
}

// Remove at index
int list_remove_at_index(list_t *list, int idx) {
    if (idx <= 0) return list_remove_from_front(list);
    if (idx >= list->length - 1) return list_remove_from_back(list);

    node_t *cur = list->head;
    for (int i = 0; i < idx - 1; i++) cur = cur->next;

    int val = cur->next->val;
    node_t *tmp = cur->next;
    cur->next = tmp->next;
    free(tmp);
    list->length--;
    return val;
}

// Get element at index
int list_get_elem_at(list_t *list, int idx) {
    if (idx < 0 || idx >= list->length) return -1;
    node_t *cur = list->head;
    for (int i = 0; i < idx; i++) cur = cur->next;
    return cur->val;
}

// Get list length
int list_length(list_t *list) {
    return list->length;
}

// Convert list to string
char* listToString(list_t *list) {
    char *str = malloc(list->length * 12 + 2); // 12 chars per int + comma
    str[0] = '\0';
    node_t *cur = list->head;
    while (cur) {
        char buf[12];
        sprintf(buf, "%d", cur->val);
        strcat(str, buf);
        if (cur->next) strcat(str, " -> ");
        cur = cur->next;
    }
    return str;
}
