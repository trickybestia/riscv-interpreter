#include <sstream>
#include <stdexcept>

#include "interpreter.hpp"
#include "opcodes.hpp"

using namespace std;

Interpreter::Interpreter(byte *mem, uint32_t memLength,
                         int32_t (*syscallHandler)(void *interpreter,
                                                   uint32_t number,
                                                   int32_t arg1))
    : mem(mem), memLength(memLength), syscallHandler(syscallHandler),
      isStopped(false) {}

void Interpreter::Tick() {
  if (this->IsStopped())
    return;

  bool updatePC = true, invalidInstruction = false;

  try {
    Instruction i = this->ReadInstruction(this->regFile.PC());

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
      else
        invalidInstruction = true;
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
        rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) <
                       static_cast<int32_t>(rr(i.r.rs2)));
      else if (i.r.funct3 == funct3::SLTU && i.r.funct7 == funct7::SLTU)
        rw(i.r.rd, rr(i.r.rs1) < rr(i.r.rs2));
      else if (i.r.funct3 == funct3::SLL && i.r.funct7 == funct7::SLL)
        rw(i.r.rd, rr(i.r.rs1) << (rr(i.r.rs2) & 0b11111));
      else if (i.r.funct3 == funct3::SRL && i.r.funct7 == funct7::SRL)
        rw(i.r.rd, rr(i.r.rs1) >> (rr(i.r.rs2) & 0b11111));
      else if (i.r.funct3 == funct3::SRA && i.r.funct7 == funct7::SRA)
        rw(i.r.rd,
           static_cast<int32_t>(rr(i.r.rs1)) >> (rr(i.r.rs2) & 0b11111));
      else if (i.r.funct3 == funct3::MUL && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) *
                       static_cast<int32_t>(rr(i.r.rs2)));
      else if (i.r.funct3 == funct3::MULH && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, (static_cast<int64_t>(rr(i.r.rs1)) *
                    static_cast<int64_t>(rr(i.r.rs2))) >>
                       32);
      else if (i.r.funct3 == funct3::MULHU && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, (static_cast<uint64_t>(rr(i.r.rs1)) * rr(i.r.rs2)) >> 32);
      else if (i.r.funct3 == funct3::MULHSU && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, (static_cast<int32_t>(rr(i.r.rs1)) *
                    static_cast<uint64_t>(rr(i.r.rs2))) >>
                       32);
      else if (i.r.funct3 == funct3::DIV && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) /
                       static_cast<int32_t>(rr(i.r.rs2)));
      else if (i.r.funct3 == funct3::DIVU && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, rr(i.r.rs1) / rr(i.r.rs2));
      else if (i.r.funct3 == funct3::REM && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, static_cast<int32_t>(rr(i.r.rs1)) %
                       static_cast<int32_t>(rr(i.r.rs2)));
      else if (i.r.funct3 == funct3::REMU && i.r.funct7 == funct7::MULDIV)
        rw(i.r.rd, rr(i.r.rs1) % rr(i.r.rs2));
      else
        invalidInstruction = true;
    } else if (i.unknown.opcode == opcode::JAL) {
      rw(i.j.rd, this->regFile.PC() + 4);

      int32_t offset = i.j.Imm();

      this->regFile.PC() += offset;

      updatePC = false;
    } else if (i.unknown.opcode == opcode::JALR && i.i.funct3 == funct3::JALR) {
      rw(i.i.rd, this->regFile.PC() + 4);

      uint32_t address = (i.i.Imm() + rr(i.i.rs1)) & 0xfffffffe;

      this->regFile.PC() = address;

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
      } else
        invalidInstruction = true;
    } else if (i.unknown.opcode == opcode::LOAD) {
      uint32_t address = rr(i.i.rs1) + i.i.Imm(), value = 0;

      if (i.i.funct3 == funct3::LW) {
        this->ReadMemory(reinterpret_cast<byte *>(&value), 4, address);
      } else if (i.i.funct3 == funct3::LH) {
        this->ReadMemory(reinterpret_cast<byte *>(&value), 2, address);

        bool isNegative = (value & 0x8000) != 0;

        if (isNegative)
          value |= 0xffff0000;
      } else if (i.i.funct3 == funct3::LHU) {
        this->ReadMemory(reinterpret_cast<byte *>(&value), 2, address);
      } else if (i.i.funct3 == funct3::LB) {
        this->ReadMemory(reinterpret_cast<byte *>(&value), 1, address);

        bool isNegative = (value & 0b10000000) != 0;

        if (isNegative)
          value |= 0xffffff00;
      } else if (i.i.funct3 == funct3::LBU) {
        this->ReadMemory(reinterpret_cast<byte *>(&value), 1, address);
      } else
        invalidInstruction = true;

      if (!invalidInstruction)
        rw(i.i.rd, value);
    } else if (i.unknown.opcode == opcode::STORE) {
      uint32_t address = rr(i.s.rs1) + i.s.Imm(), value = rr(i.s.rs2);

      if (i.s.funct3 == funct3::SW) {
        this->WriteMemory(reinterpret_cast<byte *>(&value), 4, address);
      } else if (i.s.funct3 == funct3::SH) {
        this->WriteMemory(reinterpret_cast<byte *>(&value), 2, address);
      } else if (i.s.funct3 == funct3::SB) {
        this->WriteMemory(reinterpret_cast<byte *>(&value), 1, address);
      } else
        invalidInstruction = true;
    } else if (i.unknown.opcode == opcode::SYSTEM && i.i.rd == 0 &&
               i.i.rs1 == 0) {
      if (i.i.Imm() == funct12::ECALL) {
        uint32_t number = rr(10);
        int32_t arg1 = rr(11);

        int32_t result = this->syscallHandler(this, number, arg1);

        rw(10, result);
      } else if (i.i.Imm() == funct12::EBREAK) {
        invalidInstruction = true;
      } else
        invalidInstruction = true;
    } else
      invalidInstruction = true;
  } catch (out_of_range const &ex) {
    invalidInstruction = true;
  }

  if (invalidInstruction) {
    updatePC = false;
    this->isStopped = true;
  }

  if (updatePC)
    this->regFile.PC() += 4;
}

void Interpreter::Reset() {
  memset(this->mem, 0, this->memLength);
  this->regFile.Reset();
  this->isStopped = false;
}

Instruction Interpreter::ReadInstruction(uint32_t address) const {
  Instruction result;

  this->ReadMemory(reinterpret_cast<byte *>(&result), sizeof(result), address);

  return result;
}

void Interpreter::ThrowMemoryOutOfRangeException(uint32_t address) const {
  stringstream errorStringStream;

  errorStringStream << "Memory address out of range (" << address
                    << " >= " << this->memLength << ")";

  throw out_of_range(errorStringStream.str());
}

void Interpreter::ReadMemory(std::byte *buffer, size_t count,
                             uint32_t address) const {
  for (size_t i = 0; i != count; i++, address++) {
    if (address >= this->memLength)
      this->ThrowMemoryOutOfRangeException(address);

    buffer[i] = this->mem[address];
  }
}

void Interpreter::WriteMemory(std::byte *buffer, size_t count,
                              uint32_t address) {
  for (size_t i = 0; i != count; i++, address++) {
    if (address >= this->memLength)
      this->ThrowMemoryOutOfRangeException(address);

    this->mem[address] = buffer[i];
  }
}

void Interpreter::rw(uint32_t index, uint32_t value) {
  this->regFile.Write(index, value);
}

uint32_t Interpreter::rr(uint32_t index) const {
  return this->regFile.Read(index);
}
