#include <iostream>

class Array{
private:
   int* array = nullptr;
   size_t size = 0;
public:
    Array() {}

    void get_array(size_t count) {
        size = count;
        array = new int[size];
        for (int i = 0; i < static_cast<int>(size); ++i) {
            std::cin >> array[i];
        }
    }
    
    size_t get_size()const {
        return size;
    }
    
    int get_element(size_t index)const { 
        return array[index];
    }
    
    void clear() {
       delete[] array; 
    }
};

class System{
private:
    Array* arrays = nullptr;
    bool* is_used = nullptr;
    size_t count = 0;
public:
    System(size_t argc, char* argv[]) : arrays(new Array[argc - 1]), count(argc - 1) {
        size_t max_size = 0;
        for (int i = 0; i < static_cast<int>(count); ++i) {
            size_t size = static_cast<size_t>(atoi(argv[i + 1]));
            max_size = std::max(max_size, size);
            arrays[i].get_array(size);
        }
        is_used = new bool[max_size];
        for (int i = 0; i < static_cast<int>(max_size); ++i) is_used[i] = false;
    }

    long long get_answer(int current_index_of_array) {
        if (current_index_of_array == static_cast<int>(count)) return 1;
        size_t current_size = arrays[current_index_of_array].get_size();
        long long current_value = 0;
        for (int i = 0; i < static_cast<int>(current_size); ++i) {
            if (!is_used[i]) {
                is_used[i] = 1;
                current_value += static_cast<long long>(arrays[current_index_of_array].get_element(i)) * get_answer(current_index_of_array + 1);
                is_used[i] = 0;
            }
        }
        return current_value;
    }
    
    void clear() {
        for (int i = 0; i < static_cast<int>(count); ++i) {
            arrays[i].clear();
        }
        delete[] arrays;
        delete[] is_used;
    }
};

int main(int argc, char* argv[]) {
    System arrays(argc, argv);
    if (argc == 1) std::cout << 0 << '\n';
    else std::cout << arrays.get_answer(0) << '\n';
    arrays.clear();
}
