#ifndef SPOOLER_SHARED_H
#define SPOOLER_SHARED_H

#include <stdbool.h>
#include "spooler.h"

/*
 * Function that initializes sp with a buffer of the provided size.
 */
void spool_init_shared(spool_t* sp, size_t size);

/*
 * Function that returns if the buffer is empty.
 */
bool spool_empty(spool_t* sp);

/*
 * Function that returns if the buffer is full.
 */
bool spool_full(spool_t* sp);

/*
 * Function that inserts item into sp. It makes no check that there is
 * room in the buffer because it will be reused in different implementations
 * that will provide this check. This does not require making a deep copy of
 * the item.
 */
void spool_insert_unchecked(spool_t* sp, char* item);

/*
 * Function that removes the least recently added item from sp.
 * It makes no check that there is data in the buffer because it
 * will be reused in different implementations that will provide
 * this check.
 */
char* spool_remove_unchecked(spool_t* sp);

/*
 * Function that returns the buffer allocated for data in sp.
 */
char** reveal_buffer(spool_t* sp);
#endif

