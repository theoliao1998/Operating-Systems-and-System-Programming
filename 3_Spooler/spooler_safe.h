#ifndef SPOOLER_SAFE_H
#define SPOOLER_SAFE_H

#include "spooler.h"

/*
 * Function that initializes any fields added in PART 2. You SHOULD NOT
 * initialize anything that you added in PART 1.
 */
void spool_init_safe(spool_t* sp);

/*
 * Function used to implement insertion in a thread safe manner. It should
 * use spool_insert_unchecked once it confirms there is room in the spooler.
 * This function should never return without inserting as we should wait
 * until there is room to insert in the queue.
 */
void spool_insert_safe(spool_t* sp, char* item);

/*
 * Function used to implement removal in a thread safe manner. It should
 * use spool_remove_unchecked once it confirms there is data in the spooler.
 * This function should never return NULL as we should wait until there is
 * room to insert in the queue.
 */
char* spool_remove_safe(spool_t* sp);

#endif

