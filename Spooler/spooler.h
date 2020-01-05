/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SPOOLER_H
#define SPOOLER_H

#include <pthread.h>
#include <stdbool.h>


/*
 * Structure for your spooler. You will need to add different elements based upon if you
 * are using the assumptions in PART1, PART2, PART3, or PART4.
*/
typedef struct spool {
  // PART 1 GOES HERE
  size_t write_index;
  size_t read_index;
  size_t size;
  size_t capacity;
  char** elements;
  #if defined PART2 || defined PART3 || defined PART4
  // PART 2 GOES HERE
  pthread_mutex_t buf_lock;
  #endif
  #if defined PART3 || defined PART4
  // PART 3 GOES HERE
  pthread_cond_t con_cv;
  bool shutdown;
  #endif
  #if defined PART4
  // PART 4 GOES HERE
  pthread_cond_t pro_cv;
  #endif

} spool_t;

/*
 * GENERIC SPOOLER API. You will not need to edit these functions as they call various functions
 * in the spooler_unsafe, spooler_safe, and spooler_efficient files to enable separate
 * executables for parts 1, 2, 3 and 4.
 */

/*
 * Initialization function to create the spooler
 */
void spool_init(spool_t* sp, size_t size);

/*
 * Function to insert an element into the buffer when room
 * exists. For part 1 it acceptable to have this function
 * return if the buffer is currently full.
 * For parts 2, 3, and 4 this function should always wait for
 * space to develop in the buffer. The insert function DOES NOT
 * need to worry about any shutdown conditions.
 */
void spool_insert(spool_t* sp, char* item);

/*
 * Function to remove an element from the buffer when one
 * exists. For part 1 it acceptable to have this function
 * return NULL if the buffer is currently empty.
 * For parts 2 and 3 this function should always return
 * a valid element unless we are in a shutdown condition.
 */
char* spool_remove(spool_t* sp);

/*
 * Function used to signal a shutdown. It does nothing unless
 * PART3 is implemented, in which case it just calls
 * spool_shutdown_efficient.
 */
void spool_shutdown(spool_t* sp);
#endif

