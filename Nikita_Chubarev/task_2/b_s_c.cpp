#include <cstdio>
#include <cstring>
 
 
void add_null_left(char*& num_0, char*& num_1, size_t len_nulls) {
    char *null_str = new char [len_nulls];
    sprintf (null_str,"%0*d", len_nulls, 0);
    num_0 = strcat(null_str, num_0);
}
 
void add_null_right(char*& num, size_t len_nulls){
    char *null_str = new char [len_nulls];
    sprintf (null_str,"%0*d", len_nulls, 0);
    num = strcat(num, null_str);
}
 
void check_float_parts(char*& a, char*& b, char*& dot_a, char*& dot_b) {
    size_t len_float_a = strlen(dot_a) - 1;
    size_t len_float_b = strlen(dot_b) - 1;
 
    if (len_float_a < len_float_b) {
        add_null_right(a, len_float_b - len_float_a);
    } else if (len_float_a > len_float_b) {
        add_null_right(b, len_float_a - len_float_b);
    }
}
 
void check_int_parts(char*& a, char*& b, char*& dot_a, char*& dot_b) {
    size_t len_int_a = dot_a - a;
    size_t len_int_b = dot_b - b;
 
    if (len_int_a < len_int_b) {
        add_null_left(a, b, len_int_b - len_int_a);
    } else if (len_int_b < len_int_a) {
        add_null_left(b, a, len_int_a - len_int_b);
    }
}
 
void check_dot(char*& num, char*& dot_pos) {
    if (dot_pos == nullptr) {
        num = strcat(num, ".");
        dot_pos = strchr(num, '.');
    }
}
 
int main() {
    char* num_a;
    char* num_b;
    scanf("%ms", &num_a);
    scanf("%ms", &num_b);
 
    if (num_a[0] == '-' && num_b[0] != '-') {
        printf("%c", '<');
    } else if (num_a[0] != '-' && num_b[0] == '-') {
        printf("%c", '>');
    } else {
 
        bool negative = false;
        if ((num_a[0] == '-') && (num_b[0] == '-')) {
            negative = true;
            num_a[0] = '0';
            num_b[0] = '0';
        }
 
        char* dot_pos_a = strchr(num_a, '.');
        char* dot_pos_b = strchr(num_b, '.');
 
        check_dot(num_a, dot_pos_a);
        check_dot(num_b, dot_pos_b);
 
        check_int_parts(num_a, num_b, dot_pos_a, dot_pos_b);
        check_float_parts(num_a, num_b, dot_pos_a, dot_pos_b);
 
        if (strcmp(num_a, num_b) < 0) {
            printf("%c", (negative ? '>' : '<'));
        } else if (strcmp(num_a, num_b) > 0) {
            printf("%c", (negative ? '<' : '>'));
        } else {
            printf("%c", '=');
        }
    }
}
