#ifndef CONSUMER_H
#define CONSUMER_H

#include "spooler.h"

/*
 *
 *  Consumers remove elements from the spooler buffer.
 *
 *  NULL indicates that all production has stopped, buffer has drained
 *  and consumer should terminate
 *
 */
void consumer(spool_t* sp, int con_id);

#endif

