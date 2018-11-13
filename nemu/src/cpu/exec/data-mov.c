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
        t1=cpu.eax;
        rtl_push(&t1);
        t1=cpu.ecx;
        rtl_push(&t1);
        t1=cpu.edx;
        rtl_push(&t1);
        t1=cpu.ebx;
        rtl_push(&t1);
        rtl_push(&t0);
        t1=cpu.ebp;
        rtl_push(&t1);
        t1=cpu.esi;
        rtl_push(&t1);
        t1=cpu.edi;
        rtl_push(&t1);
    }
    
    //TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
    if(decoding.is_operand_size_16)
    {
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
    rtl_pop(&t2);
    cpu.ebp=t2;
    //TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
     if(cpu.gpr[0]._16<0)
         cpu.gpr[2]._16=0xffff;
     else cpu.gpr[2]._16=0;
   //TODO();
  }
  else {
     if(cpu.eax<0)
         cpu.edx=0xffffffff;
     else cpu.edx=0;
    //TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
      if(((cpu.gpr[0]._8[0]>>7)&0x1)==1)
          cpu.gpr[0]._8[1]=0xff;
      else
          cpu.gpr[0]._8[1]=0x00;
      // TODO();
  }
  else {
      if(((cpu.gpr[0]._16>>15)&0x1)==1)
         cpu.gpr[0]._32|=0xffff0000;
      else
         cpu.gpr[0]._32&=0x0000ffff;
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
