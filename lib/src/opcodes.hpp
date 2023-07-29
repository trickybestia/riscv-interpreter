#include <cstdint>

namespace opcode {
enum opcode {
  LOAD = 0b0000011,
  STORE = 0b0100011,
  LUI = 0b0110111,
  AUIPC = 0b0010111,
  JAL = 0b1101111,
  JALR = 0b1100111,
  BRANCH = 0b1100011,
  OP = 0b0110011,
  OP_IMM = 0b0010011,
  FENCE = 0b0001111,
  SYSTEM = 0b1110011,
};
}

namespace funct3 {
enum funct3 {
  JALR = 0b000,

  BEQ = 0b000,
  BNE = 0b001,
  BLT = 0b100,
  BGE = 0b101,
  BLTU = 0b110,
  BGEU = 0b111,

  LB = 0b000,
  LH = 0b001,
  LW = 0b010,
  LBU = 0b100,
  LHU = 0b101,

  SB = 0b000,
  SH = 0b001,
  SW = 0b010,

  ADDI = 0b000,
  SLTI = 0b010,
  SLTIU = 0b011,
  XORI = 0b100,
  ORI = 0b110,
  ANDI = 0b111,
  SLLI = 0b001,
  SRLI = 0b101,
  SRAI = 0b101,

  ADD = 0b000,
  SUB = 0b000,
  SLL = 0b001,
  SLT = 0b010,
  SLTU = 0b011,
  XOR = 0b100,
  SRL = 0b101,
  SRA = 0b101,
  OR = 0b110,
  AND = 0b111,

  FENCE = 0b000,

  MUL = 0b000,
  MULH = 0b001,
  MULHSU = 0b010,
  MULHU = 0b011,
  DIV = 0b100,
  DIVU = 0b101,
  REM = 0b110,
  REMU = 0b111,
};
}

namespace funct7 {
enum funct7 {
  SLLI = 0b0000000,
  SRLI = 0b0000000,
  SRAI = 0b0100000,

  ADD = 0b0000000,
  SUB = 0b0100000,
  SLL = 0b0000000,
  SLT = 0b0000000,
  SLTU = 0b0000000,
  XOR = 0b0000000,
  SRL = 0b0000000,
  SRA = 0b0100000,
  OR = 0b0000000,
  AND = 0b0000000,

  MULDIV = 0b0000001,
};
}

namespace funct12 {
enum funct12 {
  ECALL = 0,
  EBREAK = 1,
};
}
