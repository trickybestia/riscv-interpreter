#pragma once

#include <cstdint>

struct UnknownInstruction {
  uint32_t opcode : 7;
  uint32_t remainder : 25;
};

struct R_Instruction {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t funct7 : 7;
};

struct I_Instruction {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t imm11_0 : 12;

  int32_t Imm() const;
};

struct S_Instruction {
  uint32_t opcode : 7;
  uint32_t imm4_0 : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t imm11_5 : 7;

  int32_t Imm() const;
};

struct U_Instruction {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t imm31_12 : 20;

  int32_t Imm() const;
};

struct B_Instruction {
  uint32_t opcode : 7;
  uint32_t imm11 : 1;
  uint32_t imm4_1 : 4;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t imm10_5 : 6;
  uint32_t imm12 : 1;

  int32_t Imm() const;
};

struct J_Instruction {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t imm19_12 : 8;
  uint32_t imm11 : 1;
  uint32_t imm10_1 : 10;
  uint32_t imm20 : 1;

  int32_t Imm() const;
};

union Instruction {
  UnknownInstruction unknown;
  R_Instruction r;
  I_Instruction i;
  S_Instruction s;
  U_Instruction u;
  B_Instruction b;
  J_Instruction j;
};
