//
// Created by Jesse on 4/15/2026.
//
#include "RESP_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RESP_element* parse_element(const char* input) {
    RESP_element* res = malloc(sizeof *res);
    if (res == NULL) return nullptr;

    if (input == NULL) {
        printf("error: input null\n");
        free(res);
        return nullptr;
    } else if (input[0] != '$') {
        printf("error: input not a valid input\n");
        free(res);
        return nullptr;
    } else {
        char *i;
        const long count = strtol(input + 1, &i, 10);

        res->element = malloc((size_t)count + 1);
        if (res->element == NULL) {
            free(res);
            return nullptr;
        }

        strncpy(res->element, i + 2, count);
        res->element[count] = '\0';
        res->size = count;
        res->next = nullptr;                                                                                                                                        ;
    }

    return res;
}
RESP_list* parse_list(const char* input) {
    RESP_list *res = malloc(sizeof(RESP_list));
    if (res == NULL) return nullptr;

    res->head = nullptr;
    res->tail = nullptr;
    res->size = 0;
    if (input == NULL) {
        printf("error: input null\n");
        free(res);
        return nullptr;
    }
    else if (input[0] != '*') {
        printf("error: input not a valid input\n");
        free(res);
        return nullptr;
    }
    else {
        char *i;
        int ctr = strtol(input + 1, &i, 10);
        i = i + 2;
        int temp_ctr = 0;
        do {
            RESP_element *temp = parse_element(i + temp_ctr);
            enqueue(res, temp);
            temp_ctr += temp->size + 6;
            ctr--;

        }while (ctr != 0);
    }

    return res;
}
void enqueue(RESP_list *list, RESP_element *element) {
    if (list->head == NULL) {
        list->head = element;
        list->tail = element;
        list->size++;
    }
    else {
        list->tail->next = element;
        list->tail = element;
    }
}
void dequeue(RESP_list *list) {
    if (list->head == NULL) {
        printf("error: list is empty\n");
    }
    else {
        RESP_element *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}
RESP_element* peek(const RESP_list *list) {
    if (list->head == NULL) return nullptr;
    return list->head;
}
void print_list(const RESP_list *list) {
    RESP_element *temp = list->head;
    while (temp != NULL) {
        printf("%s\n", temp->element);
        temp = temp->next;
    }
}
void free_list(RESP_list *list) {
    while (list->head != NULL) {
        RESP_element *tmp = list->head;
        tmp = tmp->next;
        free(tmp);
    }
    free(list);
}