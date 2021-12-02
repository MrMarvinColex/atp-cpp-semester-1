#include <iostream>
#include <cstring>

class String {
private:
    size_t len = 0;
    char* str;
    size_t memory_size = 2;

public:

    /// Destructor
    ~String() {
        delete[] str;
    }

    /// Constructors
    String() {
        len = 0;
        str = new char[2]; str[0] = '\0'; str[1] = '\0';
        memory_size = 2;
    }
    String (const char* inputArray): len(strlen(inputArray)), str(new char[len * 2]), memory_size(len * 2){
        str[0] = '\0';
        strcat(str, inputArray);
    }
    String (size_t count, char inputChar): len(count), str(new char[count * 2]), memory_size(count * 2) {
        memset(str, inputChar, count);
        str[len] = '\0';
    }
    String (char unoChar): len(1), str(new char[2]), memory_size(2) {
        str[0] = unoChar;
        str[1] = '\0';
    }
    String (const String& copyString): len(copyString.length()), str(new char[copyString.memory_size]), memory_size(copyString.memory_size) {
        str[0] = '\0';
        strcat(str, copyString.str);
    }

    /// Operators
    String& operator=(const String& copyString) {
        String copy(copyString);
        swap(copy);
        return *this;
    }

    char& operator[](size_t index) {
        return str[index];
    }
    const char& operator[](size_t index) const {
        return str[index];
    }

    String& operator+=(char addingChar) {
        push_back(addingChar);
        return *this;
    }
    String& operator+=(const String& addingString) {
        return operator+=(addingString.str);
    }
    String& operator+=(const char* addingChars) {
        size_t newLen = len + strlen(addingChars);
        if (newLen > memory_size) {
            String temporaryString(*this);
            delete[] str;
            memory_size = newLen * 2;
            str = new char[memory_size];
            str[0] = '\0';
            strcat(str, temporaryString.str);
        }
        strcat(str, addingChars);
        len = newLen;
        str[len] = '\0';
        return *this;
    }

    /// Methods
    size_t length() const {
        return len;
    }

    void push_back(char addingChar) {
        if (len >= memory_size - 1) {
            String temporaryString(*this);
            delete[] str;
            memory_size *= 2;
            str = new char[memory_size];
            str[0] = '\0';
            strcat(str, temporaryString.str);
        }
        str[len] = addingChar;
        len++;
        str[len] = '\0';
    }
    void pop_back() {
        str[len - 1] = '\0';
        len--;
        if (len < memory_size / 4) {
            String temporaryString(*this);
            delete[] str;
            memory_size /= 2;
            str = new char[memory_size];
            str[0] = '\0';
            strcat(str, temporaryString.str);
        }
    }

    char& front() {
        return str[0];
    }
    const char& front() const {
        return str[0];
    }
    char& back() {
        return str[len - 1];
    }
    const char& back() const {
        return str[len - 1];
    }

    size_t find(const String substring) const {
        size_t indexOfInclusion = len;
        for(size_t i = 0; i <= len - substring.len; i++) {
            size_t delta = 0;
            while (str[i + delta] == substring[delta]) {
                if (delta == substring.len - 1) indexOfInclusion = i;
                delta++;
            }
            if (indexOfInclusion == i) break;
        }
        return indexOfInclusion;
    }
    size_t rfind(const String substring) const {
        size_t indexOfInclusion = len;
        for(size_t i = len - substring.len;; i--) {
            size_t delta = 0;
            while (str[i + delta] == substring[delta]) {
                if (delta == substring.len - 1) indexOfInclusion = i;
                delta++;
            }
            if (indexOfInclusion == i || i == 0) break;
        }
        return indexOfInclusion;
    }

    String substr(size_t start, size_t count) const {
        char* substring = new char[count * 2];
        substring[0] = '\0';
        strcat(substring, str + start);
        substring[count] = '\0';
        return substring;
    }

    bool empty() {
        bool give = false;
        if (len == 0) give = true;
        return give;
    }

    void clear() {
        delete[] str;
        len = 0;
        str = new char[2]; str[0] = '\0'; str[1] = '\0';
        memory_size = 2;
    }

    /// Functions
    void swap(String& swapString) {
        std::swap(len, swapString.len);
        std::swap(str, swapString.str);
        std::swap(memory_size, swapString.memory_size);
    }

    friend std::ostream& operator<<(std::ostream& out, const String& string);
    friend std::istream& operator>>(std::istream& in, String& string);
    friend bool operator==(const String&, const String&);
};

bool operator==(const String& first, const String& second) {
    bool give = false;
    if (first.len == second.len) {
        give = true;
        for (size_t i = 0; i < first.len; i++) {
            if (first[i] != second[i])
                give = false;
        }
    }
    return give;
}

String operator+(const String& first, const String& second) {
    String temp(first);
    temp += second;
    return temp;
}

std::ostream& operator<<(std::ostream& out, const String& outString) {
    if (outString.str[0] != '\0')
        out << outString.str;
    return out;
}
std::istream& operator>>(std::istream& in, String& inputString) {
    inputString.clear();
    char readingChar;
    while (in.get(readingChar) && !std::isspace(readingChar) && !std::iscntrl(readingChar))
        inputString.push_back(readingChar);
    return in;
}
