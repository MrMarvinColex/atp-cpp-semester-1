#include <stdio.h>
#include <string.h>
#include <iostream>
 
char string_compare(char* s1, char* s2) {
  char zn1, zn2;
  if (s1[0] == '-') {
    zn1 = '-';
    strncpy(s1, s1 + 1, 10000);
  }
  else zn1 = '+';
  if (s2[0] == '-') {
    zn2 = '-';
    strncpy(s2, s2 + 1, 10000);
  }
  else zn2 = '+';
  if (zn1 == '-' && zn2 == '+') return '<';
  if (zn1 == '+' && zn2 == '-') return '>';
  int point_index = -1;
  for (int i = 0; i < 10001; ++i) {
    if (s1[i] == '.') {
      point_index = i;
      break;
    }
    if (i == 10000) {
      point_index = 10001;
      break;
    }
  }
  int p_ind = point_index;
  char cel_part1[10001], drob_part1[10001], cel_part2[10001], drob_part2[10001];
  if (point_index != -1) {
    size_t hah = point_index;
    strncpy(cel_part1, s1, hah);
    strncpy(drob_part1, s1 + point_index + 1, 10001);
  }
  else {
    *cel_part1 = *s1;
    *drob_part1 = {};
  }
  point_index = -1;
  for (int i = 0; i < 10001; ++i) {
    if (s2[i] == '.') {
      point_index = i;
      break;
    }
    if (i == 10000) {
      point_index = 10001;
      break;
    }
  }
  if (point_index != -1) {
    size_t hah = point_index;
    strncpy(cel_part2, s2, hah);
    strncpy(drob_part2, s2 + point_index + 1, 10001);
  }
  else {
    *cel_part2 = *s2;
    *drob_part2 = {};
  }
  for (int i = 0; (i < 10001) && (cel_part2[i] - '0' < 10) && (cel_part2[i] - '0' >= 0); ++i) {
    std::cout << cel_part2[i];
  }
  std::cout << std::endl;
  for (int i = 0; (i < 10001) && (drob_part2[i] - '0' < 10) && (drob_part2[i] - '0' >= 0); ++i) {
  }
  int len_cel_part1 = 0;
  while ((cel_part1[len_cel_part1] - '0' >= 0) && (cel_part1[len_cel_part1] - '0' < 10)) len_cel_part1++;
  int len_cel_part2 = 0;
  while ((cel_part2[len_cel_part2] - '0' >= 0) && (cel_part2[len_cel_part2] - '0' < 10)) len_cel_part2++;
  if (len_cel_part1 < len_cel_part2) {
    return (zn1 == '+' ? '<' : '>');
  }
  if (len_cel_part2 < len_cel_part1) {
    return (zn2 == '+' ? '>' : '<');
  }
  for (int i = 0; i < 10001; ++i) {
    if (((cel_part1[i] - '0' >= 10) || (cel_part1[i] - '0' < 0)) && ((cel_part2[i] - '0' < 10) && (cel_part2[i] - '0' >= 0))) {
      return (zn1 == '+' ? '<' : '>');
    }
    if (((cel_part2[i] - '0' >= 10) || (cel_part2[i] - '0' < 0)) && ((cel_part1[i] - '0' < 10) && (cel_part1[i] - '0' >= 0))) {
      return (zn2 == '+' ? '>' : '<');
    }
    if (!(((cel_part1[i] - '0' >= 10) || (cel_part1[i] - '0' < 0)) || ((cel_part2[i] - '0' >= 10) || (cel_part2[i] - '0' < 0)))) {
      if ((cel_part1[i] - '0' >= 10)
          || (cel_part1[i] - '0' < 0) && ((cel_part2[i] - '0' < 10) && (cel_part2[i] - '0' >= 0))) {
        return (zn2 == '+' ? '>' : '<');
      } else if ((cel_part2[i] - '0' >= 10)
          || (cel_part2[i] - '0' < 0) && ((cel_part1[i] - '0' < 10) && (cel_part1[i] - '0' >= 0))) {
        return (zn1 == '+' ? '<' : '>');
      } else if (cel_part1[i] < cel_part2[i]) {
        return (zn1 == '+' ? '<' : '>');
      } else if (cel_part2[i] < cel_part1[i]) {
        return (zn2 == '+' ? '>' : '<');
      }
    }
  }
  if (point_index != 10001) {
    for (int i = 0; i < 10001; ++i) {
      if (!(((drob_part1[i] - '0' >= 10) || (drob_part1[i] - '0' < 0))
          && ((drob_part2[i] - '0' >= 10) || (drob_part2[i] - '0' < 0)))) {
        if ((drob_part1[i] - '0' >= 10)
            || (drob_part1[i] - '0' < 0) && ((drob_part2[i] - '0' < 10) && (drob_part2[i] - '0' > 0))) {
          return (zn1 == '+' ? '<' : '>');
        } else if ((drob_part2[i] - '0' >= 10)
            || (drob_part2[i] - '0' < 0) && ((drob_part1[i] - '0' < 10) && (drob_part1[i] - '0' > 0))) {
          return (zn2 == '+' ? '>' : '<');
        }
        if (drob_part2[i] != '0' && ((drob_part1[i] - '0' >= 10) || (drob_part1[i] - '0' < 0))) {
          return (zn1 == '+' ? '>' : '<');
        } else if (drob_part1[i] != '0' && ((drob_part2[i] - '0' >= 10) || (drob_part2[i] - '0' < 0))) {
          return (zn2 == '+' ? '>' : '<');
        } else if ((drob_part1[i] < drob_part2[i] && ((drob_part2[i] - '0' < 10) && (drob_part2[i] - '0' >= 0))
            && ((drob_part1[i] - '0' < 10) && (drob_part1[i] - '0' >= 0)))) {
          return (zn1 == '+' ? '<' : '>');
        } else if ((drob_part2[i] < drob_part1[i] && ((drob_part2[i] - '0' < 10) && (drob_part2[i] - '0' >= 0))
            && ((drob_part1[i] - '0' < 10) && (drob_part1[i] - '0' >= 0)))) {
          return (zn2 == '+' ? '>' : '<');
        }
      }
    }
  }
  return '=';
}
 
int main() {
  char s1[10001];
  char s2[10001];
  scanf("%s", s1);
  scanf("%s", s2);
  printf("%c", string_compare(s1, s2));
}
