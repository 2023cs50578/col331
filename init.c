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

  printf(fd, "My PID is: %d\n", getpid());

  int pid = fork();
  
  if(pid == 0){
    printf(fd, "Child (PID %d) looping forever...\n", getpid());
    for(;;); 
  } else {
    printf(fd, "Parent waiting briefly, then killing child...\n");
    kill(pid);
    wait();
    printf(fd, "Child killed and reaped. Test passed!\n");
    for(;;);
  }
}
