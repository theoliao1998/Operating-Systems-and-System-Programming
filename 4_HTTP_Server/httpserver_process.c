/*
 * Defines an HTTP Server that creates a child process to send HTTP Responses.
 * The parent process should return to listening and accepting HTTP Requests.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "dispatch.h"

dispatcher_t *new_dispatcher(int concurrency, void (*request_handler)(int)) {
  dispatcher_t *dispatcher =  malloc(sizeof(dispatcher_t));
  dispatcher->request_handler = request_handler;
  return dispatcher;
}

void dispatch(dispatcher_t* dispatcher, int client_socket_number) {
  pid_t cpid;
  if(!(cpid = fork())){
    dispatcher->request_handler(client_socket_number);
    close(client_socket_number);
    _exit(0);
  } else {
    close(client_socket_number);
    waitpid(cpid, NULL, WNOHANG);
    waitpid(-1, NULL, WNOHANG); // kill all zombie processes
  }
}
