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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "spooler.h"
#include "spooler_shared.h"
#include "spooler_unsafe.h"

const int buffer_size = 4;

/*
 * Test that checks spool_empty works properly.
 */
void test_empty() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  assert (spool_empty (sp));
  spool_insert (sp, "test");
  assert (!spool_empty (sp));
  printf ("Empty test passed\n");
  fflush (stdout);
}

/*
 * Test that checks spool_full works properly.
 */
void test_full() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  assert (!spool_full (sp));
  spool_insert (sp, "test1");
  assert (!spool_full (sp));
  spool_insert (sp, "test2");
  assert (!spool_full (sp));
  spool_insert (sp, "test3");
  assert (!spool_full (sp));
  spool_insert (sp, "test4");
  assert (spool_full (sp));
  printf ("Full test passed\n");
  fflush (stdout);
}

/*
 * Test that checks remove returns what is inserted.
 */
void test_insert_remove() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  spool_insert (sp, "test");
  assert (!strcmp(spool_remove (sp), "test"));
  spool_insert (sp, "test1");
  spool_insert (sp, "test2");
  spool_insert (sp, "test3");
  assert (!strcmp(spool_remove (sp), "test1"));
  assert (!strcmp(spool_remove (sp), "test2"));
  assert (!strcmp(spool_remove (sp), "test3"));
  printf ("Basic insert and remove test passed.\n");
  fflush (stdout);
}

/*
 * Test that checks removing from an empty buffer is rejected in the unsafe case.
 */
void test_remove_empty() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  assert (!spool_remove (sp));
  printf ("Empty remove test passed.\n");
  fflush (stdout);
}

/*
 * Test that checks inserting to a full buffer is rejected in the unsafe case.
 */
void test_insert_full() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  spool_insert (sp, "test1");
  spool_insert (sp, "test2");
  spool_insert (sp, "test3");
  spool_insert (sp, "test4");
  spool_insert (sp, "test5");
  assert (!strcmp(spool_remove (sp), "test1"));
  assert (!strcmp(spool_remove (sp), "test2"));
  assert (!strcmp(spool_remove (sp), "test3"));
  assert (!strcmp(spool_remove (sp), "test4"));
  assert (!spool_remove (sp));
  printf ("Full insert test passed.\n");
  fflush (stdout);
}

/*
 * Test that checks inserting to the buffer after removing doesn't move memory.
 */
void test_circular() {
  spool_t spooler;
  spool_t* sp = &spooler;
  spool_init (sp, buffer_size);
  char** buffer = reveal_buffer (sp);
  spool_insert (sp, "test1");
  spool_insert (sp, "test2");
  spool_insert (sp, "test3");
  spool_insert (sp, "test4");
  int three_index = -1;
  int four_index  = -1;
  for (int i = 0; i < buffer_size; i++) {
    if (!strcmp ("test3", buffer[i])) {
      three_index = i;
    } else if (!strcmp ("test4", buffer[i])) {
      four_index = i;
    }
  }
  assert (three_index != -1 && four_index != -1);
  assert (!strcmp(spool_remove (sp), "test1"));
  assert (!strcmp(spool_remove (sp), "test2"));
  spool_insert (sp, "test5");
  spool_insert (sp, "test6");
  int five_index = -1;
  int six_index  = -1;
  for (int i = 0; i < buffer_size; i++) {
    if (!strcmp ("test3", buffer[i])) {
      assert (three_index == i);
    } else if (!strcmp ("test4", buffer[i])) {
      assert (four_index == i);
    } else if (!strcmp ("test5", buffer[i])) {
      five_index = i;
    } else if (!strcmp ("test6", buffer[i])) {
      six_index = i;
    }
  }
  assert (five_index != -1 && six_index != -1);
  printf ("Circular test passed.\n");
  fflush (stdout);

}

int main() {
  test_full ();
  test_empty ();
  test_insert_remove ();
  test_remove_empty ();
  test_insert_full ();
  test_circular ();
  printf ("All tests passed.\n");
  fflush (stdout);
}

