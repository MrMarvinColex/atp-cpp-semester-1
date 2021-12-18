#include <stdio.h>
#include <string.h>

size_t findDotIndex(char *string) {
  for (size_t i = 0; i < strlen(string); ++i) {
    if (string[i] == '.') {
      return i;
    }
  }
  return strlen(string);
}
char Compare(char *first, char *second) {
  char more = '>';
  char less = '<';
  char equal = '=';
  size_t firstDotIndex = findDotIndex(first);
  size_t secondDotIndex = findDotIndex(second);
  if (first[0] == '-' && second[0] != '-') {
    return less;
  } else if (first[0] != '-' && second[0] == '-') {
    return more;
  } else if (first[0] == '-' && second[0] == '-') {
    less = '>';
    more = '<';
  }
  if (firstDotIndex < secondDotIndex) {
    return less;
  } else if (firstDotIndex > secondDotIndex) {
    return more;
  } else {
    size_t maxLength = (strlen(first) < strlen(second) ? strlen(second) : strlen(first));
    for (int i = 0; i < maxLength; ++i) {
      if (i == firstDotIndex) {
        continue;
      }
      if (i < strlen(second) && (i >= strlen(first) || first[i] < second[i]) && second[i] != '0') {
        return less;
      }
      if (i < strlen(first) && (i >= strlen(second) || first[i] > second[i]) && first[i] != '0') {
        return more;
      }
    }
    return equal;
  }
}

int main() {
  char first[10001];
  char second[10001];

  scanf("%s", first);
  scanf("%s", second);

  printf("%c", Compare(first, second));
  return 0;
}