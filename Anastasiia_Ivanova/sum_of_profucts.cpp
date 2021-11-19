#include <iostream>

long long multiply(long long previousMultiply, size_t arrayIndex, const size_t lastArrayIndex, const size_t* length,
                   long long** arraysPointer, bool* possibleIndexes) {
  long long count = 0;
  if (arrayIndex == lastArrayIndex + 1) {
    return arrayIndex == 0 ? 0 : previousMultiply;
  }
  for (size_t i = 0; i < length[arrayIndex]; ++i) {
    if (possibleIndexes[i]) {
      possibleIndexes[i] = false;
      count += multiply(previousMultiply * arraysPointer[arrayIndex][i], arrayIndex + 1,
                        lastArrayIndex, length, arraysPointer, possibleIndexes);
      possibleIndexes[i] = true;
    }

  }
  return count;
}

int main(int argc, char* argv[]) {
  auto* length = new size_t[argc - 1];
  auto** arraysPointer = new long long* [argc - 1];
  size_t maxLength = 0;
  for (int i = 0; i < argc - 1; ++i) {
    length[i] = strtol(argv[i + 1], nullptr, 10);
    arraysPointer[i] = new long long[length[i]];
    if (length[i] > maxLength) {
      maxLength = length[i];
    }
  }
  for (int i = 0; i < argc - 1; ++i) {
    for (size_t j = 0; j < length[i]; ++j) {
      std::cin >> arraysPointer[i][j];
    }
  }
  bool* possibleIndexes = new bool[maxLength];
  for (size_t i = 0; i < maxLength; ++i) {
    possibleIndexes[i] = true;
  }
  std::cout << multiply(1, 0, argc - 2, length, arraysPointer, possibleIndexes);
  for (int i = 0; i < argc - 1; ++i) {
    delete[](arraysPointer[i]);
  }
  delete[] possibleIndexes;
  delete[] length;
  delete[] arraysPointer;
  return 0;
}
