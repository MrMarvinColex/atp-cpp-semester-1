#include <iostream>

size_t convert_string_in_int(const char* str) {
  bool isMinus = false;
  int number = 0;
  for (size_t i = 0; str[i]; ++i) {
    if (str[i] == '-')
      isMinus = true;  
    number *= 10;
    number += (str[i] - '0');
  }
  return (isMinus ? -number : number);
}

bool check_index(size_t index, const size_t* indexes, size_t number_of_indexes) {
  // returns true if indexes don't repeat
  for (size_t i = 0; i < number_of_indexes; ++i) {
    if (index == indexes[i])
      return false;
  }
  return true;
}

void count_sum(long long& sum, int** arrays, size_t number_of_arrays, const size_t* arrays_size,
               size_t* indexes, size_t current_array, long long number) {
  if (current_array >= number_of_arrays) {
    sum += number;
    return;
  }
  for (size_t index = 0; index < arrays_size[current_array]; ++index) {
    if (check_index(index, indexes, current_array)) {
      indexes[current_array] = index;
      count_sum(sum, arrays, number_of_arrays, arrays_size,
                indexes, current_array + 1, number * arrays[current_array][index]);
    }
  }
}

int main(int argc, char** argv) {
  int** arrays = new int*[argc - 1];
  auto* arrays_size = new size_t[argc - 1];
  for (int i = 1; i < argc; ++i) {
    size_t array_size = convert_string_in_int(argv[i]);
    int* array = new int[array_size];
    for (size_t j = 0; j < array_size; ++j) {
      std::cin >> array[j];
    }
    arrays[i - 1] = array;
    arrays_size[i - 1] = array_size;
  }

  long long sum = 0;
  auto* indexes = new size_t[argc - 1];
  count_sum(sum, arrays, argc - 1, arrays_size, indexes, 0, 1);
  std::cout << sum;
  for (int i = 0; i < argc - 1; ++i)
    delete[] arrays[i];
  delete[] arrays;
  delete[] arrays_size;
  delete[] indexes;
}
