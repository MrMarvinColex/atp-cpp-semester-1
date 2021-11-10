#include <iostream>
#include <cstring>

size_t max(size_t value1, size_t value2) {
  return (value1 < value2) ? value2 : value1;
}

class String {
 private:
  size_t stringLength;
  const size_t MIN_CAPACITY = 15;
  size_t capacity;
  char* charArray;

  [[nodiscard]] size_t commonFind(const String& anotherString, bool reverse) const {
    if (anotherString.length() > stringLength) {
      return stringLength;
    }
    for (size_t i = 0; i < stringLength - anotherString.length() + 1; ++i) {
      bool isEqual = true;
      for (size_t j = 0; j < anotherString.length(); ++j) {
        if ((!reverse && anotherString[j] != charArray[i + j]) ||
            (reverse && anotherString[anotherString.length() - 1 - j] != charArray[stringLength - 1 - i - j])) {
          isEqual = false;
          break;
        }
      }
      if (isEqual) {
        return reverse ? stringLength - i - anotherString.length() : i;
      }
    }
    return stringLength;
  }
  void resize(size_t newCapacity) {
    capacity = newCapacity;
    char* copy = charArray;
    charArray = new char[newCapacity];
    memcpy(charArray, copy, stringLength);
    delete[] copy;
  }

 public:
  String(const char* initArray) : stringLength(strlen(initArray)),
                                  capacity(max(stringLength * 2, MIN_CAPACITY)), charArray(new char[capacity]) {
    memcpy(charArray, initArray, stringLength);
  }
  explicit String(size_t n, char c) : stringLength(n),
                                      capacity(max(stringLength * 2, MIN_CAPACITY)), charArray(new char[capacity]) {
    memset(charArray, c, stringLength);
  }
  explicit String() : stringLength(0), capacity(MIN_CAPACITY), charArray(new char[capacity]) {}
  String(const String& anotherString) : stringLength(anotherString.length()),
                                        capacity(max(anotherString.capacity, MIN_CAPACITY)),
                                        charArray(new char[capacity]) {
    memcpy(charArray, anotherString.charArray, stringLength);
  }
  ~String() {
    delete[] charArray;
  }
  String& operator=(const String& anotherString) {
    if (&anotherString == this) {
      return *this;
    }
    stringLength = anotherString.stringLength;
    if (stringLength >= capacity) {
      delete[] charArray;
      capacity = max(capacity * 2, stringLength) + 1;
      charArray = new char[capacity];
    }
    memcpy(charArray, anotherString.charArray, stringLength);
    return *this;
  }

  const char& operator[](const size_t index) const {
    return charArray[index];
  }
  char& operator[](size_t index) {
    return charArray[index];
  }

  [[nodiscard]] size_t length() const {
    return stringLength;
  }
  [[nodiscard]] bool empty() const {
    return stringLength == 0;
  }

  char& front() {
    return charArray[0];
  }
  [[nodiscard]] const char& front() const {

    return charArray[0];
  }
  char& back() {
    return charArray[stringLength - 1];
  }
  [[nodiscard]] const char& back() const {

    return charArray[stringLength - 1];
  }

  [[nodiscard]] size_t find(const String& anotherString) const {

    return commonFind(anotherString, false);
  }
  [[nodiscard]] size_t rfind(const String& anotherString) const {
    return commonFind(anotherString, true);
  }
  [[nodiscard]] String substr(size_t start, int count) const {
    String subString(count, 'o');
    for (size_t i = start; i < start + count; ++i) {
      subString[i - start] = charArray[i];
    }
    return subString;
  }

  void push_back(char element) {
    if (stringLength + 1 >= capacity) {
      resize(max(capacity * 2 + 1, stringLength + 2));
    }
    charArray[stringLength] = element;
    ++stringLength;
  }
  void pop_back() {
    if (stringLength > 0) {
      --stringLength;
    }
  }
  void clear() {
    stringLength = 0;
  }
  String& operator+=(char element) {
    push_back(element);
    return *this;
  }
  String& operator+=(const String& anotherString) {
    if (stringLength + anotherString.length() >= capacity) {
      resize((stringLength + anotherString.length()) * 2 + 1);
    }
    size_t end = anotherString.length();
    for (size_t i = 0; i < end; ++i) {
      push_back(anotherString[i]);
    }

    return *this;
  }

  friend std::istream& operator>>(std::istream& in, String& string);
  friend std::ostream& operator<<(std::ostream& out, const String& string);
};

String operator+(const String& string, const char element) {
  String copyString = string;
  copyString += element;
  return copyString;
}
String operator+(const char element, const String& string) {
  String copyString(1, element);
  copyString += string;
  return copyString;
}
String operator+(const String& string1, const String& string2) {
  String copyString(string1);
  copyString += string2;
  return copyString;
}

bool operator==(const String& s1, const String& s2) {
  if (s1.length() != s2.length()) {
    return false;
  }
  for (size_t i = 0; i < s1.length(); ++i) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const String& string) {
  for (size_t i = 0; i < string.stringLength; ++i) {
    out << string.charArray[i];
  }
  return out;
}
std::istream& operator>>(std::istream& in, String& string) {
  string.clear();
  char inputChar;
  while (in.get(inputChar) && !in.eof() && !std::isspace(inputChar)) {
    string.push_back(inputChar);
  }
  return in;
}
