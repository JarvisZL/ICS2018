#include "common.h"
#include "syscall.h"

void _yield();
void _halt(int code);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
      case SYS_write: {
                          if(a[1]==1||a[1]==2)
                          {
                             char * ptr=(void *)a[2];
                             for(int i=0;i<8*a[3];++i)
                             {
                                  _putc(*ptr);
                                  ptr++;
                             }
                             c->GPRx=a[3];
                          }
                          else
                            c->GPRx=-1;
                          break;
                      }
    case SYS_exit: _halt(c->GPR2); break;
    case SYS_yield: _yield(); c->GPRx=0; break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
