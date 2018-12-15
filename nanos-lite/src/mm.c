#include "memory.h"
#include "proc.h"

static void *pf = NULL;
extern PCB *current;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

int _map(_Protect *p,void *va,void* pa,int mode);
/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
   
    Log("malloc");
    if(new_brk > current->max_brk)
    {
       uintptr_t above=new_brk-current->max_brk;
       uintptr_t times=(above-1)/PGSIZE;
       for(uintptr_t i=0;i<=times;++i)
       {
           void* new_pg=new_page(1);
           _map(&current->as,(void *)current->max_brk,new_pg,1);
           current->max_brk+=PGSIZE;
       }
    }

   current->cur_brk=new_brk;
    
    return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
