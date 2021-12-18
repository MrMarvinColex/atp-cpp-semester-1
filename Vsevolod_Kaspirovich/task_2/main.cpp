#include <iostream>

int main() {
  std::string first_number;
  std::string second_number;
  std::cin >> first_number >> second_number;

  bool both_negative = (first_number[0] == '-' && second_number[0] == '-');
  if (both_negative) {
    first_number[0] = '0';
    second_number[0] = '0';
  }

  size_t first_dot_pos = first_number.find('.');
  size_t second_dot_pos = second_number.find('.');
  if (first_dot_pos == std::string::npos) {
    first_number.append(".");
    first_dot_pos = first_number.length()-1;
  }
  if (second_dot_pos == std::string::npos) {
    second_number.append(".");
    second_dot_pos = second_number.length()-1;
  }

  std::string border_elements;
  for (size_t i = first_dot_pos; i < second_dot_pos; ++i) {
    border_elements.append("0");
  }
  first_number = border_elements + first_number;
  border_elements = "";
  for (size_t i = second_dot_pos; i < first_dot_pos; ++i) {
    border_elements.append("0");
  }
  second_number = border_elements + second_number;

  for (size_t i = first_number.length(); i < second_number.length(); ++i) {
    first_number.append("0");
  }
  for (size_t i = second_number.length(); i < first_number.length(); ++i) {
    second_number.append("0");
  }

  if (first_number == second_number) {
    std::cout << "=\n";
  } else if (first_number < second_number) {
    std::cout << (both_negative ? ">\n" : "<\n");
  } else {
    std::cout << (both_negative ? "<\n" : ">\n");
  }
}
