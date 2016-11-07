#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

typedef struct Node Node;

struct Node {
  char* word;
  struct Node* prev;
  struct Node* next;
};

Node* list_create(char* first_word);
void list_free(Node* head);
void list_print(Node* head);
void list_append(Node* head, char* word);
void list_remove(Node* head, char* word);
bool list_contains(Node* head, char* word);
Node* list_find(Node* head, char* word);

#endif // __LIST_H__
