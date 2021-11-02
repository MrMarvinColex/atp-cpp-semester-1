#include <iostream>
 
using std::string;
 
void get_int_float_parts(const string &num, string &int_part, string &float_part) {
    auto point_ind = num.find('.');
    if (point_ind != string::npos) {
        int_part = string(num.begin(), num.begin() + point_ind);
        float_part = string(num.begin() + point_ind + 1, num.end());
    } else {
        int_part = num;
        float_part = "0";
    }
}
 
char check_float_parts(string &a, string &b, bool negative) {
    size_t n_a = a.size();
    size_t n_b = b.size();
    if (n_a < n_b) {
        for (int i = 0; i < (n_b - n_a); ++i) {
            a += "0";
        }
    } else if (n_a > n_b) {
        for (int i = 0; i < (n_a - n_b); ++i) {
            b += "0";
        }
    }
    if (a < b) {
        return (negative ? '>' : '<');
    } else if (a > b) {
        return (negative ? '<' : '>');
    } else {
        return '=';
    }
}
 
char check_int_parts(string &a, string &b, bool negative) {
    size_t n_a = a.size();
    size_t n_b = b.size();
    if (n_a < n_b) {
        return (negative ? '>' : '<');
    } else if (n_a > n_b) {
        return (negative ? '<' : '>');
    }
 
    if (negative) {
        return (a < b ? '>' : '<');
    } else {
        return (a < b ? '<' : '>');
    }
}
 
int main() {
    string num_a;
    string num_b;
    std::cin >> num_a;
    std::cin >> num_b;
 
    string int_a;
    string float_a;
    get_int_float_parts(num_a, int_a, float_a);
 
    string int_b;
    string float_b;
    get_int_float_parts(num_b, int_b, float_b);
    if (int_a[0] == '-' && int_b[0] != '-') {
        std::cout << '<';
    } else if (int_a[0] != '-' && int_b[0] == '-') {
        std::cout << '>';
    } else {
        bool both_negative = (int_a[0] == '-') && (int_b[0] == '-');
        if (int_a != int_b) {
            std::cout << check_int_parts(int_a, int_b, both_negative);
        } else {
            std::cout << check_float_parts(float_a, float_b, both_negative);
        }
    }
}
