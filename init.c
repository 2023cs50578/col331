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

  printf(fd, "Testing fork, wait, and exit!\n");

  int pid = fork();
  
  if(pid < 0){
    printf(fd, "Fork failed!\n");
  } else if(pid == 0){
    printf(fd, "Child executing, will now exit...\n");
    exit(); 
  } else {
    printf(fd, "Parent waiting for child...\n");
    int reaped_pid = wait();
    printf(fd, "Parent reaped child PID %d. Test successful!\n", reaped_pid);
    for(;;); // Keep parent alive so OS doesn't panic
  }
  
  return 0;
}
