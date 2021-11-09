//
// Created by vsvood on 11/8/21.
//

#ifndef STRING_STRING_H
#define STRING_STRING_H

#include <cstring>
#include <iostream>

class String {
 public:
  String();
  String(const char* source);
  String(size_t length, char filler = ' ');
  String(const String& source);
  ~String();

  bool operator==(const String& other) const;

  String& operator=(const String& source);
  char& operator[](size_t index);
  const char& operator[](size_t index) const;

  [[nodiscard]] size_t length() const;

  void push_back(char chr);
  void pop_back();

  char& front();
  char& back();
  [[nodiscard]] const char& front() const;
  [[nodiscard]] const char& back() const;

  String& operator+=(char chr);
  String& operator+=(const String& str);

  [[nodiscard]] const char* c_str() const;

  size_t find(const char* substr) const;
  size_t rfind(const char* substr) const;
  [[nodiscard]] String substr(size_t start, size_t count) const;

  [[nodiscard]] bool empty() const;
  void clear();

 private:
  size_t length_;
  size_t capacity_;

  char* string_;
};

String operator+(const String& str, char chr);
String operator+(char chr, const String& str);
String operator+(const String& str1, const String& str2);
std::ostream& operator<<(std::ostream& out, const String& str);
std::istream& operator>>(std::istream& in, String& str);

String::String() : length_(0), capacity_(0), string_(new char[1]) {
  string_[0] = '\0';
}

String::String(const char* source)
    : length_(strlen(source)),
      capacity_(strlen(source)),
      string_(new char[capacity_ + 1]) {
  strcpy(string_, source);
}

String::String(size_t length, char filler)
    : length_(length), capacity_(length), string_(new char[capacity_ + 1]) {
  memset(string_, filler, length_);
  string_[length_] = '\0';
}

String::String(const String& source) : String() { *this = source; }

String::~String() { delete[] string_; }

bool String::operator==(const String& other) const {
  return strcmp(string_, other.string_) == 0;
}

String& String::operator=(const String& source) {
  if (&source == this) return *this;
  clear();
  length_ = source.length_;

  if (length_ > capacity_) {
    capacity_ = std::max(capacity_ * 2, length_);
    delete[] string_;
    string_ = new char[capacity_ + 1];
  }

  strcpy(string_, source.string_);

  return *this;
}

char& String::operator[](size_t index) { return string_[index]; }

const char& String::operator[](size_t index) const { return string_[index]; }

size_t String::length() const {
  if (length_ == 12) {
    fprintf(stderr, "'%s'\n", string_);
    return strlen(string_);
  }
  return length_;
}

void String::push_back(char chr) {
  if (length_ >= capacity_) {
    capacity_ = std::max(capacity_ * 2, length_ + 1);
    char* tmp = new char[capacity_ + 1];
    strcpy(tmp, string_);
    delete[] string_;
    string_ = tmp;
  }
  string_[length_] = chr;
  ++length_;
  string_[length_] = '\0';
}

void String::pop_back() {
  --length_;
  string_[length_] = '\0';
}

char& String::front() { return string_[0]; }

char& String::back() { return string_[length_ - 1]; }

const char& String::front() const { return string_[0]; }

const char& String::back() const { return string_[length_ - 1]; }

String& String::operator+=(char chr) {
  this->push_back(chr);
  return *this;
}

String& String::operator+=(const String& str) {
  size_t total_length = length_ + str.length_;
  if (capacity_ < total_length) {
    capacity_ = std::max(capacity_ * 2, total_length);
    char* tmp = new char[capacity_ + 1];
    strcpy(tmp, string_);
    delete[] string_;
    string_ = tmp;
  }

  char* tmp = new char[str.length_ + 1];
  strcpy(tmp, str.string_);
  strcat(string_, tmp);
  delete[] tmp;

  length_ = total_length;

  return *this;
}

const char* String::c_str() const { return string_; }

size_t String::find(const char* substr) const {
  char* occurrence_pointer = strstr(string_, substr);
  return (occurrence_pointer == nullptr)
             ? length_
             : static_cast<size_t>(occurrence_pointer - string_);
}

size_t String::rfind(const char* substr) const {
  char* occurrence_pointer = nullptr;
  char* search_pointer = string_;

  while (search_pointer <= &back() &&
         strstr(search_pointer, substr) != nullptr) {
    occurrence_pointer = strstr(search_pointer, substr);
    search_pointer = occurrence_pointer + strlen(substr);
  }

  return (occurrence_pointer == nullptr)
             ? length_
             : static_cast<size_t>(occurrence_pointer - string_);
}

String String::substr(size_t start, size_t count) const {
  String substr(count);
  memcpy(substr.string_, string_ + start, count);
  return substr;
}

bool String::empty() const { return length_ == 0; }

void String::clear() { length_ = 0; }

String operator+(const String& str, char chr) {
  String string(str);
  string += chr;
  return string;
}

String operator+(char chr, const String& str) {
  String string(1, chr);
  string += str;
  return string;
}

String operator+(const String& str1, const String& str2) {
  String string(str1);
  string += str2;
  return string;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.c_str();
  return out;
}

std::istream& operator>>(std::istream& in, String& str) {
  str.clear();
  char buffer;
  while (in.get(buffer) && !std::isspace(buffer) && !std::iscntrl(buffer)) {
    str.push_back(buffer);
  }
  return in;
}

#endif  // STRING_STRING_H
