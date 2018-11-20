#include "common.h"
#include "syscall.h"
#include "unistd.h"

void _yield();
void _halt(int code);
int fs_open(const char* pathname,int flags, int mode);
int fs_close(int fd);
off_t fs_lseek(int fd,off_t offset,int whence);
ssize_t fs_read(int fd, void* buf,size_t len);
ssize_t fs_write(int fd, void* buf,size_t len);

static void out_write(uintptr_t index,uintptr_t len)
{
    char *ptr=(char *)index;
    for(uintptr_t i=0;i<len;++i)
         _putc(ptr[i]);
}

//pa3.2 we can let user to control stack
/*
extern char _end;
void *program_break=&_end;
*/
 int brk(void * addr)
{
  //  program_break=addr;
    return 0;
}


_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_lseek: c->GPRx=fs_lseek(a[1],(off_t)a[2],a[3]); 
                    break;
    case SYS_close: c->GPRx=fs_close(a[1]); 
                    break;
    case SYS_open: c->GPRx=fs_open((const char*)a[1],a[2],a[2]); 
                   break;
    case SYS_read: if(a[1]!=1&&a[1]!=2&&a[1]!=0) 
                       c->GPRx=fs_read(a[1],(void *)a[2],(size_t)a[3]); 
                   break;
    case SYS_brk:  c->GPRx=brk((void *)a[1]); 
                   break;    
    case SYS_write: if(a[1]==1||a[1]==2)
                    { out_write(a[2],a[3]); /*there is no ret*/}
                    else if(a[1]!=1&&a[1]!=2&&a[1]!=0) 
                            c->GPRx=fs_write(a[1],(void*)a[2],(size_t)a[3]); 
                     break;
    case SYS_exit: _halt(c->GPR2); 
                   break;
    case SYS_yield: _yield(); c->GPRx=0; 
                    break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

