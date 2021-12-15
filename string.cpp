#include <iostream>
#include <cstring>

class String {
public:

    /// Destructor
    ~String() {
        delete[] str;
    }

    /// Constructors
    String() {
        len = 0;
        str = new char[8];
        str[0] = '\0';
        memory_size = 8;
    }
    String(std::initializer_list<char> lst): len(lst.size()), str(new char[len * 2]), memory_size(len * 2) {
        std::copy(lst.begin(), lst.end(), str);
        str[len] = '\0';
    }
    String (const char* inputArray): len(strlen(inputArray)), str(new char[len * 2]), memory_size(len * 2){
        str[0] = '\0';
        strcat(str, inputArray);
    }
    String (size_t count, char inputChar): len(count), str(new char[count * 2]), memory_size(count * 2) {
        memset(str, inputChar, count);
        str[len] = '\0';
    }
    String (char unoChar): len(1), str(new char[8]), memory_size(8) {
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
        return mainFind(substring, false);
    }
    size_t rfind(const String substring) const {
        return mainFind(substring, true);
    }

    String substr(size_t start, size_t count) const {
        char* substring = new char[len * 2];
        substring[0] = '\0';
        strcat(substring, str + start);
        substring[count] = '\0';
        return substring;
    }

    bool empty() {
        return len == 0;
    }

    void clear() {
        delete[] str;
        len = 0;
        str = new char[2]; str[0] = '\0'; str[1] = '\0';
        memory_size = 2;
    }

    friend std::ostream& operator<<(std::ostream& out, const String& string);
    friend std::istream& operator>>(std::istream& in, String& string);
    friend bool operator==(const String&, const String&);

private:
    size_t len = 0;
    char* str;
    size_t memory_size = 2;


    /// Functions
    void swap(String& swapString) {
        std::swap(len, swapString.len);
        std::swap(str, swapString.str);
        std::swap(memory_size, swapString.memory_size);
    }

    void reverse(String& gnirts) const {
        String copy = gnirts;
        for (size_t i = 0; i < copy.len; i++) {
            gnirts[i] = copy[copy.len - i - 1];
        }
    }
    size_t mainFind(const String substring, bool isReverse) const {
        String copySubstring = substring;
        String copyOfThis = *this;
        if (isReverse) {
            reverse(copySubstring);
            reverse(copyOfThis);
        }
        size_t indexOfInclusion = len;
        for(size_t i = 0; i <= len - copySubstring.len; i++) {
            size_t delta = 0;
            while (copyOfThis.str[i + delta] == copySubstring[delta]) {
                if (delta == copySubstring.len - 1) indexOfInclusion = i;
                delta++;
            }
            if (indexOfInclusion == i) break;
        }
        if (indexOfInclusion == len)
            return len;
        else {
            return isReverse ? len - indexOfInclusion - copySubstring.len : indexOfInclusion;
        }
    }
    // Она почему-то не хочет работать! 
    String& updateMemorySize(String& copy, size_t newLen) {
        String temporaryString(copy);
        delete[] str;
        copy.memory_size = newLen * 2;
        copy.str = new char[copy.memory_size];
        copy.str[0] = '\0';
        strcat(str, temporaryString.str);
        return copy;
    }
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
