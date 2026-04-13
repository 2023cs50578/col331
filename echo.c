#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    // Print the argument, followed by a space if it's not the last one, or a newline if it is
    printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
  }
  
  exit();
}