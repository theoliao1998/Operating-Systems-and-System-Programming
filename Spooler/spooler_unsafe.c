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

#include <stddef.h>
#include "spooler_shared.h"
#include "spooler_unsafe.h"

/*
 * Function that inserts item in sp. It should call spool_insert_unchecked
 * to do the actual insertion but should first check that there is room in
 * the spooler. It the spooler is full it should not insert the element and
 * just return.
 */
void spool_insert_unsafe(spool_t* sp, char* item) {
  if(spool_full(sp))
    return;
  spool_insert_unchecked (sp, item);
}

/*
 * Function that removes the least recently added item in sp.
 * It should call spool_remove_unchecked to do the actual removal,
 * but should first check that the spooler is not empty.
 * It returns the item removed or NULL if the buffer is empty.
 */
char* spool_remove_unsafe(spool_t* sp) {
  if(spool_empty(sp))
    return NULL;
  return spool_remove_unchecked (sp);
}

