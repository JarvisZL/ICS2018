#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
   uint32_t te0,te1,te2;
   te0=cpu.eflags;
   rtl_push(&te0);
   cpu.EFLAGS.IF=0;
   te0=cpu.CS;
   rtl_push(&te0);
   rtl_push(&ret_addr);
   
   te0=cpu.IDTR.base+NO*8;
   te1=vaddr_read(te0,4);
   te0=te0+4;
   te2=vaddr_read(te0,4);
// 63:47 offset(31:16) .......
// ...........offset(15:0)15:0
   te1=te1&0x0000ffff;
   te2=te2&0xffff0000;
   te1=te1|te2;
   rtl_j(te1);

Log("raise");
 // TODO();
}

void dev_raise_intr() {
   cpu.INTR=true;

}
