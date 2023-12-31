#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../lib/include/riscv-interpreter.h"

using namespace std;

const size_t MEMORY_SIZE = 16384;

void ReadFile(filesystem::path &path, byte *data, size_t dataLength) {
  ifstream file(path);

  size_t fileSize = filesystem::file_size(path);

  if (fileSize > dataLength) {
    stringstream errorStringStream;

    errorStringStream << "file is too large (" << fileSize << " > "
                      << MEMORY_SIZE << ")";

    throw length_error(errorStringStream.str());
  }

  file.read(reinterpret_cast<char *>(data), fileSize);
}

int32_t SyscallHandler(void *, uint32_t number, int32_t arg1) {
  if (number == 1) {
    cout << char(arg1) << flush;

    return 0;
  }

  return -1;
}

int main(int argc, char **argv) {
  if (argc < 2)
    throw logic_error("file name not provided");

  filesystem::path filePath = filesystem::path(argv[1]);

  byte *memory = new byte[MEMORY_SIZE];

  memset(memory, 0, MEMORY_SIZE);

  ReadFile(filePath, memory, MEMORY_SIZE);

  void *interpreter = RiscVInterpreterNew(memory, MEMORY_SIZE, SyscallHandler);

  while (!RiscVInterpreterIsStopped(interpreter))
    RiscVInterpreterTick(interpreter);
}
