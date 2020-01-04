/*
 * Defines an HTTP Server that spins off a new thread to send HTTP Responses.
 * The main thread should continue listening and accepting HTTP Requests.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dispatch.h"

struct client_info {
    void (*request_handler)(int);
    int client_socket_number;
};

void* handle_client(void * arg) {
  struct client_info* c = (struct client_info *)arg;
  c->request_handler(c->client_socket_number);
  close(c->client_socket_number);
  free(c);
  pthread_exit(NULL);
}

dispatcher_t *new_dispatcher(int concurrency, void (*request_handler)(int)) {
  dispatcher_t *dispatcher =  malloc(sizeof(dispatcher_t));
  dispatcher->request_handler = request_handler;
  return dispatcher;
}

void dispatch(dispatcher_t* dispatcher, int client_socket_number) {
  struct client_info* c = malloc(sizeof(struct client_info));
  pthread_t t;
  c->request_handler = dispatcher->request_handler;
  c->client_socket_number = client_socket_number;
  pthread_create(&t, NULL, handle_client, (void *)c);
  pthread_detach(t); // detach the thread on the stack
}
