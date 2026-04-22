//
// Created by Jesse on 4/22/2026.
//

#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setNode(struct node* node, char* key, char* value) {
    node->key = key;
    node->value = value;
    node->next = NULL;
};
void initializeHashMap(struct hashMap* mp) {
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node**)calloc(mp->capacity, sizeof(struct node*));
}
int hashFunction(const struct hashMap* mp, const char* key) {
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {

        // sum = sum + (ascii value of char * (primeNumber ^ x)) where x = 1, 2, 3....n
        sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) % mp->capacity;

        // factor = factor * prime
        // number...(prime number) ^ x
        factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
    }

    const int bucketIndex = sum;
    return bucketIndex;
}
void insert(const struct hashMap* mp, char* key, char* value) {

    // Getting bucket index for the given
    // key - value pair
    const int bucketIndex = hashFunction(mp, key);
    struct node* newNode = (struct node*)malloc(sizeof(struct node));

    // Setting value of node
    setNode(newNode, key, value);

    // Bucket index is empty...no collision
    if (mp->arr[bucketIndex] == NULL) {
        mp->arr[bucketIndex] = newNode;
    }

    // Collision
    else {

        // Adding newNode at the head of
        // linked list which is present
        // at bucket index insertion at
        // head in linked list
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
}
void delete (const struct hashMap* mp, const char* key) {

    // Getting bucket index for the
    // given key
    const int bucketIndex = hashFunction(mp, key);

    struct node* prevNode = NULL;

    // Points to the head of
    // linked list present at
    // bucket index
    struct node* currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {

        // Key is matched at delete this
        // node from linked list
        if (strcmp(key, currNode->key) == 0) {

            // Head node
            // deletion
            if (currNode == mp->arr[bucketIndex]) {
                mp->arr[bucketIndex] = currNode->next;
            }

            // Last node or middle node
            else {
                if (prevNode != NULL) {
                    prevNode->next = currNode->next;
                }
            }
            free(currNode);
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
}
char* search(const struct hashMap* mp, const char* key) {

    // Getting the bucket index
    // for the given key
    const int bucketIndex = hashFunction(mp, key);

    // Head of the linked list
    // present at bucket index
    const struct node* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {

        // Key is found in the hashMap
        if (strcmp(bucketHead->key, key) == 0) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }

    // If no key found in the hashMap
    // equal to the given key
    //auto errorMssg = (char*)malloc(sizeof(char) * 25);
    char* errorMssg = "Oops! No data found.\n";
    return errorMssg;
}
void printHashMap(const struct hashMap* mp) {
    for (int i = 0; i < mp->capacity; i++) {
        struct node* currNode = mp->arr[i];
        while (currNode != NULL) {
            printf("%s\n", currNode->key);
            currNode = currNode->next;
        }
    }
}
void freeHashMap(const struct hashMap* mp) {
    for (int i = 0; i < mp->capacity; i++) {
        struct node* currNode = mp->arr[i];
        while (currNode != NULL) {
            struct node* nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
    }
    free(mp->arr);
}