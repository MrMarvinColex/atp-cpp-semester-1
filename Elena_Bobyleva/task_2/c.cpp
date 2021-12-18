#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 10010
#define NO  0
#define YES 1

typedef char BOOL;

char compare(const char* a, const char* b);
char compare_abs(const char* a, const char* b);
unsigned int find_dot_idx(const char* _string);
BOOL find_natural_digit(const char* _string, unsigned int l, unsigned int r);
unsigned int minimum(unsigned int _left, unsigned int _right);


int main()
{
    char a[MAX_LENGTH], b[MAX_LENGTH];
    scanf("%s",a);
    scanf("%s",b);
    
    printf("%c",compare(a, b));

    return 0;
}

char compare(const char* a, const char* b)
{
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

char compare_abs(const char* a, const char* b)
{
    unsigned int n = strlen(a);
    unsigned int m = strlen(b);
    unsigned int dot_idx_a = find_dot_idx(a);
    unsigned int dot_idx_b = find_dot_idx(b);
    
    if (dot_idx_a < dot_idx_b)
        return '<';
    
    if (dot_idx_a > dot_idx_b)
        return '>';
    
        for (int i = 0; i < minimum(n, m); i++)
        {
            if (a[i] > b[i])
                return '>';
            if (a[i] < b[i])
                return '<';
        }
        
    BOOL find_natural_a = find_natural_digit(a, minimum(n, m), n);
    BOOL find_natural_b = find_natural_digit(b, minimum(n, m), m);
    if (find_natural_a)
        return '>';
    if (find_natural_b)
        return '<';
    return '=';
        
}


unsigned int find_dot_idx(const char* _string)
{
    int i;
    for(i = 0; _string[i]; i++)
        if(_string[i] == '.')
            return i;
    return i;        
}

BOOL find_natural_digit(const char* _string, unsigned int l, unsigned int r)
{
        if(l >= r)
        return NO;
    for (unsigned int i = l; i < r; i++)
        if ('1' <= _string[i] && _string[i] <= '9')
            return YES;
    return NO;
}

unsigned int minimum(unsigned int _left, unsigned int _right)
{
    return _left < _right ? _left : _right;
}
