#ifndef SPOOLER_UNSAFE_H
#define SPOOLER_UNSAFE_H

#include <stdbool.h>
#include "spooler.h"

/*
 * Function that inserts item in sp. It should call spool_insert_unchecked
 * to do the actual insertion but should first check that there is room in
 * the spooler. It the spooler is full it should not insert the element and
 * just return.
 */
void spool_insert_unsafe(spool_t* sp, char* item);

/*
 * Function that removes the least recently added item in sp.
 * It should call spool_remove_unchecked to do the actual removal,
 * but should first check that the spooler is not empty.
 * It returns the item removed or NULL if the buffer is empty.
 */
char* spool_remove_unsafe(spool_t* sp);

#endif

