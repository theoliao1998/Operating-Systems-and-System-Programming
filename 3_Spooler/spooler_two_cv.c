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

#ifndef PART4
#define PART4
#endif

#include "spooler_shared.h"
#include "spooler_two_cv.h"

/*
 * Function to initialize any fields added in PART 4. You SHOULD NOT
 * initialize anything you added in PART 1, PART 2, or PART 3.
 */
void spool_init_cv2(spool_t* sp) {
  while(pthread_cond_init(&sp->con_cv, NULL)!=0);
  while(pthread_cond_init(&sp->pro_cv, NULL)!=0);
  sp->shutdown = false;
}

/*
 * Function used to implement insertion in a thread safe manner. It
 * should use spool_insert_unchecked once it confirms there is room
 * in the spooler. This function should always insert the item.
 */
void spool_insert_cv2(spool_t* sp, char* item) {
  pthread_mutex_lock(&sp->buf_lock);
  while (spool_full(sp)) {
    pthread_cond_signal(&sp->con_cv);
    pthread_cond_wait(&sp->pro_cv,&sp->buf_lock);
  }
  spool_insert_unchecked(sp, item);
  pthread_cond_signal(&sp->con_cv);
  pthread_mutex_unlock(&sp->buf_lock);
}

/*
 * Function used to implement removal in a thread safe manner. It
 * should use spool_remove_unchecked once it confirms there is data
 * in the spooler. This function should only return NULL if a shutdown
 * has been issued and otherwise must return an element.
 */
char* spool_remove_cv2(spool_t* sp) {
  pthread_mutex_lock(&sp->buf_lock);
  while (spool_empty(sp) && (!sp->shutdown)) {
    pthread_cond_signal(&sp->pro_cv);
    pthread_cond_wait(&sp->con_cv,&sp->buf_lock);
  }
  char* item = spool_remove_unchecked(sp);
  pthread_cond_signal(&sp->pro_cv);
  pthread_mutex_unlock(&sp->buf_lock);
  return item;
}

