#ifndef PRODUCER_H
#define PRODUCER_H

#include "spooler.h"

/*
 *  Each producer inserts its sequence of streams.
 *  Where in real life this would be something that runs indefinitely,
 *  until it is shut down by external action, in this simple driver it is
 *  built in
 *
 */

void producer(spool_t* sp, int prod_id, int nstrs);

#endif

