//
// Created by Jesse on 4/15/2026.
//

#ifndef RESP_PARSER_RESP_PARSER_H
#define RESP_PARSER_RESP_PARSER_H


typedef struct [[maybe_unused]] RESP_element {
    char* element;
    int size;
    struct RESP_element* next;
} RESP_element;
typedef struct [[maybe_unused]] RESP_list {
    RESP_element* head;
    RESP_element* tail;
    int size;
}RESP_list;
RESP_element* parse_element(const char*);
RESP_list* parse_list(const char*);
void enqueue(RESP_list*, RESP_element*);
void dequeue(RESP_list*);
RESP_element* peek(const RESP_list*);
void print_list(const RESP_list*);
void free_list(RESP_list*);

#endif //RESP_PARSER_RESP_PARSER_H
