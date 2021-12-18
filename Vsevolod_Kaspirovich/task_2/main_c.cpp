#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

int main() {
  char* first_number;
  char* second_number;
  scanf("%ms\n%ms", &first_number, &second_number);
  size_t first_length = strlen(first_number);
  size_t second_length = strlen(second_number);

  bool both_negative = (first_number[0] == '-' && second_number[0] == '-');
  if (both_negative) {
    first_number[0] = '0';
    second_number[0] = '0';
  }

  char* first_dot_pos = strchr(first_number, '.');
  char* second_dot_pos = strchr(second_number, '.');
  if (first_dot_pos == nullptr) {
    first_number[first_length] = '.';
    first_dot_pos = first_number+first_length;
    ++first_length;
  }
  if (second_dot_pos == nullptr) {
    second_number[second_length] = '.';
    second_dot_pos = second_number+second_length;
    ++second_length;
  }

  size_t max_dot_indent = std::max(first_dot_pos - first_number,
                                   second_dot_pos - second_number);
  size_t first_offset = max_dot_indent - (first_dot_pos - first_number);
  size_t second_offset = max_dot_indent - (second_dot_pos - second_number);
  size_t buffer_size = max_dot_indent +
                       std::max(first_number+first_length-first_dot_pos,
                                second_number+second_length-second_dot_pos);

  char* first_buffer = static_cast<char *>(calloc(buffer_size + 1, sizeof(char)));
  char* second_buffer = static_cast<char *>(calloc(buffer_size + 1, sizeof(char)));
  memset(first_buffer, '0', buffer_size);
  first_buffer[buffer_size] = '\0';
  memset(second_buffer, '0', buffer_size);
  second_buffer[buffer_size] = '\0';

  memcpy(first_buffer + first_offset, first_number, first_length);
  memcpy(second_buffer + second_offset, second_number, second_length);

  int comparison_result = strcmp(first_buffer, second_buffer);
  if (comparison_result == 0) {
    printf("=\n");
  } else {
    printf(((comparison_result > 0) != both_negative) ? ">\n" : "<\n");
  }
}
