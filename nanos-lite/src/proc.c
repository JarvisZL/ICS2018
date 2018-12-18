#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;
PCB s_pcb[MAX_NR_PROC];
PCB *fg_pcb;


void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}


extern void naive_uload(PCB *pcb,const char *filename);
extern void context_kload(PCB *pcb, void *entry);
extern void context_uload(PCB *pcb,const char *filename);

void init_proc() {
  //  context_kload(&pcb[0],(void*) hello_fun);
    context_uload(&pcb[0],"/bin/hello");
    context_uload(&pcb[1],"/bin/pal");
    context_uload(&pcb[2],"/bin/slider");
    context_uload(&pcb[3],"/bin/bmptest");
    for(int i=0;i<MAX_NR_PROC;++i)
        s_pcb[i]=pcb[i];
    fg_pcb=&pcb[3];
    switch_boot_pcb();
   // naive_uload(NULL, "/bin/init");
}


_Context* schedule(_Context *prev) {
    current->cp=prev;

     current=(current==&pcb[0]? fg_pcb:&pcb[0]);
  //  current=(current==&pcb[0]? &pcb[1]:&pcb[0]);
    // current=&pcb[0];
    return current->cp;
}
