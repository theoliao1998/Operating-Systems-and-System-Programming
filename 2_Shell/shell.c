#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "tokenizer.h"

/* Convenience macro to silence compiler warnings about unused function parameters. */
#define unused __attribute__((unused))

#define _XOPEN_SOURCE 700


/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

int cmd_exit(struct tokens *tokens);
int cmd_help(struct tokens *tokens);
int cmd_pwd(struct tokens *tokens);
int cmd_cd(struct tokens *tokens);

/* Built-in command functions take token array (see parse.h) and return int */
typedef int cmd_fun_t(struct tokens *tokens);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_exit, "exit", "exit the command shell"},
  {cmd_pwd, "pwd", "show the current working directory"},
  {cmd_cd, "cd", "change the current working directory"},
};

/* Prints a helpful description for the given command */
int cmd_help(unused struct tokens *tokens) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  return 1;
}

/* Exits this shell */
int cmd_exit(unused struct tokens *tokens) {
  exit(0);
}

/* Prints the current working directory */
int cmd_pwd(unused struct tokens *tokens) {
  int len = 20;
  char* buffer = (char*)malloc(sizeof(char)*len);
  while(!getcwd(buffer,len)){
    len *= 2;
    buffer =(char*)realloc(buffer,len);
  }
  printf("%s\n", buffer);
  free(buffer);
  return 1;
}

/* Changes the current working directory */
int cmd_cd(unused struct tokens *tokens) {
  char * path = tokens_get_token(tokens, 1);
  if(path) {
    chdir(path);
  }
  return 1;
}

char* get_cmd(char* token) {
  if( access(token, F_OK ) != -1 ) {
    return token;
  } else {
    char* s = getenv("PATH");
    int i = 0;
    int len = 128;
    char* cmd = malloc(sizeof(char)*len);
    for(int j=1; j<=strlen(s); j++){
      if(s[j] == ':' || s[j] == '\0'){
        if(j-i > len){
          len *= 2;
          cmd = realloc(cmd, sizeof(char)*len);
        }
        strncpy(cmd,s+i,j-i);
        cmd[j-i] = '/';
        cmd[j-i+1] = '\0';
        strcat(cmd,token);
        if(access(cmd, F_OK) != -1){
          return cmd;
        }
        i = j+1;
      }
    }
    return cmd;
  }
}

/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  return -1;
}

/* Intialization procedures for this shell */
void init_shell() {
  /* Our shell is connected to standard input. */
  shell_terminal = STDIN_FILENO;

  /* Check if we are running interactively */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    /* If the shell is not currently in the foreground, we must pause the shell until it becomes a
     * foreground process. We use SIGTTIN to pause the shell. When the shell gets moved to the
     * foreground, we'll receive a SIGCONT. */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save the current termios to a variable, so it can be restored later. */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}


void sig_chld(int chld){
  signal(SIGINT       , SIG_DFL);
  kill(-getpgrp(), SIGTTIN);
}

void ignore_signal(){
  signal(SIGHUP       , SIG_IGN);   
  signal(SIGINT       , SIG_IGN);   
  signal(SIGQUIT      , SIG_IGN); 
  signal(SIGILL       , SIG_IGN);   
  signal(SIGTRAP      , SIG_IGN); 
  signal(SIGABRT      , SIG_IGN);    
  signal(SIGIOT       , SIG_IGN);  
  signal(SIGBUS       , SIG_IGN); 
  signal(SIGFPE       , SIG_IGN);   
  signal(SIGKILL      , SIG_IGN);  
  signal(SIGUSR1      , SIG_IGN);   
  signal(SIGSEGV      , SIG_IGN);   
  signal(SIGUSR2      , SIG_IGN);   
  signal(SIGPIPE      , SIG_IGN);  
  signal(SIGALRM      , SIG_IGN);   
  signal(SIGTERM      , SIG_IGN); 
  signal(SIGSTKFLT    , SIG_IGN); 
  signal(SIGCHLD      , SIG_IGN);  
  signal(SIGCLD       , SIG_IGN);    
  signal(SIGCONT      , SIG_IGN);   
  signal(SIGSTOP      , SIG_IGN);   
  signal(SIGTSTP      , SIG_IGN);   
  signal(SIGTTIN      , SIG_IGN);   
  signal(SIGTTOU      , SIG_IGN); 
  signal(SIGURG       , SIG_IGN);  
  signal(SIGXCPU      , SIG_IGN);  
  signal(SIGXFSZ      , SIG_IGN);  
  signal(SIGVTALRM    , SIG_IGN);  
  signal(SIGPROF      , SIG_IGN);   
  signal(SIGWINCH     , SIG_IGN);   
  signal(SIGIO        , SIG_IGN);   
  signal(SIGPOLL      , SIG_IGN);                                      
  signal(SIGIO, SIG_IGN);
  signal(SIGPWR       , SIG_IGN);   
  signal(SIGSYS       , SIG_IGN); 
}

