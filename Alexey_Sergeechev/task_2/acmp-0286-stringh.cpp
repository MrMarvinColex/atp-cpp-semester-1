#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
 
char CmpIntPart(const char* first_num, const char* second_num, size_t dot_pos, bool is_negative) {
  if (strncmp(first_num, second_num, dot_pos) < 0) {
    return (is_negative ? '>' : '<');
  }
  if (strncmp(first_num, second_num, dot_pos) > 0) {
    return (is_negative ? '<' : '>');
  }
  return '=';
}
 
char CmpFloatPart(const char* first_num, const char* second_num, size_t float_index, bool is_negative) {
  while (float_index < strlen(first_num) && float_index < strlen(second_num)) {
    if (first_num[float_index] < second_num[float_index]) {
      return (is_negative ? '>' : '<');
    }
    if (first_num[float_index] > second_num[float_index]) {
      return (is_negative ? '<' : '>');
    }
    ++float_index;
  }
  while (float_index < strlen(first_num)) {
    if (first_num[float_index++] != '0') {
      return (is_negative ? '<' : '>');
    }
  }
  while (float_index < strlen(second_num)) {
    if (second_num[float_index++] != '0') {
      return (is_negative ? '>' : '<');
    }
  }
 
  return '=';
}
 
char Cmp(const char* first_num, const char* second_num) {
  bool is_negative = false;
  if (first_num[0] == '-' && second_num[0] == '-') {
    is_negative = true;
  }
  if (first_num[0] == '-' && second_num[0] != '-') {
    return '<';
  }
  if (first_num[0] != '-' && second_num[0] == '-') {
    return '>';
  }
  size_t dot_pos_first = (strchr(first_num, '.') == nullptr ? strlen(first_num) : strchr(first_num, '.') - first_num);
  size_t dot_pos_second = (strchr(second_num, '.') == nullptr ? strlen(second_num) : strchr(second_num, '.') - second_num);
  if (dot_pos_first < dot_pos_second) {
    return (is_negative ? '>' : '<');
  }
  if (dot_pos_first > dot_pos_second) {
    return (is_negative ? '<' : '>');
  }
  char result_int_cmp = CmpIntPart(first_num, second_num, dot_pos_first, is_negative);
  if (result_int_cmp != '=') {
    return result_int_cmp;
  }
  return CmpFloatPart(first_num, second_num, dot_pos_first + 1, is_negative);
}
 
 
int main() {
  char first_num[10000];
  char second_num[10000];
  scanf("%s", &first_num);
  scanf("%s", &second_num);
  printf("%c", Cmp(first_num, second_num));
  return 0;
}
