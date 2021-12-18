#include <iostream>
#include <string>
 
char string_compare(std::string s1, std::string s2) {
  char zn1, zn2;
  if (s1[0] == '-') {
    zn1 = '-';
    s1 = s1.substr(1, s1.size() - 1);
  }
  else zn1 = '+';
  if (s2[0] == '-') {
    zn2 = '-';
    s2 = s2.substr(1, s2.size() - 1);
  }
  else zn2 = '+';
  if (zn1 == '-' && zn2 == '+') return '<';
  if (zn1 == '+' && zn2 == '-') return '>';
  int point_index = s1.find('.');
  std::string cel_part1, drob_part1, cel_part2, drob_part2;
  if (point_index != -1) {
    cel_part1 = s1.substr(0, point_index);
    drob_part1 = s1.substr(point_index + 1, s1.size() - point_index);
  }
  else {
    cel_part1 = s1;
    drob_part1 = "";
  }
  point_index = s2.find('.');
  if (point_index != -1) {
    cel_part2 = s2.substr(0, point_index);
    drob_part2 = s2.substr(point_index + 1, s2.size() - point_index);
  }
  else {
    cel_part2 = s2;
    drob_part2 = "";
  }
  while (cel_part1.size() < cel_part2.size()) cel_part1 = "0" + cel_part1;
  while (cel_part2.size() < cel_part1.size()) cel_part2 = "0" + cel_part2;
  while (drob_part1.size() < drob_part2.size()) drob_part1 = drob_part1 + "0";
  while (drob_part2.size() < drob_part1.size()) drob_part2 = drob_part2 + "0";
  if (cel_part1 > cel_part2) {
    return (zn1 == '+' ? '>' : '<');
  }
  if (cel_part2 > cel_part1) {
    return (zn2 == '+' ? '<' : '>');
  }
  if (drob_part1 > drob_part2) {
    return (zn1 == '+' ? '>' : '<');
  }
  if (drob_part2 > drob_part1) {
    return (zn2 == '+' ? '<' : '>');
  }
  return '=';
}
 
int main() {
  std::string s1, s2;
  std::getline(std::cin, s1);
  std::getline(std::cin, s2);
  std::cout << string_compare(s1, s2);
}
