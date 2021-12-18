#include <iostream>
#include <string>
 
char CmpIntPart(const std::string& first_num, const std::string& second_num, size_t dot_pos, bool is_negative) {
  if (first_num.substr(0, dot_pos) < second_num.substr(0, dot_pos)) {
    return (is_negative ? '>' : '<');
  }
  if (first_num.substr(0, dot_pos) > second_num.substr(0, dot_pos)) {
    return (is_negative ? '<' : '>');
  }
  return '=';
}
 
char CmpFloatPart(const std::string& first_num, const std::string& second_num, size_t float_index, bool is_negative) {
  while (float_index < first_num.size() && float_index < second_num.size()) {
    if (first_num[float_index] < second_num[float_index]) {
      return (is_negative ? '>' : '<');
    }
    if (first_num[float_index] > second_num[float_index]) {
      return (is_negative ? '<' : '>');
    }
    ++float_index;
  }
  while (float_index < first_num.size()) {
    if (first_num[float_index++] != '0') {
      return (is_negative ? '<' : '>');
    }
  }
  while (float_index < second_num.size()) {
    if (second_num[float_index++] != '0') {
      return (is_negative ? '>' : '<');
    }
  }
 
  return '=';
}
 
char Cmp(const std::string& first_num, const std::string& second_num) {
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
  size_t dot_pos_first = first_num.find(".");
  dot_pos_first = (dot_pos_first == std::string::npos ? first_num.size() : dot_pos_first);
  size_t dot_pos_second = second_num.find(".");
  dot_pos_second = (dot_pos_second == std::string::npos ? second_num.size() : dot_pos_second);
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
  std::string first_num;
  std::string second_num;
  std::cin >> first_num;
  std::cin >> second_num;
  std::cout << Cmp(first_num, second_num);
  return 0;
}
