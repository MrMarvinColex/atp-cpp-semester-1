//
// Created by VladimirSlastin on 11.11.2021 - 25.11.2021.
//
#include <iostream>
#include <cstring>
class String {
 public:
  String () : capacity(1), sz(0), str(new char[capacity]) {} //Default constructor

  String (const char* string) : capacity(strlen(string) + 1),
  								sz(strlen(string)), str(new char[capacity]) { //Constructor by C-style string (const char*).
	  memcpy(str, string, strlen(string));
	  str[sz] = '\0';
  }

  String (size_t n, char c) : capacity(n + 1), sz(n), str(new char[capacity]) {
	  // Constructor which makes string from n copies of character c
	  memset(str, c, n);
	  str[sz] = '\0';
  }

  String (const String& string) : capacity(string.capacity), sz(string.sz), str(new char[string.capacity]) {
	  memcpy(str, string.str, sz);
	  str[sz] = '\0';
  }

  explicit String (size_t n) : capacity(n), sz(0), str(new char[capacity]) {}
  // Constructor for emptystring with concrete memory
  void push_back (const char& c) { // add character c to back (if needed -> extend capacity)
	  if (sz > (capacity - 2) || sz == 0) {
		  capacity *= 2;
		  rebuild();
	  }
	  str[sz++] = c;
	  str[sz] = '\0';
  }

  void pop_back () { // if possible remove last character else do nothing
	  if (sz) {
		  if (sz * 4 + 1 <= capacity) {
			  capacity /= 2;
			  rebuild();
		  }
		  str[--sz] = '\0';
	  }
  }

  void clear () { // clear our String object
	  delete[] str;
	  sz = 0;
	  capacity = 1;
	  str = new char[1];
  }

  [[nodiscard]] size_t length () const { // return length of our string
	  return sz;
  }

  char& front () { // get front character of the string
	  return *str;
  }

  [[nodiscard]] char front () const { // get front character of the const string
	  return *str;
  }

  char& back () { // get back character of the string
	  return *(str + sz - 1);
  }

  [[nodiscard]] char back () const { // get back character of the const string
	  return *(str + sz - 1);
  }

  [[nodiscard]] bool empty () const {
	  return sz == 0;
  }

  [[nodiscard]] size_t find (const String& string, bool is_reversed = false) const {
	  bool is_find = false;
	  size_t i;
	  size_t count;
	  if (is_reversed) {
		  i = sz - string.sz - 1;
		  for (; i + 1; --i) {
			  count = 0;
			  for (size_t j = i; j < i + string.sz; ++j) {
				  if (str[j] == string.str[j - i])
					  ++count;
				  else
					  break;
			  }
			  if (count == string.sz) {
				  is_find = true;
				  break;
			  }
		  }
	  } else {
		  i = 0;
		  for (; i < sz - string.sz; ++i) {
			  count = 0;
			  for (size_t j = i; j < i + string.sz; ++j) {
				  if (str[j] == string.str[j - i])
					  ++count;
				  else
					  break;
			  }
			  if (count == string.sz) {
				  is_find = true;
				  break;
			  }
		  }
	  }
	  return is_find ? i : sz;
  }

  [[nodiscard]] size_t find (const char* c) const {
	  String tmp(c);
	  return find(tmp);
  }
  [[nodiscard]] size_t rfind (const String& string) const {
	  return find(string, true);
  }

  [[nodiscard]] size_t rfind (const char* c) const {
	  String tmp(c);
	  return rfind(tmp);
  }

  [[nodiscard]] String substr (int start, int count) const {
	  // get substring of our string from start index for count characters
	  String tmp(count * 2);
	  memcpy(tmp.str, str + start, count);
	  tmp.sz = count;
	  //tmp.push_back('\0');
	  tmp.str[count] = '\0';
	  return tmp;
  }

  String& operator = (const String& other) { // our string <- our string, Copy-and-swap idiom++
	  if (this != &other) {
		  delete[] str;
		  str = new char[other.capacity];
		  memcpy(str, other.str, other.sz);
		  sz = other.sz;
		  capacity = other.capacity;
		  str[sz] = '\0';
	  }
	  return *this;
  }

  String& operator = (const char* c) { // our string <- char or array of chars
	  if (str != c) {
		  delete[] str;
		  str = new char[2];
		  capacity = 2;
		  sz = 0;
		  str[sz++] = *c;
	  }
	  return *this;
  }

  char& operator [] (const size_t& i) {
	  return *(str + i);
  }

  const char& operator [] (const size_t& i) const {
	  return *(str + i);
  }

  String& operator += (const char c) {
	  this->push_back(c);
	  return *this;
  }

  String& operator += (const String& string) {
	  String tmp(sz + string.sz);
	  memcpy(tmp.str, str, sz);
	  memcpy(tmp.str + sz, string.str, string.sz);
	  tmp.sz = sz + string.sz;
	  tmp.capacity = 2 * tmp.sz;
	  swap(tmp);
	  tmp.str[tmp.sz] = '\0';
	  return *this;
  }

  String operator + (const char& c) const {
	  String tmp;
	  tmp = *this;
	  tmp += c;
	  return tmp;
  }

  String operator + (const String& other) const {
	  String tmp;
	  tmp = *this;
	  tmp += other;
	  return tmp;
  }

  ~String () { // Destructor for String objects
	  delete[] str;
  }
  
 private:
  size_t capacity;
  size_t sz;
  char* str;

  void swap (String& string) { // additional function, swap for Copy-and-swap idiom
	  std::swap(str, string.str);
	  std::swap(sz, string.sz);
	  std::swap(capacity, string.capacity);
  }

  void rebuild () {
	  char* tmp = new char[capacity];
	  memcpy(tmp, str, sz);
	  std::swap(tmp, str);
	  delete[] tmp;
  }
};
bool operator == (const String& s1, const String& s2) {
	bool flag = false;
	if (s1.length() == s2.length()) {
		size_t i = 0;
		while (s1[i] == s2[i] && i < s1.length())
			++i;
		if (i == s1.length())
			flag = true;
	}
	return flag;
}

String operator + (const char c, const String& string) {
	String tmp;
	tmp += c;
	tmp += string;
	return tmp;
}

std::ostream& operator << (std::ostream& out, const String& string) {
	for (size_t i = 0; i < string.length(); ++i) {
		out << string[i];
	}
	return out;
}

std::istream& operator >> (std::istream& in, String& string) {
	char c;
	c = static_cast<char>(in.get());
	while (!std::isspace(c) && !in.eof()) {
		string.push_back(c);
		c = static_cast<char>(in.get());
	}
	return in;
}