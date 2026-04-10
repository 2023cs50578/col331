// init: The initial user-level program

#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int fd;

  // Open (or create) the console device as fd 0
  if((fd = open("console", O_RDWR)) < 0){
    mknod("console", 1, 1);
    fd = open("console", O_RDWR);
  }

  printf(fd, "Hello COL331 from init.c!\n");

  int pid = fork();
  
  if(pid < 0){
    printf(fd, "Fork failed!\n");
  } else if(pid == 0){
    printf(fd, "I am the child process!\n");
    for(;;); // Keep child alive
  } else {
    printf(fd, "I am the parent, my child's PID is %d\n", pid);
    for(;;); // Keep parent alive
  }
}
