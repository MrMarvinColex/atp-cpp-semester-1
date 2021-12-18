#include <stdio.h>

int find_dot(const char* number) {
  int i = 0;
  while (number[i] && (number[i] != '.'))
    ++i;
  return i;
}

char compare_before_dot(const char* number1, const char* number2, int dot_index) {
  for (int i = 0; i < dot_index; ++i) {
    if (number1[i] < number2[i])
      return '<';
    if (number1[i] > number2[i])
      return '>';
  }
  return '=';
}

char choose_sign(char sign, bool is_minus) {
  if (!is_minus)
    return sign;
  if (sign == '<')
    return '>';
  return '<';
}

char compare_after_dot(const char* number1, const char* number2, int dot_index) {
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

char compare(const char* number1, const char* number2) {
  bool is_minus = false;
  if ((number1[0] == '-') && (number2[0] != '-'))
    return '<';
  if ((number1[0] != '-') && (number2[0] == '-'))
    return '>';
  if ((number1[0] == '-') && (number2[0] == '-'))
    is_minus = true;

  int dot_index1 = find_dot(number1);
  int dot_index2 = find_dot(number2);
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
  char number1[10002];
  char number2[10002];
  scanf("%s", number1);
  scanf("%s", number2);
  printf("%c", compare(number1, number2));
}
