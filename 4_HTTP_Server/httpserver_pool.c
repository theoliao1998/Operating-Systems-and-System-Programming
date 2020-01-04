/*
 * Defines an HTTP server that uses a thread pool to respond to HTTP requests.
 * Upon initialization, a fixed amount of threads are created. They will block
 * until an HTTP request is received. After serving an HTTP request, the thread
 * will return to the pool and await for more requests to be received.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dispatch.h"
#include "wq.h"

/* Worker routine for each thread in the pool. */
void *handle_clients(void *arg) {
  /* (Valgrind) Detach so thread frees its memory on completion, since we won't
   * be joining on it. */
  pthread_detach(pthread_self());
  dispatcher_t *dispatcher = (dispatcher_t *)arg;
  while(1){
    int client_socket_number = wq_pop(&(dispatcher->work_queue));
    dispatcher->request_handler(client_socket_number);
    close(client_socket_number);
  }
  pthread_exit(NULL);
}

dispatcher_t *new_dispatcher(int concurrency, void (*request_handler)(int)) {
  dispatcher_t *dispatcher =  malloc(sizeof(dispatcher_t));
  dispatcher->request_handler = request_handler;
  dispatcher->workers = malloc(sizeof(pthread_t)*concurrency);
  wq_init(&(dispatcher->work_queue));
  for(int i=0; i<concurrency; i++){
    pthread_create(dispatcher->workers+i, NULL, handle_clients, dispatcher);
  }
  return dispatcher;
}

void dispatch(dispatcher_t* dispatcher, int client_socket_number) {
  wq_push(&(dispatcher->work_queue),client_socket_number);
}