void accept_signal(){
  signal(SIGHUP       , SIG_DFL);   
  signal(SIGINT       , SIG_DFL);   
  signal(SIGQUIT      , SIG_DFL); 
  signal(SIGILL       , SIG_DFL);   
  signal(SIGTRAP      , SIG_DFL); 
  signal(SIGABRT      , SIG_DFL);    
  signal(SIGIOT       , SIG_DFL);  
  signal(SIGBUS       , SIG_DFL); 
  signal(SIGFPE       , SIG_DFL);   
  signal(SIGKILL      , SIG_DFL);  
  signal(SIGUSR1      , SIG_DFL);   
  signal(SIGSEGV      , SIG_DFL);   
  signal(SIGUSR2      , SIG_DFL);   
  signal(SIGPIPE      , SIG_DFL);  
  signal(SIGALRM      , SIG_DFL);   
  signal(SIGTERM      , SIG_DFL); 
  signal(SIGSTKFLT    , SIG_DFL); 
  signal(SIGCHLD      , SIG_DFL);  
  signal(SIGCLD       , SIG_DFL);    
  signal(SIGCONT      , SIG_DFL);   
  signal(SIGSTOP      , SIG_DFL);   
  signal(SIGTSTP      , SIG_DFL);   
  signal(SIGTTIN      , SIG_DFL);   
  signal(SIGTTOU      , SIG_DFL); 
  signal(SIGURG       , SIG_DFL);  
  signal(SIGXCPU      , SIG_DFL);  
  signal(SIGXFSZ      , SIG_DFL);  
  signal(SIGVTALRM    , SIG_DFL);  
  signal(SIGPROF      , SIG_DFL);   
  signal(SIGWINCH     , SIG_DFL);   
  signal(SIGIO        , SIG_DFL);   
  signal(SIGPOLL      , SIG_DFL);                                      
  signal(SIGIO        , SIG_DFL);
  signal(SIGPWR       , SIG_DFL);   
  signal(SIGSYS       , SIG_DFL); 
}

int main(unused int argc, unused char *argv[]) {
  init_shell();
  
  // struct sigaction sa = {.sa_handler = sig_chld};
  // sigaction(SIGCHLD, &sa, NULL);
  //ignore_signal();
  // pipe(p2c);

  static char line[4096];
  int line_num = 0;

  /* Please only print shell prompts when standard input is not a tty */
  if (shell_is_interactive)
    fprintf(stdout, "%d: ", line_num);

  int saved_stdout = dup(STDOUT_FILENO);
  int saved_stdin = dup(STDIN_FILENO);
  while (fgets(line, 4096, stdin) != NULL) {
    /* Split our line into words. */
    struct tokens *tokens = tokenize(line);
    int fd;
    int l = tokens_get_length(tokens);
    char** a = malloc(sizeof(char*)*l);
    int i=0, j = 0;
    bool valid = true;
    
    while(i<tokens_get_length(tokens)){
      char* token = tokens_get_token(tokens,i++);
      if (strcmp(token,">") == 0){
        fd = open(tokens_get_token(tokens,i++),O_WRONLY | O_CREAT, 0600);
      }
      else if (strcmp(token,"<") == 0) {
        char* readfile = tokens_get_token(tokens,i++);
        FILE* fr = fopen(readfile,"r");
        if(fr!=NULL){
          char* tmp = malloc(sizeof(char)*1024);
          while(fscanf(fr,"%s",tmp)==1){
            if(j>=l){
              l *= 2;
              a = realloc(a,sizeof(char*)*l);
            }
            a[j++] = tmp;
            tmp = malloc(sizeof(char)*1024);
          }
          fclose(fr);
        } else {
          fprintf(stdout, "%s: No such file or directory\n", readfile);
          valid = false;
        }
      } 
      else {
        a[j++] = token;
      }
    }
    
    a = realloc(a,sizeof(char*)*j);

    /* Find which built-in function to run. */
    int fundex = lookup(tokens_get_token(tokens, 0));

    if(valid){
      if (fundex >= 0) {
        cmd_table[fundex].fun(tokens);
      } else {
        
        pid_t cpid;
        /* Run commands as programs. */
        if ((cpid=fork())==0){
          //accept_signal();
                  
          setpgrp();

          if(fd != -1){
            dup2(fd,STDOUT_FILENO);
            close(fd);
          }
          char* cmd = get_cmd(tokens_get_token(tokens, 0));
          if(j==1 && strcmp(a[0],"cat")==0){
            dup2(saved_stdin,STDIN_FILENO);
            // char *arg = (char *) malloc(sizeof(char)*1024);
            // scanf("%s", arg);
            execl(cmd, cmd, NULL, NULL);
            
          } else{
            execv(cmd, a);
          }

          free(cmd);
          
          dup2(saved_stdout,STDOUT_FILENO);
          _exit(0);
        } else {
          close(fd);
          
          
          // set the proccess group of child 
          setpgid(cpid, cpid);
          // set the proccess group in the foreground
          signal(SIGTTOU , SIG_IGN);
          tcsetpgrp(STDIN_FILENO, cpid);
          
          int status;
          wait(&status);
          tcsetpgrp(STDIN_FILENO, getpid());
          //init_shell();
        }
      }
    }
    
    
    
    if (shell_is_interactive)
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);
    
    /* Clean up memory */
    tokens_destroy(tokens);
    free(a);
    dup2(saved_stdin,STDIN_FILENO);
  }
  close(saved_stdout);
  return 0;
}
