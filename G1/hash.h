#ifndef __HASH_H__
#define __HASH_H__

/* Allow for a hashmap to be created mapping words to a set of potential
anograms.
*/

#include <stdbool.h>
#include "list.h"

typedef struct {
  Node** buckets;
} Hash;

int hash(char* word);

Hash* hash_create();
void hash_free(Hash* h);
bool hash_contains(Hash* h, char* word);
Node* hash_find(Hash* h, char* word);
void hash_insert(Hash* h, char* word);
void hash_print(Hash* h);

#endif // __LIST_H__
