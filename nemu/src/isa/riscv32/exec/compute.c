#include "cpu/exec.h"

make_EHelper(lui) 
{
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc) 
{
  rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(auipc);
}

make_EHelper(addi)
{
  rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(addi);
}

//slli
make_EHelper(slli)
{
  rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(slli);
}

//slti
make_EHelper(slti)
{
  rtl_setrelop(RELOP_LT, &id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(slti);
}

//sltiu
make_EHelper(sltiu)
{
  rtl_setrelop(RELOP_LTU, &id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(sltiu);
}

//xori
make_EHelper(xori)
{
  rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(xori);
}

//srli_srai
make_EHelper(srli_srai)
{
  if (decinfo.isa.instr.funct7 == 0)
  {
    rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template2(srli);
  }
  else
  {
    rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template2(srai);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  //print_asm_template2(srli_srai);
}

//ori
make_EHelper(ori)
{
  rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(ori);
}

//andi
make_EHelper(andi)
{
  rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);

  print_asm_template2(andi);
}

//sub_add
make_EHelper(sub_add)
{
  switch(decinfo.isa.instr.funct7)
  {
    case 0b0000000:
      rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sub);
      break;
    case 0b0100000:
      rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(add);
      break;
    case 0b0000001:
      rtl_mul_lo(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mul);
      break;
    default:
      assert(0); 
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//sll(0b0000000->sll,0b0000001->mulh)
make_EHelper(sll)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(sll);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(mulh);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//slt(0b0000000->slt,0b0000001->mulhsu)
make_EHelper(slt)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_setrelop(RELOP_LT, &id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(slt);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_mul_hsu(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(mulhsu);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//sltu(0b0000000->sltu,0b0000001->mulhu)
make_EHelper(sltu)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_setrelop(RELOP_LTU, &id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(sltu);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(mulhu);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//xor(0b0000000->xor,0b0000001->div)
make_EHelper(xor)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(xor);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(div);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//srl_sra(0b0000000->srl,0b0100000->sra,0b0000001->divu)
make_EHelper(srl_sra)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(srl);
  }
  else if (decinfo.isa.instr.funct7 == 0b0100000)
  {
    rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(sra);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(divu);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//or(0b0000000->or,0b0000001->rem)
make_EHelper(or)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(or);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(rem);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

//and(0b0000000->and,0b0000001->remu)
make_EHelper(and)
{
  if (decinfo.isa.instr.funct7 == 0b0000000)
  {
    rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(and);
  }
  else if (decinfo.isa.instr.funct7 == 0b0000001)
  {
    rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
    print_asm_template3(remu);
  }
  else
  {
    assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}
