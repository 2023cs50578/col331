#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "file.h"

#define PIPESIZE 512

struct pipe {
  char data[PIPESIZE];
  uint nread;     // number of bytes read
  uint nwrite;    // number of bytes written
  int readopen;   // read fd is still open
  int writeopen;  // write fd is still open
};

int
pipealloc(struct file **f0, struct file **f1)
{
  struct pipe *p;

  p = 0;
  *f0 = *f1 = 0;
  if((*f0 = filealloc()) == 0 || (*f1 = filealloc()) == 0)
    goto bad;
  if((p = (struct pipe*)kalloc()) == 0)
    goto bad;
  p->readopen = 1;
  p->writeopen = 1;
  p->nwrite = 0;
  p->nread = 0;
  
  (*f0)->type = FD_PIPE;
  (*f0)->readable = 1;
  (*f0)->writable = 0;
  (*f0)->pipe = p;
  
  (*f1)->type = FD_PIPE;
  (*f1)->readable = 0;
  (*f1)->writable = 1;
  (*f1)->pipe = p;
  return 0;

bad:
  if(p) kfree((char*)p);
  if(*f0) fileclose(*f0);
  if(*f1) fileclose(*f1);
  return -1;
}

void
pipeclose(struct pipe *p, int writable)
{
  pushcli(); // Lock by disabling interrupts
  if(writable){
    p->writeopen = 0;
    wakeup(&p->nread);
  } else {
    p->readopen = 0;
    wakeup(&p->nwrite);
  }
  
  if(p->readopen == 0 && p->writeopen == 0){
    popcli(); // Unlock
    kfree((char*)p);
  } else {
    popcli(); // Unlock
  }
}

int
pipewrite(struct pipe *p, char *addr, int n)
{
  int i;
  pushcli(); // Lock by disabling interrupts
  
  for(i = 0; i < n; i++){
    while(p->nwrite == p->nread + PIPESIZE){  
      if(p->readopen == 0 || myproc()->killed){
        popcli();
        return -1;
      }
      wakeup(&p->nread);
      
      // Use your 1-argument sleep
      sleep(&p->nwrite);  
    }
    p->data[p->nwrite++ % PIPESIZE] = addr[i];
  }
  wakeup(&p->nread);  
  popcli(); // Unlock
  return n;
}

int
piperead(struct pipe *p, char *addr, int n)
{
  int i;
  pushcli(); // Lock by disabling interrupts
  
  while(p->nread == p->nwrite && p->writeopen){  
    if(myproc()->killed){
      popcli();
      return -1;
    }
    
    // Use your 1-argument sleep
    sleep(&p->nread); 
  }
  
  for(i = 0; i < n; i++){  
    if(p->nread == p->nwrite)
      break;
    addr[i] = p->data[p->nread++ % PIPESIZE];
  }
  wakeup(&p->nwrite);  
  popcli(); // Unlock
  return i;
}