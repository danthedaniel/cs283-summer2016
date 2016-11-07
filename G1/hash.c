#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "hash.h"

#define NUM_BUCKETS 1024

int hash(char* word)
{
  int accumulator = 0;

  for (unsigned int i = 0; i < strlen(word); ++i)
  {
    char c = toupper(word[i]);
    accumulator += (int) c;
  }

  return accumulator % NUM_BUCKETS;
}

Hash* hash_create()
{
  Hash* h = (Hash*) malloc(sizeof(Hash));
  h->buckets = (Node**) malloc(NUM_BUCKETS * sizeof(Node*));

  // Zero out all of the addresses in the malloc'd space
  for (int i = 0; i < NUM_BUCKETS; ++i)
    h->buckets[i] = NULL;

    return h;
}

void hash_free(Hash* h)
{
  for (int i = 0; i < NUM_BUCKETS; ++i)
  {
    if (h->buckets[i] != NULL)
      list_free(h->buckets[i]);
  }

  free(h->buckets);
  free(h);
}

void hash_insert(Hash* h, char* word)
{
  Node* bucket = h->buckets[hash(word)];

  if (bucket == NULL)
    h->buckets[hash(word)] = list_create(word);
  else
  {
    if (!list_contains(bucket, word))
    {
      list_append(bucket, word);
    }
  }
}

Node* hash_find(Hash *h, char* word)
{
  return h->buckets[hash(word)];
}

bool hash_contains(Hash *h, char* word)
{
  Node* bucket = h->buckets[hash(word)];

  if (bucket == NULL)
    return false;
  else
    return list_contains(bucket, word);
}

void hash_print(Hash* h)
{
  for (int i = 0; i < NUM_BUCKETS; ++i)
  {
    if (h->buckets[i] != NULL)
    {
      printf("BUCKET[%d]\n", i);
      list_print(h->buckets[i]);
    }
  }
}
