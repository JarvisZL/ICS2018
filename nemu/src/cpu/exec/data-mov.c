#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
    rtl_push(&(id_dest->val));
//  TODO();

  print_asm_template1(push);
}

make_EHelper(pop) {
    rtl_pop(&t2);
    operand_write(id_dest,&t2);

//  TODO();

  print_asm_template1(pop);
}

make_EHelper(pusha) {
    if(decoding.is_operand_size_16)
    {
        assert(0);
        t0=cpu.gpr[4]._16;
        t1=cpu.gpr[0]._16;
        rtl_push(&t1);
        t1=cpu.gpr[1]._16;
        rtl_push(&t1);
        t1=cpu.gpr[2]._16;
        rtl_push(&t1);
        t1=cpu.gpr[3]._16;
        rtl_push(&t1);
        rtl_push(&t0);
        t1=cpu.gpr[5]._16;
        rtl_push(&t1);
        t1=cpu.gpr[6]._16;
        rtl_push(&t1);
        t1=cpu.gpr[7]._16;
        rtl_push(&t1);
    }
    else
    {
        t0=cpu.esp;
        rtl_push(&cpu.eax);
        rtl_push(&cpu.ecx);
        rtl_push(&cpu.edx);
        rtl_push(&cpu.ebx);
        rtl_push(&t0);
        rtl_push(&cpu.ebp);
        rtl_push(&cpu.esi);
        rtl_push(&cpu.edi);
    }
    
    //TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
    if(decoding.is_operand_size_16)
    {
        assert(0);
        rtl_pop(&t0);
        cpu.gpr[7]._16=(uint16_t) t0;
        rtl_pop(&t0);
        cpu.gpr[6]._16=(uint16_t) t0;
        rtl_pop(&t0);
        cpu.gpr[5]._16=(uint16_t) t0;
        rtl_pop(&t0);//useless esp
        rtl_pop(&t0);
        cpu.gpr[3]._16=(uint16_t) t0;
        rtl_pop(&t0);
        cpu.gpr[2]._16=(uint16_t) t0;
        rtl_pop(&t0);
        cpu.gpr[1]._16=(uint16_t) t0;
        rtl_pop(&t0);
        cpu.gpr[0]._16=(uint16_t) t0;
    }
    else
    {
        rtl_pop(&cpu.edi);
        rtl_pop(&cpu.esi);
        rtl_pop(&cpu.ebp);
        rtl_pop(&t0);//useless esp;
        rtl_pop(&cpu.ebx);
        rtl_pop(&cpu.edx);
        rtl_pop(&cpu.ecx);
        rtl_pop(&cpu.eax);
    }
  //TODO();

  print_asm("popa");
}

make_EHelper(leave) {
    
    cpu.esp=cpu.ebp;
    rtl_pop(&cpu.ebp);
    //TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
     assert(0);
     rtl_lr(&t0,0,2);
     t0>>=15;
     if(t0==1)
         rtl_li(&t1,0xffff);
     else
         rtl_li(&t1,0x0);
     rtl_sr(2,&t1,2);
     /* if(cpu.gpr[0]._16<0)
         cpu.gpr[2]._16=0xffff;
     else cpu.gpr[2]._16=0;
   */
     //TODO();
  }
  else {
    rtl_lr(&t0,0,4);
    t0>>=31;
    if(t0==1)
        rtl_li(&t1,0xffffffff);
    else
        rtl_li(&t1,0);
    rtl_sr(2,&t1,4); 
    /*
     if(cpu.eax<0)
         cpu.edx=0xffffffff;
     else cpu.edx=0;
   */
      //TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
      assert(0);
      rtl_lr(&t0,0,1);
      rtl_sext(&t1,&t0,1);
      rtl_sr(0,&t1,2);
      /*if(((cpu.gpr[0]._8[0]>>7)&0x1)==1)
          cpu.gpr[0]._8[1]=0xff;
      else
          cpu.gpr[0]._8[1]=0x00;
      */
      // TODO();
  }
  else {
     rtl_lr(&t0,0,2);
     rtl_sext(&t1,&t0,2);
     rtl_sr(0,&t1,4);
  
  /*
      if(((cpu.gpr[0]._16>>15)&0x1)==1)
         cpu.gpr[0]._32|=0xffff0000;
      else
         cpu.gpr[0]._32&=0x0000ffff;
   */
      // TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
