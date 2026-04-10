#include "types.h"
#include "user.h"
#include "fcntl.h"

#define MAXARGS 10
#define BUFSIZE 100

// Statically allocated buffers: xv6 has no malloc yet.
static char buf[BUFSIZE];
static char *argv[MAXARGS];

static int
iswhite(char c)
{
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static int
getcmd(char *b, int nbuf)
{
  printf(2, "$ ");
  b[0] = 0;
  gets(b, nbuf);
  if(b[0] == 0)
    return -1;
  return 0;
}

int
main(void)
{
  int fd;

  // Make sure fds 0, 1, 2 are all open on the console.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  while(getcmd(buf, sizeof(buf)) >= 0){
    int n = 0;
    char *redir_in = 0;
    char *redir_out = 0;
    char *p = buf;
    int bad = 0;

    // Tokenize in place: split on whitespace, recognise < and > as
    // one-character tokens whose following word is the target file.
    while(*p){
      while(iswhite(*p))
        *p++ = 0;
      if(*p == 0)
        break;

      if(*p == '<' || *p == '>'){
        char kind = *p;
        *p++ = 0;
        while(iswhite(*p))
          *p++ = 0;
        if(*p == 0){
          printf(2, "syntax: missing file after %c\n", kind);
          bad = 1;
          break;
        }
        if(kind == '<')
          redir_in = p;
        else
          redir_out = p;
        while(*p && !iswhite(*p))
          p++;
        continue;
      }

      if(n >= MAXARGS - 1){
        printf(2, "too many args\n");
        bad = 1;
        break;
      }
      argv[n++] = p;
      while(*p && !iswhite(*p))
        p++;
    }
    argv[n] = 0;

    if(bad || n == 0)
      continue;

    if(fork() == 0){
      if(redir_in){
        close(0);
        if(open(redir_in, O_RDONLY) < 0){
          printf(2, "cannot open %s\n", redir_in);
          exit();
        }
      }
      if(redir_out){
        close(1);
        if(open(redir_out, O_CREATE | O_WRONLY) < 0){
          printf(2, "cannot create %s\n", redir_out);
          exit();
        }
      }
      exec(argv[0], argv);
      printf(2, "exec %s failed\n", argv[0]);
      exit();
    }
    wait();
  }
  exit();
}
