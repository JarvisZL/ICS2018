#ifndef __ARCH_H__
#define __ARCH_H__

#include <am.h>

#define PMEM_SIZE (128 * 1024 * 1024)
#define PGSIZE    4096    // Bytes mapped by a page

typedef uint32_t size_t;



//more ealier ,addr more small --zly 
struct _Context {
  struct _Protect *prot;
  uintptr_t edi,esi,ebp,esp,ebx,edx,ecx,eax; 
  int irq;
  uintptr_t err;
  uintptr_t eip,cs,eflags;
};

#define GPR1 eax
#define GPR2 ebx
#define GPR3 ecx
#define GPR4 edx
#define GPRx eax
//yuan lai shi eip

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
