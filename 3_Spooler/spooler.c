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

#include <stdlib.h>
#include "spooler.h"
#include "spooler_shared.h"
#if defined PART2 || defined PART3 || defined PART4
#include "spooler_safe.h"
#else
#include "spooler_unsafe.h"
#endif
#if defined PART3 || defined PART4
#include "spooler_one_cv.h"
#endif
#if defined PART4
#include "spooler_two_cv.h"
#endif


void spool_init(spool_t* sp, size_t size) {
  spool_init_shared(sp, size);
  #if defined PART2 || defined PART3 || defined PART4
  spool_init_safe(sp);
  #endif
  #if defined PART3 || defined PART4
  spool_init_cv1(sp);
  #endif
  #if defined PART4
  spool_init_cv2(sp);
  #endif
}

void spool_insert(spool_t* sp, char* item) {
  #if defined PART4
  spool_insert_cv2(sp, item);
  #elif defined PART3
  spool_insert_cv1(sp, item);
  #elif defined PART2
  spool_insert_safe(sp, item);
  #else
  spool_insert_unsafe(sp, item);
  #endif
}

char* spool_remove(spool_t* sp) {
  #if defined PART4
  char* item = spool_remove_cv2(sp);
  #elif defined PART3
  char* item = spool_remove_cv1(sp);
  #elif defined PART2
  char* item = spool_remove_safe(sp);
  #else
  char* item = spool_remove_unsafe(sp);
  #endif
  return item;
}

void spool_shutdown(spool_t* sp) {
  #if defined PART3 || defined PART4
  spool_shutdown_efficient (sp);
  #endif
}

