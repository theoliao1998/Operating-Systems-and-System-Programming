

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "word_count.h"
//#include "word_helpers.h"

/*
 * main - handle command line and file handles.
 */
int main(int argc, char *argv[]) {
  /* Create the empty data structure. */
  word_count_list_t word_counts;
  init_words(&word_counts);
  add_word(&word_counts,"word_count.h");
  add_word_with_count(&word_counts,"word_count.h",3);
  add_word_with_count(&word_counts,"xxx",8);

  printf("%ld\n",len_words(&word_counts));
  fprint_words(&word_counts,stdout);
  return 0;
}
