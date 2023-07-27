#include <limits>

#include "instructions.hpp"

int32_t I_Instruction::Imm() const {
  bool isNegative = (this->imm11_0 & 0b100000000000) != 0;

  uint32_t result = isNegative ? 0xfffff800 : 0;

  result |= this->imm11_0;

  return result;
}

int32_t S_Instruction::Imm() const {
  bool isNegative = (this->imm11_5 & 0b1000000) != 0;

  uint32_t result = isNegative ? 0xfffff800 : 0;

  result |= this->imm4_0;
  result |= this->imm11_5 << 5;

  return result;
}

int32_t U_Instruction::Imm() const {
  uint32_t result = this->imm31_12 << 12;

  return result;
}

int32_t B_Instruction::Imm() const {
  bool isNegative = this->imm12 != 0;

  uint32_t result = isNegative ? 0xfffff000 : 0;

  result |= this->imm4_1 << 1;
  result |= this->imm10_5 << 5;
  result |= this->imm11 << 11;

  return result;
}

int32_t J_Instruction::Imm() const {
  bool isNegative = this->imm20 != 0;

  uint32_t result = isNegative ? 0xfff00000 : 0;

  result |= this->imm10_1 << 1;
  result |= this->imm11 << 11;
  result |= this->imm19_12 << 12;

  return result;
}
