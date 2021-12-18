#include <iostream>

bool is_index_unused(int num_indexes, size_t*& indexes, size_t new_index){ 
    for (int i = 0; i < num_indexes; ++i) {
        if (new_index == indexes[i]) {
	    return false;	
        }
    }
    return true;
}

long long calculate(int current_array, size_t*& indexes, size_t*& starts_of_indexes,
                    int*& total_array, int last_array) {
    if (current_array > last_array) {
        return 1ll;
    }
    long long result = 0ll;
    for (size_t i_k = 0ll; i_k < starts_of_indexes[current_array] - starts_of_indexes[current_array-1]; ++i_k) {
        if (is_index_unused(current_array, indexes, i_k)){
            indexes[current_array] = i_k;
            result += total_array[starts_of_indexes[current_array-1] + i_k] * calculate(current_array+1, indexes,
                                                                                   starts_of_indexes,total_array, last_array);
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    size_t* starts_of_indexes;
    starts_of_indexes = new size_t[argc-1];
    starts_of_indexes[0] = std::stoi(argv[1]);
    for (int i = 2; i < argc; ++i) {
        size_t n_i = std::stoll(argv[i]);
        starts_of_indexes[i-1] = n_i + starts_of_indexes[i-2];
    }

    int* total_array = new int[starts_of_indexes[argc-2]];
    for (size_t i = 0; i < starts_of_indexes[argc-2]; ++i) {
        std::cin >> total_array[i];
    }
    long long total_sum = 0; // искомая сумма
    auto* indexes = new size_t[argc-1]; // индексы, которые уже выбраны
    for (size_t i_1 = 0ll; i_1 < starts_of_indexes[0]; ++i_1){
        indexes[0] = i_1;
        total_sum += total_array[i_1] * calculate(1, indexes,starts_of_indexes,
                                                 total_array, argc-2);
    }
    std::cout<<total_sum;
    delete[] starts_of_indexes;
    delete[] total_array;
    delete[] indexes;
}
