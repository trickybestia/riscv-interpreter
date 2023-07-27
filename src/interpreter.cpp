#include <iostream>

#include "interpreter.hpp"
#include "opcodes.hpp"

using namespace std;

Interpreter::Interpreter() { this->stop = false; }

void Interpreter::Tick() {
  if (this->Stop())
    return;

  Instruction i = this->ReadInstruction(this->regFile.PC());

  bool updatePC = true;

  if (i.unknown.opcode == opcode::OP_IMM) {
    if (i.i.funct3 == funct3::ADDI)
      rw(i.i.rd, rr(i.i.rs1) + i.i.Imm());
    else if (i.i.funct3 == funct3::SLTI)
      rw(i.i.rd, static_cast<int32_t>(rr(i.i.rs1)) < i.i.Imm());
    else if (i.i.funct3 == funct3::SLTIU)
      rw(i.i.rd, rr(i.i.rs1) < static_cast<uint32_t>(i.i.Imm()));
    else if (i.i.funct3 == funct3::ANDI)
      rw(i.i.rd, rr(i.i.rs1) & i.i.Imm());
    else if (i.i.funct3 == funct3::ORI)
      rw(i.i.rd, rr(i.i.rs1) | i.i.Imm());
    else if (i.i.funct3 == funct3::XORI)
      rw(i.i.rd, rr(i.i.rs1) ^ i.i.Imm());
    else if (i.i.funct3 == funct3::SLLI && i.r.funct7 == funct7::SLLI)
      rw(i.r.rd, rr(i.r.rs1) << i.r.rs2);
    else if (i.i.funct3 == funct3::SRLI && i.r.funct7 == funct7::SRLI)
      rw(i.r.rd, rr(i.r.rs1) >> i.r.rs2);
    else if (i.i.funct3 == funct3::SRAI && i.r.funct7 == funct7::SRAI)
      rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) >> i.r.rs2);
    else {
      updatePC = false;
      this->stop = true;
    }
  } else if (i.unknown.opcode == opcode::LUI) {
    rw(i.u.rd, i.u.Imm());
  } else if (i.unknown.opcode == opcode::AUIPC) {
    rw(i.u.rd, i.u.Imm() + this->regFile.PC());
  } else if (i.unknown.opcode == opcode::OP) {
    if (i.r.funct3 == funct3::ADD && i.r.funct7 == funct7::ADD)
      rw(i.r.rd, rr(i.r.rs1) + rr(i.r.rs2));
    else if (i.r.funct3 == funct3::SUB && i.r.funct7 == funct7::SUB)
      rw(i.r.rd, rr(i.r.rs1) - rr(i.r.rs2));
    else if (i.r.funct3 == funct3::AND && i.r.funct7 == funct7::AND)
      rw(i.r.rd, rr(i.r.rs1) & rr(i.r.rs2));
    else if (i.r.funct3 == funct3::OR && i.r.funct7 == funct7::OR)
      rw(i.r.rd, rr(i.r.rs1) | rr(i.r.rs2));
    else if (i.r.funct3 == funct3::XOR && i.r.funct7 == funct7::XOR)
      rw(i.r.rd, rr(i.r.rs1) ^ rr(i.r.rs2));
    else if (i.r.funct3 == funct3::SLT && i.r.funct7 == funct7::SLT)
      rw(i.r.rd,
         static_cast<int32_t>(rr(i.r.rs1)) < static_cast<int32_t>(rr(i.r.rs2)));
    else if (i.r.funct3 == funct3::SLTU && i.r.funct7 == funct7::SLTU)
      rw(i.r.rd, rr(i.r.rs1) < rr(i.r.rs2));
    else if (i.r.funct3 == funct3::SLL && i.r.funct7 == funct7::SLL)
      rw(i.r.rd, rr(i.r.rs1) << (rr(i.r.rs2) & 0b11111));
    else if (i.r.funct3 == funct3::SRL && i.r.funct7 == funct7::SRL)
      rw(i.r.rd, rr(i.r.rs1) >> (rr(i.r.rs2) & 0b11111));
    else if (i.r.funct3 == funct3::SRA && i.r.funct7 == funct7::SRA)
      rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) >> (rr(i.r.rs2) & 0b11111));
    else {
      updatePC = false;
      this->stop = true;
    }
  } else if (i.unknown.opcode == opcode::JAL) {
    rw(i.j.rd, this->regFile.PC() + 4);

    this->regFile.PC() += i.j.Imm();

    updatePC = false;
  } else if (i.unknown.opcode == opcode::JALR && i.i.funct3 == funct3::JALR) {
    rw(i.i.rd, this->regFile.PC() + 4);

    this->regFile.PC() += (i.i.Imm() + rr(i.i.rs1)) & 0xfffffffe;

    updatePC = false;
  } else if (i.unknown.opcode == opcode::BRANCH) {
    if (i.b.funct3 == funct3::BEQ) {
      if (rr(i.b.rs1) == rr(i.b.rs2)) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else if (i.b.funct3 == funct3::BNE) {
      if (rr(i.b.rs1) != rr(i.b.rs2)) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else if (i.b.funct3 == funct3::BLT) {
      if (static_cast<int32_t>(rr(i.b.rs1)) <
          static_cast<int32_t>(rr(i.b.rs2))) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else if (i.b.funct3 == funct3::BLTU) {
      if (rr(i.b.rs1) < rr(i.b.rs2)) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else if (i.b.funct3 == funct3::BGE) {
      if (static_cast<int32_t>(rr(i.b.rs1)) >=
          static_cast<int32_t>(rr(i.b.rs2))) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else if (i.b.funct3 == funct3::BGEU) {
      if (rr(i.b.rs1) >= rr(i.b.rs2)) {
        this->regFile.PC() += i.b.Imm();

        updatePC = false;
      }
    } else {
      updatePC = false;
      this->stop = true;
    }
  } else if (i.unknown.opcode == opcode::LOAD) {
    if (i.i.funct3 == funct3::LW) {
      byte rawValue[4];

      for (int j = 0; j != 4; j++)
        rawValue[j] =
            this->mem[(j + rr(i.i.rs1) + i.i.Imm()) % this->mem.size()];

      rw(i.i.rd, *reinterpret_cast<uint32_t *>(&rawValue));
    } else if (i.i.funct3 == funct3::LH) {
      byte rawValue[2];

      for (int j = 0; j != 2; j++)
        rawValue[j] =
            this->mem[(j + rr(i.i.rs1) + i.i.Imm()) % this->mem.size()];

      uint32_t value = *reinterpret_cast<uint16_t *>(&rawValue);

      bool isNegative = (value & 0x8000) != 0;

      if (isNegative)
        value |= 0xffff0000;

      rw(i.i.rd, value);
    } else if (i.i.funct3 == funct3::LHU) {
      byte rawValue[2];

      for (int j = 0; j != 2; j++)
        rawValue[j] =
            this->mem[(j + rr(i.i.rs1) + i.i.Imm()) % this->mem.size()];

      rw(i.i.rd, *reinterpret_cast<uint16_t *>(&rawValue));
    } else if (i.i.funct3 == funct3::LB) {
      byte rawValue = this->mem[(rr(i.i.rs1) + i.i.Imm()) % this->mem.size()];

      uint32_t value = static_cast<uint32_t>(rawValue);

      bool isNegative = (value & 0b10000000) != 0;

      if (isNegative)
        value |= 0xffffff00;

      rw(i.i.rd, value);
    } else if (i.i.funct3 == funct3::LHU) {
      byte rawValue = this->mem[(rr(i.i.rs1) + i.i.Imm()) % this->mem.size()];

      rw(i.i.rd, static_cast<uint32_t>(rawValue));
    } else {
      updatePC = false;
      this->stop = true;
    }
  } else if (i.unknown.opcode == opcode::STORE) {
    if (i.s.funct3 == funct3::SW) {
      uint32_t value = rr(i.s.rs2);
      byte *rawValue = reinterpret_cast<byte *>(&value);

      for (int j = 0; j != 4; j++)
        this->mem[(j + rr(i.i.rs1) + i.i.Imm()) % this->mem.size()] =
            rawValue[j];
    } else if (i.s.funct3 == funct3::SH) {
      uint32_t value = rr(i.s.rs2);
      byte *rawValue = reinterpret_cast<byte *>(&value);

      for (int j = 0; j != 2; j++)
        this->mem[(j + rr(i.i.rs1) + i.i.Imm()) % this->mem.size()] =
            rawValue[j];
    } else if (i.s.funct3 == funct3::SW) {
      uint32_t value = rr(i.s.rs2);
      byte *rawValue = reinterpret_cast<byte *>(&value);

      this->mem[(rr(i.i.rs1) + i.i.Imm()) % this->mem.size()] = *rawValue;
    } else {
      updatePC = false;
      this->stop = true;
    }
  } else if (i.unknown.opcode == opcode::SYSTEM && i.i.rd == 0 &&
             i.i.rs1 == 0) {
    if (i.i.Imm() == funct12::ECALL) {
      uint32_t c = this->regFile.Read(10);

      cout << (char)c;
    } else if (i.i.Imm() == funct12::EBREAK) {
      updatePC = false;
      this->stop = true;
    } else {
      updatePC = false;
      this->stop = true;
    }
  } else {
    updatePC = false;
    this->stop = true;
  }

  if (updatePC)
    this->regFile.PC() += 4;
}

void Interpreter::Reset() {
  this->Mem().clear();
  this->regFile.Reset();
  this->stop = false;
}

Instruction Interpreter::ReadInstruction(uint32_t address) const {
  byte result[4];

  for (size_t i = 0; i != 4; i++) {
    result[i] = this->mem[(address + i) % this->mem.size()];
  }

  return *reinterpret_cast<Instruction *>(&result);
}

void Interpreter::rw(uint32_t index, uint32_t value) {
  this->regFile.Write(index, value);
}

uint32_t Interpreter::rr(uint32_t index) const {
  return this->regFile.Read(index);
}
