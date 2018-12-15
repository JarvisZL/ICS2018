#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
    rtl_lm(&t0,&id_dest->addr,2);
    cpu.IDTR.limit=t0;
    if(id_dest->width==2)
    {
         assert(0);
         t1=id_dest->addr+0x1;
         rtl_lm(&t2,&t1,3);
         cpu.IDTR.base=t2;
    }
    else if(id_dest->width==4)
    {
        t1=id_dest->addr+0x2;
        rtl_lm(&t2,&t1,4);
        cpu.IDTR.base=t2;
    }
    // TODO();
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //TODO();
  switch(id_dest->reg)
  {
      case 0: cpu.cr0.val=id_src->val; Log("write cr0");break;
      case 3: cpu.cr3.val=id_src->val; Log("write cr3"); break;
      default : assert(0); break;
  }
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //TODO();
  switch(id_src->reg)
  {
      case 0: operand_write(id_dest,&cpu.cr0.val); break;
      case 3: operand_write(id_dest,&cpu.cr3.val); break;
      default : assert(0); break;
  }
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

extern  void raise_intr(uint8_t NO,vaddr_t ret_addr);
make_EHelper(int) {
 
    raise_intr(id_dest->val,decoding.seq_eip); 
    
    /*  //store message
    t0=cpu.eflags;
    rtl_push(&t0);
    t0=cpu.CS;
    rtl_push(&t0);
    rtl_push(&decoding.seq_eip);

   //get the door
    t0=cpu.IDTR.base+id_dest->val*8;
    t1=vaddr_read(t0,4);//low 32
    t0=t0+4;
    t2=vaddr_read(t0,4);//high 32
   // p==1 in linux
    t1=t1&0x0000ffff;
    t2=t2&0xffff0000;
    t1=t1|t2;//offset
    rtl_j(t1);
    */
    // TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
    
    rtl_pop(&decoding.jmp_eip);
    rtl_pop(&cpu.CS);
    rtl_pop(&cpu.eflags);
    rtl_j(decoding.jmp_eip);
    //TODO();

  print_asm("iret");
}

extern uint32_t pio_read_w(ioaddr_t addr);
extern uint32_t pio_read_l(ioaddr_t addr);
extern uint32_t pio_read_b(ioaddr_t addr);

make_EHelper(in) {
 switch(id_dest->width)
 {
     case 1: t2=pio_read_b(id_src->val); operand_write(id_dest,&t2); break;
     case 2: t2=pio_read_w(id_src->val); operand_write(id_dest,&t2); break;
     case 4: t2=pio_read_l(id_src->val); operand_write(id_dest,&t2); break;
 }
    // TODO();

  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif

}

extern uint32_t pio_write_l(ioaddr_t addr,uint32_t data);
extern uint32_t pio_write_w(ioaddr_t addr,uint32_t data);
extern uint32_t pio_write_b(ioaddr_t addr,uint32_t data);

make_EHelper(out) {
    switch(id_src->width)
    {
        case 1: pio_write_b(id_dest->val,id_src->val); break;
        case 2: pio_write_w(id_dest->val,id_src->val); break;
        case 4: pio_write_l(id_dest->val,id_src->val); break;
    }
    //TODO();

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
