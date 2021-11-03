#include <cstdio>
#include <cstring>
#include <algorithm>

struct Number {
    bool sign = true;
    char* integer = new char[10000];
    char* fractional = new char[10000];
    Number(char* s) {
        if (s[0] == '-') {
            sign = false;
            strcpy(s, s+1);
        }
        char* pos = strstr(s, ".");
        if (pos != nullptr) {
            strncpy(integer, s, strlen(s)-strlen(pos));
            //удаляем нули с конца
            strcpy(s, pos+1);
            int end = -1;
            for (int i = strlen(s)-1; i >= 0; --i) {
                if (s[i] != '0') {
                    end = i;
                    break;
                }
            }
            strncpy(fractional, s, end+1);
        } else {
            integer = s;
        }
    }
};
bool is_first_bigger(char* a, char* b, bool is_integer = true) {
    if (is_integer && strlen(a) != strlen(b)) {
        return strlen(a) > strlen(b);
    } else if (strlen(a) != strlen(b)) {
        for (size_t i = 0; i < std::min(strlen(a),strlen(b)); ++i) {
            if (a[i] != b[i]) {
                return a[i] > b[i];
            }
        }
        return strlen(a) > strlen(b);
    }
    return (strcmp(a,b) + 1) / 2;//convert -1 and +1 to bool
}
char output(bool type, bool sign = true) {
    if (type == sign) {
        return '>';
    }
    return '<';
}
char cmp(char* first, char* second) {
    Number first_number(first);
    Number second_number(second);
    //равенство
    if (strcmp(first_number.integer, second_number.integer) == 0
        && strlen(first_number.integer) == strlen(second_number.integer)
        && strcmp(first_number.fractional, second_number.fractional) == 0
        && strlen(first_number.fractional) == strlen(second_number.fractional)
        && first_number.sign == second_number.sign) {
        return '=';
    }
    // только одно из чисел положительно
    if ((int)first_number.sign + (int)second_number.sign == 1) {
        return output(first_number.sign);
    }
    //целые части совпали
    if (strcmp(first_number.integer, second_number.integer) == 0) {
        return output(is_first_bigger(first_number.fractional,
                                      second_number.fractional,false),
                      first_number.sign);
    }
    return output(is_first_bigger(first_number.integer,
                                  second_number.integer),
                  first_number.sign);
}
int main() {
    char first[10000];
    char second[10000];
    scanf("%s%s", first, second);
    printf("%c", cmp(first, second));
}
