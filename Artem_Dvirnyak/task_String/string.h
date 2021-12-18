#include <iostream>
#include <cstring>

class String {
public:
    String(const char* source);
    String(size_t n, char c);
    String();
    String(const String&);
    ~String();

    String& operator =(const String& source);
    String& operator =(const char* source);
    bool operator ==(String const &source) const;
    bool operator ==(const char* source) const;

    const char& operator [](size_t index) const;
    char& operator [](size_t index);

    String& operator +=(char source);
    String& operator +=(const String& source);

    [[nodiscard]] size_t length() const;
    void push_back(char c);

    void pop_back();
    char& front();
    char& back();
    [[nodiscard]] const char& front() const;
    [[nodiscard]] const char& back() const;

    size_t find(const char* substring) const;
    size_t rfind(const char* substring) const;

    [[nodiscard]] String substr(size_t start, size_t count) const;

    bool empty();
    void clear();
private:
    size_t size_;
    char* str_;
    size_t length_;
};

String operator +(const String& s1, char s2);
String operator +(const String& s1, const String& s2);
String operator +(char s1, const String& s2);

std::ostream& operator << (std::ostream& out, const String& str);
std::istream& operator >> (std::istream& in, String& str);

String::String(const char* source): size_(strlen(source)), str_(new char[size_]),
length_(size_) {
    memcpy(str_, source, length_);
}

String::String(size_t n, const char c): size_(n), str_(new char[n]), length_(n) {
    memset(str_, c, n);
}

String::String(): size_(16), str_(new char[size_]), length_(0) {
    str_[0] = '\0';
}

String::String(const String& source): size_(source.length()),
str_(new char[size_]), length_(size_)  {
    memcpy(str_, source.str_, length_);
}

String::~String() {
    delete[] str_;
}

const char& String::operator[](size_t index) const{
    return str_[index];
}

char& String::operator[](size_t index){
    return str_[index];
}

String& String::operator =(const String& source) {
    if (this == &source) {
        return *this;
    }
    length_ = source.length_;
    if (length_ >= size_) {
        delete[] str_;
        size_ = length_;
        str_ = new char[++size_];
    }
    memcpy(str_, source.str_, length_);
    return *this;
}

String& String::operator =(const char *source) {
    *this = String(source);
    return *this;
}

bool String::operator==(String const &source) const {
    return (strcmp(str_, source.str_) == 0);
}

bool String::operator==(const char* source) const {
    return (strcmp(str_, source) == 0);
}

String& String::operator +=(char source) {
    push_back(source);
    return *this;
}

String& String::operator +=(const String& source0) {
    String source = source0;
    if (size_ < length_ + source.length() + 1) {
        size_ = std::max(2 * size_, length_ + source.length() + 1);
        char* copy;
        std::swap(copy, str_);
        str_ = new char[size_];
        memcpy(str_, copy, length_);
        delete[] copy;
    }
    memcpy(&str_[length_], source.str_, source.length_);
    length_ += source.length_;
    return *this;
}

String operator +(const String& s1, const String& s2) {
    String copy1 = s1;
    return copy1 += s2;
}

String operator +(const String& s1, char s2) {
    String copy1 = s1;
    return copy1 += s2;
}

String operator +(char s1, const String& s2) {
    String copy1("");
    return copy1 + s1 += s2;
}

size_t String::length() const {
    return length_;
}

void String::push_back(char c) {
    if (length_ >= size_) {
        size_ = std::max((int)length_+1, (int)size_ * 2);
        char* copy;
        std::swap(copy, str_);
        str_ = new char[++size_];
        memcpy(str_, copy, length_);
        delete[] copy;
    }
    str_[length_++] = c;
}

void String::pop_back() {
    str_[length_--] = '\0';
    if (4 * length_ <= size_) {
        size_ = length_;
        char* copy;
        std::swap(copy, str_);
        str_ = new char[size_];
        memcpy(str_, copy, length_);
        delete[] copy;
    }
}

char& String::front() {
    return str_[0];
}

char& String::back() {
    return str_[length_-1];
}

const char& String::front() const {
    return str_[0];
}

const char& String::back() const {
    return str_[length_-1];
}

size_t String::find(const char* substring) const {
    char* pos = strstr(str_, substring);
    size_t i = (pos != nullptr) ? pos - &str_[0] : length_;
    return i;
}

size_t String::rfind(const char* substring) const {
    char* pos = strstr(str_, substring);
    char* prev;
    size_t i = 0;
    while(pos != nullptr && i < length_) {
        prev = pos;
        pos = strstr(&str_[i++], substring);
    }
    size_t j = (prev != nullptr) ? prev - &str_[0] : length_;
    return j;
}

String String::substr(size_t start, size_t count) const {
    char* result_chars = new char[count];
    strncpy(result_chars, &str_[start], count);
    String result = result_chars;
    delete[] result_chars;
    return result;
}

bool String::empty() {
    return (length_ == 0);
}

void String::clear() {
    *this = "";
}

std::ostream& operator << (std::ostream& out, const String& str) {
    for(size_t i = 0; i < str.length(); i++) {
        out << str[i];
    }
    return out;
}

std::istream& operator >> (std::istream& in, String& str) {
    char temp;
    str.clear();
    while (in.get(temp) && temp != ' ' && temp != '\n' && !in.eof()) {
        str.push_back(temp);
    }
    return in;
}
