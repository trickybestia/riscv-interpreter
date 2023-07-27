#include <csignal>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "interpreter.hpp"

using namespace std;

void ReadFile(filesystem::path &path, vector<byte> &data) {
  ifstream file(path);

  size_t fileSize = filesystem::file_size(path);

  if (data.size() < fileSize)
    data.resize(fileSize);

  file.read(reinterpret_cast<char *>(data.data()), fileSize);
}

int main(int argc, char **argv) {
  if (argc < 2)
    return 1;

  filesystem::path filePath = filesystem::path(argv[1]);

  Interpreter interpreter;

  interpreter.Mem().resize(16384);

  ReadFile(filePath, interpreter.Mem());

  while (!interpreter.Stop()) {
    interpreter.Tick();
  }
}
