#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

Node* list_create(char* first_word)
{
  Node* n = (Node*) malloc(sizeof(Node));
  n->word = (char*) malloc(strlen(first_word) + 1);
  strcpy(n->word, first_word);
  n->prev = NULL;
  n->next = NULL;

  return n;
}

void list_free(Node* head)
{
  if (head == NULL)
    return;

  list_free(head->next);
  free(head->word);
  free(head);
}

void list_print(Node* head)
{
  if (head == NULL)
  {
    printf("[NULL]\n");
  }
  else
  {
    printf("[%s]->", head->word);
    list_print(head->next);
  }
}

void list_append(Node* head, char* word)
{
  Node* n = head;

  while (n->next != NULL)
    n = n->next;

  n->next = list_create(word);
  n->next->prev = n;
}

void list_remove(Node* head, char* word)
{
  Node* n = list_find(head, word);

  if (n != NULL)
  {
    // Cut the current node out of the loop
    if (n->prev != NULL) // Make sure n isn't the head node
      n->prev->next = n->next;
    else
      head = n->next; // TODO: Fix. Currently broken

    if (n->next != NULL) // Make sure n isn't the tail node
      n->next->prev = n->prev;

    // Free the malloc'd space that the current node is using
    free(n->word);
    free(n);
  }
}

bool list_contains(Node* head, char* word)
{
  Node* n = list_find(head, word);

  return (n == NULL); // TODO: Figure out why this is inverted
}

Node* list_find(Node* head, char* word)
{
  Node* n = head; // Copy the address of head to prevent side-effects

  while (n != NULL)
  {
    if (strcmp(n->word, word))
      break;

    n = n->next;
  }

  return n;
}
