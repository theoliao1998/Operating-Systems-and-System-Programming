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

#ifndef PART2
#define PART2
#endif

#include "spooler.h"
#include "spooler_shared.h"
#include "spooler_safe.h"

/*
 * Function that initializes any fields added in PART 2. You SHOULD NOT
 * initialize anything that you added in PART 1.
 */
void spool_init_safe(spool_t* sp) {
  while(pthread_mutex_init(&sp->buf_lock, NULL)!=0);
}

/*
 * Function used to implement insertion in a thread safe manner. It should
 * use spool_insert_unchecked once it confirms there is room in the spooler.
 * This function should never return without inserting as we should wait
 * until there is room to insert in the queue.
 */
void spool_insert_safe(spool_t* sp, char* item) {
  while(true){
    pthread_mutex_lock(&sp->buf_lock);
    if(spool_full(sp))
      pthread_mutex_unlock(&sp->buf_lock);
    else{
      spool_insert_unchecked(sp, item);
      pthread_mutex_unlock(&sp->buf_lock);
      return;
    }
  }
}

/*
 * Function used to implement removal in a thread safe manner. It should
 * use spool_remove_unchecked once it confirms there is data in the spooler.
 * This function should never return NULL as we should wait until there is
 * room to insert in the queue.
 */
char* spool_remove_safe(spool_t* sp) {
  while(true){
    pthread_mutex_lock(&sp->buf_lock);
    if(spool_empty(sp))
      pthread_mutex_unlock(&sp->buf_lock);
    else{
      char* item = spool_remove_unchecked (sp);
      pthread_mutex_unlock(&sp->buf_lock);
      return item;
    }
  }
}

