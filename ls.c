#include "types.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  int fd;
  struct dirent de;
  char *path = "."; // Default to current directory

  if(argc > 1){
    path = argv[1];
  }

  // O_RDONLY is 0
  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    exit();
  }

  // A directory is just a file containing an array of struct dirent.
  // We can read them sequentially.
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue; // inum 0 means the entry is empty/deleted
    
    printf(1, "%s\n", de.name);
  }
  
  close(fd);
  exit();
}