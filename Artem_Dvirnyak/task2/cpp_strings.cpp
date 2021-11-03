#include <iostream>
#include <string>
#include <algorithm>

using std::string;

struct Number {
    bool sign = true;
    string integer;
    string fractional;
    Number(string s) {
        if (s[0] == '-') {
            sign = false;
            s = s.substr(1);
        }
        if (s.find('.') != string::npos) {
            size_t pos = s.find('.');
            integer = s.substr(0, pos);
            //удаляем нули с конца
            s = s.substr(pos+1);
            size_t end = s.find_last_not_of('0');
            end = (end == string::npos) ? 0 : end + 1;
            s = s.substr(0, end);
            fractional = s;
        } else {
            integer = s;
            fractional = "";
        }
    }
};
bool is_first_bigger(string a, string b, bool is_integer = true) {
    if (is_integer && a.size() != b.size()) {
        return a.size() > b.size();
    }
    return a > b;
}
string output(bool type, bool sign = 1) {
    if (type == sign) {
        return ">";
    }
    return "<";
}
string cmp(string& first, string& second) {
    Number first_number(first);
    Number second_number(second);
    //равенство
    if (first_number.integer == second_number.integer
    && first_number.fractional == second_number.fractional
    && first_number.sign == second_number.sign) {
        return "=";
    }
    // только одно из чисел положительно
    if ((int)first_number.sign + (int)second_number.sign == 1) {
        return output(first_number.sign);
    }
    if (first_number.integer == second_number.integer) {
        return output(is_first_bigger(first_number.fractional,
                                      second_number.fractional,false),
                      first_number.sign);
    }
    return output(is_first_bigger(first_number.integer,
                                  second_number.integer),
                  first_number.sign);
}
int main() {
    string first, second;
    std::cin >> first >> second;
    std::cout << cmp(first, second);
}
