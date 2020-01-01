/*
 * Word count application with one process per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

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

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "word_count.h"
#include "word_helpers.h"

/*
 * Read stream of counts and accumulate globally.
 */
void merge_counts(word_count_list_t *wclist, FILE *count_stream) {
  char *word;
  int count;
  int rv;
  while ((rv = fscanf(count_stream, "%8d\t%ms\n", &count, &word)) == 2) {
    add_word_with_count(wclist, word, count);
  }
  if ((rv == EOF) && (feof(count_stream) == 0)) {
    perror("could not read counts");
  } else if (rv != EOF) {
    fprintf(stderr, "read ill-formed count (matched %d)\n", rv);
  }
}

/*
 * main - handle command line, spawning one process per file.
 */
int main(int argc, char *argv[]) {
  /* Create the empty data structure. */
  word_count_list_t word_counts;
  init_words(&word_counts);

  if (argc <= 1) {
    /* Process stdin in a single process. */
    count_words(&word_counts, stdin);
  } else {
    pid_t cpid[argc-1];
    int fd[2 * (argc-1)];
    for(int t = 1; t < argc; t++) {
      pipe(&fd[2*(t-1)]);
      if((cpid[t-1]=fork())==0){
        close(fd[2*(t-1)]);
        FILE * f = fopen(argv[t],"r");
        FILE * w = fdopen(fd[2*(t-1)+1],"w");
        if(f!=NULL){
          count_words(&word_counts,f);
          fclose(f);
        } else{
          perror("fopen argv");
          return 1;
        }
        if(w!=NULL){
          fprint_words(&word_counts, w);
          fclose(w);
        } else{
          close(fd[2*(t-1)]+1);
          perror("fopen pipe");
          return 1;
        }
        _exit(2);
      } else {
        close(fd[2*(t-1)]+1);
      }
    } 
    for (int t = 1; t < argc; t++) {
      waitpid(cpid[t-1], NULL, WNOHANG);
      FILE * r = fdopen(fd[2*(t-1)],"r");
      merge_counts(&word_counts,r);
      fclose(r);
    }
  }

  /* Output final result of all process' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  return 0;
}
