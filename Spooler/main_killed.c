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
#include <stdio.h>
#include "consumer.h"
#include "producer.h"
#include "spooler.h"


/* Thread work structure */

spool_t string_spooler;   /* The shared buffer */
int nprods;     /* Number of producers */
int ncons;
int nstrs;

typedef struct twork {
  pthread_t thread;   /* Thread descriptor */
  int tid;      /* Local id  */
  enum TType {READER, WRITER} ttype;
} TWork;

void* thread_writer_fun(void* tw) {
  /* Run a producer threads */
  TWork* my_tw = (TWork*)tw;
  producer(&string_spooler, my_tw->tid, nstrs);
  printf("Writer Thread #%d done\n", my_tw->tid);
  pthread_exit(NULL);
}

void* thread_reader_fun(void* tw) {
  /* Run a consumer */
  TWork* my_tw = (TWork*)tw;
  int rdr      = my_tw->tid - nprods;
  printf("Reader %d Thread %d started\n", rdr, my_tw->tid);
  consumer(&string_spooler, rdr);
  printf("Reader %d Thread %d done\n", rdr, my_tw->tid);
  pthread_exit(NULL);
}

/* Main thread */

void run_threads(int nthreads, int bufsize) {
  int i, c, rc;
  TWork threads[nthreads];

  spool_init(&string_spooler, bufsize);

  for (i = 0; i < nprods; i++) {
    threads[i].tid   = i;
    threads[i].ttype = WRITER;
    rc = pthread_create(&threads[i].thread, NULL, thread_writer_fun, (void*)&threads[i]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  printf("%d Writers created\n", nprods);

  for (c = 0; c < ncons; c++) {
    i = c + nprods;
    threads[i].tid   = i;
    threads[i].ttype = READER;
    rc = pthread_create(&threads[i].thread, NULL, thread_reader_fun, (void*)&threads[i]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  printf("%d Readers created\n", ncons);

  /* Wait for each of the producer threads to complete. */
  for (i = 0; i < nprods; i++) {
    pthread_join(threads[i].thread, NULL);
  }
}


int main(int argc, char* argv[]) {
  int nentries = 4;
  if (argc < 4) {
    printf("usage: %s n_proids n_cons n_strings [buf entries]\n", argv[0]);
    return -1;
  }
  if (argc > 4) {
    nentries = atoi(argv[4]);
  }
  if (nentries < 2) {
    fprintf(stderr, "Buffer must have at least two entries.\n");
    return 0;
  }

  nprods = atoi(argv[1]);
  ncons  =  atoi(argv[2]);
  nstrs  =  atoi(argv[3]);
  run_threads(nprods + ncons, nentries);
  return 0;
}

