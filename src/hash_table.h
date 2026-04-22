#ifndef HASHTABLE_HASH_TABLE_H
#define HASHTABLE_HASH_TABLE_H
// Linked List node
struct node {

    // key is string
    char* key;

    // value is also string
    char* value;
    struct node* next;
};
struct hashMap {

    // Current number of elements in hashMap
    // and capacity of hashMap
    int numOfElements, capacity;

    // hold base address array of linked list
    struct node** arr;
};

void setNode(struct node*, char*, char*);
void initializeHashMap(struct hashMap*);
int hashFunction(const struct hashMap*, const char*);
void insert(const struct hashMap*, char*, char*);
void delete (const struct hashMap*, const char*);
char* search(const struct hashMap*, const char*);
void freeHashMap(const struct hashMap*);

#endif //HASHTABLE_HASH_TABLE_H
