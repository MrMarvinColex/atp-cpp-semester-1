#ifndef MY_STRING_H
#define MY_STRING_H

#include <cstring>
#include <iostream>

class String{
    char* symbols_; // символы строки
    size_t size_; // размер строки
    size_t size_reserved_; // capacity

    // Считаем размер capacity от заданного size
    static size_t mk_sz_res(size_t sz) {
        size_t sz_res = 1;
        while ((sz_res <= sz)) {
            sz_res *= 2;
        }
        return sz_res;
    }

public:
    // default конструктор (size_reserved_ равен 1 для коректной работы push_back
    String() {
        size_ = 0;
        size_reserved_ = 1;
        symbols_ = nullptr;
    }

    // конструктор от c-строки
    String(const char* chr): size_(strlen(chr)), size_reserved_(mk_sz_res(size_)) {
        symbols_ = new char[size_reserved_];
        memcpy(symbols_, chr, size_);
    }

    // конструтор от числа повторений и символа
    String(size_t n, const char chr): size_(n), size_reserved_(mk_sz_res(size_)) {
        symbols_ = new char[size_reserved_];
        memset(symbols_, chr, size_);
    }

    // конструтор копирования
    String(const String& str_1): size_(str_1.size_), size_reserved_(mk_sz_res(size_)) {
        symbols_ = new char[size_reserved_];
        memcpy(symbols_, str_1.symbols_, size_);
    }

    // деструктор
    ~String() {
        delete[] symbols_;
    }

    // оператор присваивания
    String& operator = (const String& str_1) {
        if (this != &str_1) {
            delete[] symbols_;
            size_ = str_1.size_;
            size_reserved_ = str_1.size_reserved_;
            symbols_ = new char[size_reserved_];
            memcpy(symbols_, str_1.symbols_, size_);
        }
        return *this;
    }

    // оператор обращения по индексу для const String (копирование)
    char operator [] (size_t ind) const {
        return symbols_[ind];
    }
    // оператор обращения по индексу для String (ссылка)
    char& operator [] (size_t ind) {
        return symbols_[ind];
    }

    // оператор String += String
    String& operator += (const String& str_1) {
        if (!str_1.empty()) {
            size_t new_size = size_ + str_1.size_;
            size_t new_size_reserved = (new_size >= size_reserved_? mk_sz_res(new_size) : size_reserved_);
            char* new_symbols = new char[new_size_reserved];
            memcpy(new_symbols, symbols_, size_);
            delete[] symbols_;
            symbols_ = new_symbols;
            for (size_t i = 0; i < str_1.size_; ++i) {
                symbols_[size_ + i] = str_1.symbols_[i];
            }
            size_ = new_size;
            size_reserved_ = new_size_reserved;
        }
        return *this;
    }

    // оператор String += char
    String& operator += (const char chr) {
        this->push_back(chr);
        return *this;
    }

    // операторы, для удобства определенные вне класса
    friend bool operator == (const String& str_1, const String& str_2);
    friend std::ostream& operator << (std::ostream& out_stream, const String& str_to_out);
    friend std::istream& operator >> (std::istream& in_stream, String& str_to_in);

    // длина строки
    size_t length() const {
        return size_;
    }

    // push_back с проверкой, превышает ли capacity или нет
    void push_back(const char chr) {
        if (size_ + 1 == size_reserved_) {
            char* new_symbols;
            size_reserved_ *= 2;
            new_symbols = new char[size_reserved_];
            memcpy(new_symbols, symbols_, size_);
            delete[] symbols_;
            symbols_ = new_symbols;
        }
        symbols_[size_++] = chr;
    }

    // pop_back с проверкой, меньше ли размер 1/4 от capacity
    void pop_back() {
        if (!empty()) {
            if (size_ - 1 <= size_reserved_/4) {
                char* new_symbols;
                size_reserved_ /= 2;
                new_symbols = new char[size_reserved_];
                memcpy(new_symbols, symbols_, size_ - 1);
                delete[] symbols_;
                symbols_ = new_symbols;
            }
            --size_;
        }
    }

    // первый символ у const String (копирование)
    char front() const {
        char& front_char = symbols_[0];
        return front_char;
    }

    // первый символ у String (ссылка)
    char& front() {
        char& front_char = symbols_[0];
        return front_char;
    }

    // последний символ у const String (копирование)
    char back() const {
        char& back_char = symbols_[size_ - 1];
        return back_char;
    }

    // последний символ у String (ссылка)
    char& back() {
        char& back_char = symbols_[size_ - 1];
        return back_char;
    }

    // подстрока String
    String substr(size_t start, size_t count) const {
        String new_str;
        new_str.size_ = count;
        new_str.size_reserved_ = mk_sz_res(count);
        new_str.symbols_ = new char[size_reserved_];
        for (size_t i = 0; i < count; ++i) {
            new_str.symbols_[i] = symbols_[start + i];
        }

        return new_str;
    }

    // find
    size_t find(const String& substring) const {
        for (size_t i = 0; i < size_; ++i) {
            bool if_find = true;
            for (size_t k = 0; k < substring.size_; ++k) {
                if (symbols_[i + k] != substring[k]) {
                    if_find = false;
                    break;
                }
            }
            if (if_find) {
                return i;
            }
        }
        return size_;
    }

    // rfind
    size_t rfind(const String& substring) const {
        size_t k = size_ - substring.size_;
        for (size_t i = 0; i <= k; ++i) {
            bool if_find = true;
            for (size_t j = 0; j < substring.size_; ++j) {
                if (symbols_[k - i + j] != substring[j]) {
                    if_find = false;
                    break;
                }
            }
            if (if_find) {
                return k - i;
            }
        }
        return size_;
    }

    // empty
    bool empty() const {
        return size_ == 0;
    }

    // clear (size_reserved аналогично как в default конструкторе)
    void clear() {
        delete[] symbols_;
        symbols_ = nullptr;
        size_ = 0;
        size_reserved_ = 1;
    }
};

// String == String
bool operator == (const String& str_1, const String& str_2) {
    if (str_1.size_ != str_2.size_) {
        return false;
    }
    for (size_t i = 0; i < str_1.size_; ++i) {
        if (str_1.symbols_[i] != str_2.symbols_[i]) {
            return false;
        }
    }
    return true;
}

// вывод
std::ostream& operator << (std::ostream& out_stream, const String& str_to_out) {
    for (size_t k = 0; k < str_to_out.size_; ++k) {
        out_stream << str_to_out.symbols_[k];
    }
    return out_stream;
}

// ввод, до конца строки, конца файла или не ведущего пробела
std::istream& operator >> (std::istream& in_stream, String& str_to_in) {
    str_to_in.clear();
    char chr;
    in_stream.get(chr);
    while (!in_stream.eof() && (chr != '\n') && (chr != ' ' || str_to_in.size_ == 0)) {
        if (chr != ' ') {
            str_to_in += chr;
        }
        in_stream.get(chr);
    }
    return in_stream;
}

// оператор String + String
String operator + (const String& str_1, const String& str_2) {
    String new_str;
    if (!str_1.empty()) {
        new_str = str_1;
    }
    new_str += str_2;
    return new_str;
}

// оператор String + char
String operator + (const String& str_1, char chr) {
    String new_str;
    if (!str_1.empty()) {
        new_str = str_1;
    }
    new_str += chr;
    return new_str;
}

// оператор char + String
String operator + (char chr, const String& str_1) {
    String new_str(1, chr);
    new_str += str_1;
    return new_str;
}

#endif