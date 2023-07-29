#pragma once

#include <cstdint>
#include <cstring>

class RegFile {
public:
  RegFile() { this->Reset(); }

  uint32_t &PC() { return this->pc; }

  uint32_t Read(std::size_t index) const {
    if (index == 0)
      return 0;

    return this->registers[index - 1];
  }

  void Write(std::size_t index, uint32_t value) {
    if (index == 0)
      return;

    this->registers[index - 1] = value;
  }

  void Reset() {
    std::memset(&this->registers, 0, sizeof(this->registers));
    this->pc = 0;
  }

private:
  uint32_t registers[31];
  uint32_t pc;
};
