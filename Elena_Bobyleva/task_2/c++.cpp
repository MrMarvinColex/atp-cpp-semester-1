#include <bits/stdc++.h>

using std::cin;
using std::cout;

unsigned int find_dot_idx(std::string& a, unsigned int& n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] == '.') {
            return i;
        }
    }
    a += '.';
    ++n;
    return n - 1;
}

bool find_natural_digit(std::string& s, unsigned int l, unsigned int r) {
    if (l >= r)
        return false;
    for (unsigned int i = l; i < r; ++i) {
        if ('1' <= s[i] && s[i] <= '9')
            return true;
    }
    return false;
}

char compare_abs(std::string& a, std::string& b) {
    unsigned int n = a.size();
    unsigned int m = b.size();
    unsigned int dot_idx_a = find_dot_idx(a, n);
    unsigned int dot_idx_b = find_dot_idx(b, m);

    if (dot_idx_a < dot_idx_b) {
        return '<';
    }
    if (dot_idx_a > dot_idx_b) {
        return '>';
    }

    for (int i = 0; i < std::min(n, m); ++i) {
        if (a[i] > b[i])
            return '>';
        if (a[i] < b[i])
            return '<';
    }

    bool find_natural_a = find_natural_digit(a, std::min(n, m), n);
    bool find_natural_b = find_natural_digit(b, std::min(n, m), m);
    if (find_natural_a)
        return '>';
    if (find_natural_b)
        return '<';
    return '=';
}

char compare(std::string& a, std::string& b) {
    if (a[0] == '-' && b[0] != '-') {
        return '<';
    }
    if (a[0] != '-' && b[0] == '-') {
        return '>';
    }
    if (a[0] != '-' && b[0] != '-') {
        return compare_abs(a, b);
    }
    char c = compare_abs(a, b);
    if (c == '=')
        return '=';
    if (c == '>')
        return '<';
    return '>';
}

signed main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string a, b;
    cin >> a >> b;
    cout << compare(a, b);
}
