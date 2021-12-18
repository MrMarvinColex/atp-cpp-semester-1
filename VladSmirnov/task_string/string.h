#include <iostream>
#include <cstring>

class String {
private:
    char* string_;
    size_t size_ = 8;
    int64_t index_of_last_char_ = -1;
    
public:
    String() {
        string_ = new char[size_];
        string_[0] = '\0';
    }
    
    String (char char_to_string) : index_of_last_char_(0) {
        string_ = new char[size_];
        string_[0] = char_to_string;
        string_[1] = '\0';
    }
    
    // explicit не пишем, чтобы const char[] в случае чего кастанулось в const char* (array-to-pointer conversion)
    String(const char* pointer_to_arg_string) {
        size_t new_size = strlen(pointer_to_arg_string) + 1;
        index_of_last_char_ = new_size - 2;
        while (new_size > size_) {
            size_ *= 2; 
        }
        string_ = new char[size_];
        string_[0] = '\0';
        strcat(string_, pointer_to_arg_string);
    }

    String(int n, char char_for_string) :  index_of_last_char_(n - 1) { 
        size_t new_size = n + 1;
        while (new_size > size_) {
            size_ *= 2;
        }
        string_ = new char[size_];
        memset(string_, char_for_string, n);
        string_[n] = '\0';
    }
    
    String(const String& arg_string) : string_(new char[arg_string.size_]), size_(arg_string.size_), 
                                                index_of_last_char_(arg_string.index_of_last_char_) {
        memcpy(string_, arg_string.string_, size_);
    }
    
    void swap(String& arg_string) {
        std::swap(string_, arg_string.string_);
        std::swap(size_, arg_string.size_);
        std::swap(index_of_last_char_, arg_string.index_of_last_char_);
    }
    
    // copy-and-swap idiom
    String& operator=(const String& to_copy_string) {
        String temp_string(to_copy_string); // не можем написать String temp_string = to_copy_string, п.ч. конструктор явный
        swap(temp_string);
        return *this;
    }
     
    char& operator[](size_t index) {
        return string_[index];
    }
    
    const char& operator[](size_t index)const {
        return string_[index];
    }

    // concatenation
    String& operator+=(const String& second_string) {
        return operator+=(second_string.string_);
    }

    String& operator+=(const char* second_string) {
        size_t new_size = index_of_last_char_ + strlen(second_string) + 2;      
        if (new_size > size_) {
            String temp_string(*this);
            while (new_size > size_) size_ *= 2; 
            delete[] string_;
            string_ = new char[size_];
            string_[0] = '\0';
            strcat(string_, temp_string.string_);      
        }
        strcat(string_, second_string);
        index_of_last_char_ += strlen(second_string); 
        return *this;
    }

    String& operator+=(char second_string) {
        push_back(second_string);
        return *this;
    }

    void pop_back() {
        string_[index_of_last_char_--] = '\0';
        if (static_cast<size_t>(index_of_last_char_) * 4 < size_) {
            String temp_string(*this);
            delete[] string_;
            size_ /= 2;
            string_ = new char[size_];
            string_[0] = '\0';
            strcat(string_, temp_string.string_);
        }
    }
    
    void push_back(char new_char) {;
        if (static_cast<size_t>(index_of_last_char_ + 2) == size_) {
            String temp_string(*this);
            delete[] string_;
            size_ *= 2; 
            string_ = new char[size_];
            string_[0] = '\0';
            strcat(string_, temp_string.string_);
        }
        string_[++index_of_last_char_] = new_char;
        string_[index_of_last_char_ + 1] = '\0';
    }
        
    String substr(size_t start_index, int count)const {
        char* substring = new char[count + 1];
        substring[count] = '\0';
        while (--count >= 0) {
            substring[count] = string_[start_index + count];
        }
        String result = substring;
        delete substring;
        return result;
    }

    bool is_substring(int i, const char*& substring, size_t substring_size)const {
        bool is_substring = true;
        for (int j = 0; j < static_cast<int>(substring_size); ++j) {
            if (string_[i + j] != substring[j]) {
                is_substring = false;
                break;
            }
        }
        return is_substring;
    }

    size_t find(const char* substring)const {
        size_t substring_size = strlen(substring);
        if (substring_size == 0 || substring_size > static_cast<size_t>(index_of_last_char_ + 1)) return index_of_last_char_ + 1;
        for (int i = 0; i <= static_cast<int>(index_of_last_char_ - substring_size) + 1; ++i) {
            if (is_substring(i, substring, substring_size)) return i;   
        }
        return index_of_last_char_ + 1;
    }

    size_t rfind(const char* substring)const {
        size_t substring_size = strlen(substring);
        if (substring_size == 0 || substring_size > static_cast<size_t>(index_of_last_char_ + 1)) return index_of_last_char_ + 1;
        for (int i = index_of_last_char_ - substring_size + 1; i >= 0; --i) {
            if (is_substring(i, substring, substring_size)) return i;
        }
        return index_of_last_char_ + 1;
    }
    
    char& front() {
       return string_[0];
    }

    const char& front()const {
        return string_[0];
    }

    char& back() {
        return string_[index_of_last_char_];
    }

    const char& back()const {
        return string_[index_of_last_char_];
    }


    bool empty()const {
        return !(index_of_last_char_ + 1);
    }
    
    // можно сделать отдельную функцию для очистки памяти
    void clear() {
        string_[0] = '\0';
        index_of_last_char_ = -1; 
    }

    size_t length()const {
        return index_of_last_char_ + 1;
    }
     
    friend std::ostream& operator<<(std::ostream& out, const String& string);
    
    friend std::istream& operator>>(std::istream& in, String& string);
    
    friend bool operator==(const String&, const String&);  

    ~String() {
        delete[] string_;
    }
};

std::ostream& operator<<(std::ostream& out, const String& string) {
    if (string.string_ != nullptr) {  
        out << string.string_;
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& string) {
    string.clear();
    char char_for_string;
    while (in.get(char_for_string) && !std::isspace(char_for_string) && 
            !std::iscntrl(char_for_string)) {
        string.push_back(char_for_string);
    }
    return in;
}

String operator+(const String& first_string, const String& second_string) {
    String temp(first_string);
    temp += second_string;
    return temp;
}

bool operator==(const String& first_string, const String& second_string) {
    if (first_string.index_of_last_char_ != second_string.index_of_last_char_) return false;
    else {
        for (int i = 0; i <= first_string.index_of_last_char_; ++i) {
            if (first_string.string_[i] != second_string.string_[i]) return false;
        }
        return true;
    }
}

