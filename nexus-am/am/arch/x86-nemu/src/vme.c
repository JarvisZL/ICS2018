#include <x86.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }
  
  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return 0;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
}

int _map(_Protect *p, void *va, void *pa, int mode) {
   if(mode==0)
       return 0;
    
   uintptr_t dir_base=(uintptr_t)p->ptr;
   uintptr_t dir=(uintptr_t)(PDX(va)<<2);
   PDE * pde_p=(PDE *)(dir_base|dir);
   
   if(((*pde_p)&PTE_P)==0)//need to create the second page table 
   {
       PTE * new_pt=(PTE *)pgalloc_usr(1);
       (*pde_p)=(uintptr_t)new_pt|PTE_P;
   }

   //assert((*pde_p)&PTE_P);
   //search the second page table
   
   uintptr_t pg=(uintptr_t)(PTX(va)<<2);
   uintptr_t pg_base=(uintptr_t)((*pde_p)&0xfffff000); 
   PTE* pte_p=(PTE*)(pg_base|pg); 
   
   (*pte_p)=(uintptr_t)pa|PTE_P; 
    
   return 0;
}

_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
    _Context * ret_u=(_Context *) ((uintptr_t)ustack.end-sizeof(_Context)-4);
   
     
     ret_u->prot=p;
     ret_u->edi=0;
     ret_u->esi=0;
     ret_u->ebp=(uintptr_t) ustack.end;
     ret_u->esp=0;
     ret_u->ebx=0;
     ret_u->edx=0;
     ret_u->ecx=0;
     ret_u->eax=0;
     ret_u->irq=0x81;
     ret_u->err=0;
     ret_u->eip=(uintptr_t) entry;
     ret_u->cs=8;
     ret_u->eflags=0x2;
     
   //  memset(ret_u+sizeof(_Context),0,4);
    return ret_u;
}
