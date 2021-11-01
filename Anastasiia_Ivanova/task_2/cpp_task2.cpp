#include <iostream>
#include <string>
#include <algorithm>

size_t findIndex(const std::string &string, const char element) {
  size_t index = string.find(element);
  if (index == std::string::npos) {
    index = string.size();
  }
  return index;
}


char Compare(const std::string &first, const std::string &second) {
  char more = '>';
  char less = '<';
  char equal = '=';
  if (first[0] == '-' && second[0] != '-') {
    return less;
  } else if (first[0] != '-' && second[0] == '-') {
    return more;
  } else if (first[0] == '-' && second[0] == '-') {
    less = '>';
    more = '<';
  }
  size_t firstDotIndex = findIndex(first, '.');
  size_t secondDotIndex = findIndex(second, '.');

  if (firstDotIndex < secondDotIndex) {
    return less;
  } else if (firstDotIndex > secondDotIndex) {
    return more;
  } else {
    for (int i = 0; i < std::max(first.size(), second.size()); ++i) {
      if (i == firstDotIndex) {
        continue;
      }
      if (i < second.size() && (i >= first.size() || first[i] < second[i]) && second[i] != '0') {
        return less;
      }
      if (i < first.size() && (i >= second.size() || first[i] > second[i]) && first[i] != '0') {
        return more;
      }
    }
    return equal;
  }

}

int main() {
  std::string first;
  std::string second;
  std::cin >> first >> second;
  std::cout << Compare(first, second);
  return 0;
}
