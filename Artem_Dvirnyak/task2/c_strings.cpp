#include <cstdio>
#include <cstring>

char output(bool type, bool sign = true) {
    if (type == sign) {
        return '>';
    }
    return '<';
}
char cmp(char* first, char* second) {
    //только одно число положительно
    if (static_cast<int>(first[0] == '-') + static_cast<int>(second[0] == '-') == 1) {
        return output(first[0] != '-');
    }
    size_t pos1 = (strchr(first, '.') == nullptr) ? strlen(first) : strchr(first, '.') - &first[0];
    size_t pos2 = (strchr(second, '.') == nullptr) ? strlen(second) : strchr(second, '.') - &second[0];
    //целая часть длиннее
    if (pos1 != pos2) {
        return output((pos1 > pos2), (first[0] != '-'));
    }
    //целая часть больше
    if (strncmp(first, second, pos1) != 0) {
        return output(strncmp(first, second, pos1) > 0, (first[0] != '-'));
    }
    //сравниваем дробные части
    size_t i = pos1+1;
    for ( ; i < strlen(first) && i < strlen(second); ++i) {
        if (first[i] != second[i]) {
            return output(first[i] > second[i], first[0] != '-');
        }
    }
    // одна дробь закончилась, другая - нет
    for ( ; i < strlen(first) || i < strlen(second); ++i) {
        if (strlen(first) > i && first[i] != '0') {
            return output(true, first[0] != '-');
        }
        if (strlen(first) > i && first[i] != '0') {
            return output(false, first[0] != '-');
        }
    }
    return '=';
}
int main() {
    char first[10001];
    char second[10001];
    scanf("%s%s", first, second);
    printf("%c", cmp(first, second));
}
