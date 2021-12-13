#include <iostream>
#include <cstring>

class String {
 public:
  String(const char* c_string): len(strlen(c_string)), size(len + 1), str(new char[size]) {
    for (size_t i = 0; i < len; ++i)
      str[i] = c_string[i];
    str[len] = '\0';
  }

  String(size_t n, char c): len(n), size(len + 1), str(new char[size]) {
    memset(str, c, n);
    str[len] = '\0';
  }

  String(): len(0), size(1), str(new char[size]) {
    str[len] = '\0';
  }

  String(const String& string): len(string.len), size(string.size), str(new char[size]) {
    strcpy(str, string.str);
  }

  explicit String(size_t n): len(0), size(n + 1), str(new char[size]) {
    str[0] = '\0';
  }

  [[nodiscard]] size_t length() const {
    return len;
  }

  void push_back(char c) {
    if (len == size - 1)
      set_memory(size * 2);
    str[len++] = c;
    str[len] = '\0';
  }

  void pop_back() {
    str[--len] = '\0';
    if (len < size / 4)
      set_memory(size / 2);
  }

  char& front() {
    return str[0];
  }

  [[nodiscard]] char front() const {
    return str[0];
  }

  char& back() {
    return str[len - 1];
  }

  [[nodiscard]] char back() const {
    return str[len - 1];
  }

  [[nodiscard]] size_t find(const String& substr) const {
    for (size_t i = 0; i + substr.len <= len; ++i) {
      for (size_t j = 0; str[i + j] == substr.str[j]; ++j) {
        if (j == substr.len - 1)
          return i;
      }
    }
    return len;
  }

  [[nodiscard]] size_t rfind(const String& substr) const {
    for (size_t i = len - substr.len + 1; i > 0; --i) {
      for (size_t j = 0; str[i - 1 + j] == substr.str[j]; ++j) {
        if (j == substr.len - 1)
          return i - 1;
      }
    }
    return len;
  }

  [[nodiscard]] String substr(size_t start, size_t count) const {
    String string(count);
    for (size_t i = 0; i < count; ++i) {
      string.push_back(str[start + i]);
    }
    return string;
  }

  [[nodiscard]] bool empty() const {
    return len == 0;
  }

  void clear() {
    len = 0;
    str[len] = '\0';
  }

  char& operator[](size_t index) {
    return str[index];
  }

  char operator[](size_t index) const {
    return str[index];
  }

  String& operator=(const String& string) {
    if (this == &string)
      return *this;
    clear();
    if (string.len + 1 > size)
      set_memory(string.len + 1);
    len = string.len;
    for (size_t i = 0; i < len; ++i)
      str[i] = string.str[i];
    return *this;
  }

  bool operator==(const String& string) const {
    if (len != string.len)
      return false;
    for (size_t i = 0; i < len; ++i) {
      if (str[i] != string.str[i])
        return false;
    }
    return true;
  }

  String& operator+=(char c) {
    push_back(c);
    return *this;
  }

  String& operator+=(const String& string) {
    if (len + string.len >= size - 1)
      set_memory(len + string.len + 1);
    for (size_t i = 0; i < string.len; ++i)
      str[len++] = string.str[i];
    str[len] = '\0';
    return *this;
  }

  ~String() {
    delete[] str;
  }
 private:
  void set_memory(size_t new_size) {
    size = new_size;
    char* tmp = new char[size];
    for (size_t i = 0; i < len; ++i)
      tmp[i] = str[i];
    delete[] str;
    str = tmp;
  }

  friend std::ostream& operator<<(std::ostream& out, const String& string);

  size_t len;
  size_t size;
  char* str;
};

String operator+(const String& string, char c) {
  String str(string.length() + 1);
  str += string;
  str.push_back(c);
  return str;
}

String operator+(char c, const String& string) {
  String str(string.length() + 1);
  str.push_back(c);
  str += string;
  return str;
}

String operator+(const String& string1, const String& string2) {
  String str(string1.length() + string2.length());
  str += string1;
  str += string2;
  return str;
}

std::ostream& operator<<(std::ostream& out, const String& string) {
  out << string.str;
  return out;
}

std::istream& operator>>(std::istream& in, String& string) {
  string.clear();
  char c;
  in.get(c);
  while (!std::isspace(c) && !in.eof()) {
    string.push_back(c);
    in.get(c);
  }
  return in;
}

