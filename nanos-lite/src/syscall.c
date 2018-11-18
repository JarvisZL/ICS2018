#include "common.h"
#include "syscall.h"

void _yield();
void _halt(int code);


static void out_write(uintptr_t index,uintptr_t len)
{
    char *ptr=(char *)index;
    for(uintptr_t i=0;i<len;++i)
         _putc(ptr[i]);
}


_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
      case SYS_write: if(a[1]==1||a[1]==2){ out_write(a[2],a[3]); c->GPRx=c->GPR4; } else c->GPRx=-1; break;
    case SYS_exit: _halt(c->GPR2); break;
    case SYS_yield: _yield(); c->GPRx=0; break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

