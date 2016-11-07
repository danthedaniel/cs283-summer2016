#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
#include "hash.h"

#define MAX_WORD_SIZE 128

void read_into_hash(const char* filepath, Hash* h);
char* sort_word(const char* word);
int compare(const void *a, const void *b);

// Usage: g1 <word> [pivot char] [pivot pos]
int main(int argc, char* argv[])
{
  Hash* my_hash = hash_create();

  if (argc > 1)
  {
    read_into_hash("/usr/share/dict/words", my_hash);

    Node* possibles = hash_find(my_hash, argv[1]);

    char* sorted_input = sort_word(argv[1]);

    while (possibles != NULL)
    {
      char* sorted_dict = sort_word(possibles->word);

      // Only continue if the two words are anagrams, also ignore the found word
      // if it is the same as the input word
      if ((strcmp(sorted_input, sorted_dict) == 0) &&
          (strcmp(argv[1], possibles->word) != 0))
      {
        // Scrabble rules
        if (argc > 3)
        {
          // Convert arg to integer
          char* end;
          unsigned int pos = (unsigned int) strtol(argv[3], &end, 10);

          // Character that must be fixed in permutations
          char fixed_char = argv[2][0];

          // Only print the word if it has the desired character in the desired
          // position.
          if (possibles->word[pos] == fixed_char)
          {
            printf("%s\n", possibles->word);
          }
        } else { // Simple anagrams
          printf("%s\n", possibles->word);
        }
      }

      free(sorted_dict);

      possibles = possibles->next;
    }

    free(sorted_input);
  } else {
    printf("Usage:\n%s WORD [LETTER] [PLACE]\n", argv[0]);
  }

  hash_free(my_hash);

  return 0;
}

void read_into_hash(const char* filepath, Hash* h)
{
  FILE* dict = fopen(filepath, "r");

  if(dict == NULL)
  {
    exit(-1);
  }

  // Read each line of the file, and store it in the hash
  char word[MAX_WORD_SIZE];
  while(fgets(word, sizeof(word), dict) != NULL)
  {
    char stripped_word[MAX_WORD_SIZE];
    unsigned int stripped_index = 0;

    // Remove all non-alphabet characters
    for (unsigned int i = 0; i < strlen(word); ++i)
    {
      if ((word[i] > '@' && word[i] < '[') ||
          (word[i] > '`' && word[i] < '{'))
      {
        stripped_word[stripped_index] = word[i];
        stripped_index++;
      }
    }

    stripped_word[stripped_index] = '\0';

    hash_insert(h, &stripped_word[0]);
  }

  fclose(dict);
}

int compare(const void* a, const void* b)
{
  return *(const char*) a - *(const char*) b;
}

char* sort_word(const char* word)
{
  char* ret = (char*) malloc(MAX_WORD_SIZE * sizeof(char));
  strcpy(ret, word);

  // Convert word to be all capitals
  for (unsigned int i = 0; i < strlen(ret); ++i)
    ret[i] = toupper(ret[i]);

  qsort(ret, strlen(ret), 1, compare);

  return ret;
}
