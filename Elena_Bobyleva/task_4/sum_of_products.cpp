#include <iostream>

int str_to_int(const char* str) {
    int result = 0;
    for (int i = 0; str[i]; i++) {
        result *= 10;
        result += str[i] - '0';
    }
    if (str[0] == '-') {
        result *= (-1);
    }
    return result;
}


long long get_sum_of_products(long long product, int index, int argc, int* sizes, int** array, bool* used) {
    static long long ans = 0;
    if (argc == index) {
        ans += product;
        return -1;
    }
    for (int i = 0; i < sizes[index]; ++i) {
        if (!used[i]) {
            used[i] = true;
            get_sum_of_products(product * array[index][i], index + 1, argc, sizes, array, used);
            used[i] = false;
        }

    }
    return ans;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main(int argc, char** argv) {
    argc--;
    int** array = new int* [argc];
    int* sizes = new int[argc];

    int max_k = 0;
    for (int i = 0; i < argc; i++) {
        sizes[i] = str_to_int(argv[i + 1]);
        max_k = max(max_k, sizes[i]);
        array[i] = new int[sizes[i]];
    }

    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            std::cin >> array[i][j];
        }
    }
    bool* used = new bool[max_k];
    for (int i = 0; i < max_k; i++) {
        used[i] = false;
    }
    std::cout << get_sum_of_products(1, 0, argc, sizes, array, used);
    for (int i = 0; i < argc; i++) {
        delete[] array[i];
    }
    delete[] array;
    delete[] sizes;
    delete[] used;
    return 0;
}
