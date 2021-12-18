#include <iostream>
#include <string>

char compare_before_dot(const std::string& number1, const std::string& number2, size_t dot_index) {
  if (number1.substr(0, dot_index) > number2.substr(0, dot_index))
    return '>';
  if (number1.substr(0, dot_index) < number2.substr(0, dot_index))
    return '<';
  return '=';
}

char choose_sign(char sign, bool is_minus) {
  if (!is_minus)
    return sign;
  if (sign == '<')
    return '>';
  return '<';
}

char compare_after_dot(const std::string& number1, const std::string& number2, int dot_index) {
  int i = dot_index;
  while (number1[i] && number2[i]) {
    if (number1[i] < number2[i])
      return '<';
    if (number1[i] > number2[i])
      return '>';
    ++i;
  }

  while (number2[i]) {
    if ((number2[i] != '0') && (number2[i] != '.'))
      return '<';
    ++i;
  }
  while (number1[i]) {
    if ((number1[i] != '0') && (number1[i] != '.'))
      return '>';
    ++i;
  }
  return '=';
}

char compare(std::string number1, std::string number2) {
  bool is_minus = false;
  if ((number1[0] == '-') && (number2[0] != '-'))
    return '<';
  if ((number1[0] != '-') && (number2[0] == '-'))
    return '>';
  if ((number1[0] == '-') && (number2[0] == '-'))
    is_minus = true;

  size_t dot_index1 = number1.find('.');
  if (dot_index1 == -1)
    dot_index1 = number1.size();
  size_t dot_index2 = number2.find('.');
  if (dot_index2 == -1)
    dot_index2 = number2.size();

  if (dot_index1 != dot_index2)
    return choose_sign((dot_index1 < dot_index2) ? '<' : '>', is_minus);

  char cmp_before_dot = compare_before_dot(number1, number2, dot_index1);
  if (cmp_before_dot != '=')
    return choose_sign(cmp_before_dot, is_minus);

  char cmp_after_dot = compare_after_dot(number1, number2, dot_index1);
  if (cmp_after_dot != '=')
    return choose_sign(cmp_after_dot, is_minus);
  return '=';
}

int main() {
  std::string number1;
  std::string number2;
  std::cin >> number1 >> number2;
  std::cout << compare(number1, number2);
}
