#include <iostream>
#include <vector>
#include <string>

class BigInteger {
 private:
  const int base = 1e+09;
  bool isMinus;
  int size;
  std::vector<long long> number;   //number = v[0] * base^0 + v[1] * base^1 + ...

  [[nodiscard]] bool is_not_greater_than(const BigInteger& x) const {
    //return true if |*this| <= |x|
    if (size != x.size)
      return size < x.size;
    for (int i = size - 1; i >= 0; --i) {
      if (number[i] != x.number[i])
        return number[i] < x.number[i];
    }
    return true;
  }

  void resize() {
    // delete zeros in the beginning
    while ((size > 1) && (number[size - 1] == 0))
      --size;
    if ((size == 1) && (number[0] == 0))
      isMinus = false;
    number.resize(size);
  }
 public:
  BigInteger(int x): isMinus(x < 0), size(std::abs(x) / base + 1), number() {
    x = std::abs(x);
    for (int i = 0; i < size; ++i) {
      number.push_back(x % base);
      x /= base;
    }
  }

  BigInteger(): isMinus(false), size(1), number({0}) {}

  BigInteger(const BigInteger& x): isMinus(x.isMinus), size(x.size), number(x.number) {}

  [[nodiscard]] std::string toString() const{
    std::string str;
    if (isMinus)
      str += '-';
    str += std::to_string(number[size - 1]);
    for (int i = size - 2; i >= 0; --i) {
      int j = 10;
      while ((j < base) && (number[i] / (base / j) == 0)) {
        str += '0';
        j *= 10;
      }
      str += std::to_string(number[i]);
    }
    return str;
  }

  [[nodiscard]] BigInteger abs() const {
    return ((*this < 0) ? -*this : *this);
  }

  BigInteger& operator=(const BigInteger& x) {
    isMinus = x.isMinus;
    size = x.size;
    number = x.number;
    return *this;
  }

  bool operator<=(const BigInteger& x) const {
    if (isMinus xor x.isMinus)
      return isMinus;
    return (isMinus ? !is_not_greater_than(x) : is_not_greater_than(x));
  }

  bool operator>=(const BigInteger& x) const {
    return (x <= *this);
  }

  bool operator==(const BigInteger& x) const {
    return ((x <= *this) && (*this <= x));
  }

  bool operator<(const BigInteger& x) const {
    return !(x <= *this);
  }

  bool operator>(const BigInteger& x) const {
    return !(*this <= x);
  }

  bool operator!=(const BigInteger& x) const {
    return ((x <= *this) xor (*this <= x));
  }

  BigInteger& operator+=(const BigInteger& x) {
    if (!(isMinus xor x.isMinus)) {
      long long extra = 0;
      int max_size = std::max(size, x.size) + 1;
      for (int i = 0; i < max_size; ++i) {
        if (i >= size) {
          ++size;
          number.push_back(0);
        }
        long long x1 = number[i];
        long long x2 = (i < x.size) ? x.number[i] : 0;
        number[i] = (x1 + x2 + extra) % base;
        extra = (x1 + x2 + extra) / base;
        if ((i >= x.size - 1) && (extra == 0))
          break;
      }

    } else if (is_not_greater_than(x)) {
      isMinus = !isMinus;
      long long debt = 0;
      for (int i = 0; i < x.size; ++i) {
        if (i >= size) {
          ++size;
          number.push_back(0);
        }
        if (x.number[i] >= number[i] + debt) {
          number[i] = x.number[i] - number[i] - debt;
          debt = 0;
        } else {
          number[i] = base + x.number[i] - number[i] - debt;
          debt = 1;
        }
      }

    } else {
      long long debt = 0;
      for (int i = 0; i < size; ++i) {
        if ((i >= x.size) && (debt == 0))
          break;
        long long x2 = (i < x.size) ? x.number[i] : 0;
        if (number[i] >= x2 + debt) {
          number[i] = number[i] - x2 - debt;
          debt = 0;
        } else {
          number[i] = base + number[i] - x2 - debt;
          debt = 1;
        }
      }
    }

    resize();
    return *this;
  }

  BigInteger& operator-=(const BigInteger& x) {
    if (isMinus xor x.isMinus) {
      long long extra = 0;
      int max_size = std::max(size, x.size) + 1;
      for (int i = 0; i < max_size; ++i) {
        if (i >= size) {
          ++size;
          number.push_back(0);
        }
        long long x1 = number[i];
        long long x2 = (i < x.size) ? x.number[i] : 0;
        number[i] = (x1 + x2 + extra) % base;
        extra = (x1 + x2 + extra) / base;
        if ((i >= x.size - 1) && (extra == 0))
          break;
      }

    } else if (is_not_greater_than(x)) {
      isMinus = !isMinus;
      long long debt = 0;
      for (int i = 0; i < x.size; ++i) {
        if (i >= size) {
          ++size;
          number.push_back(0);
        }
        if (x.number[i] >= number[i] + debt) {
          number[i] = x.number[i] - number[i] - debt;
          debt = 0;
        } else {
          number[i] = base + x.number[i] - number[i] - debt;
          debt = 1;
        }
      }

    } else {
      long long debt = 0;
      for (int i = 0; i < size; ++i) {
        if ((i >= x.size) && (debt == 0))
          break;
        long long x2 = (i < x.size) ? x.number[i] : 0;
        if (number[i] >= x2 + debt) {
          number[i] = number[i] - x2 - debt;
          debt = 0;
        } else {
          number[i] = base + number[i] - x2 - debt;
          debt = 1;
        }
      }
    }

    resize();
    return *this;
  }

  BigInteger& operator*=(const BigInteger& x) {
    isMinus = isMinus xor x.isMinus;
    std::vector<long long> new_number(size + x.size);
    for (int i = 0; i < x.size; ++i) {
      for (int j = 0; j < size; ++j) {
        new_number[i + j] += number[j] * x.number[i];
        new_number[i + j + 1] += new_number[i + j] / base;
        new_number[i + j] %= base;
      }
    }

    number = new_number;
    size = static_cast<int>(number.size());
    resize();
    return *this;
  }

  BigInteger& operator/=(const BigInteger& x) {
    bool minus = isMinus xor x.isMinus;
    isMinus = false;

    BigInteger divider = (x < 0 ? -x : x);
    BigInteger part_of_divided = 0;
    int digit_counter = size - 1;
    while (part_of_divided < divider) {
      part_of_divided *= base;
      part_of_divided += static_cast<int>(number[digit_counter--]);
    }
    BigInteger answer = 0;

    while (digit_counter >= -1) {
      int left = 0;
      int right = base;
      while (left != right - 1) {
        int middle = (left + right) / 2;
        if (divider * middle <= part_of_divided)
          left = middle;
        else
          right = middle;
      }
      answer *= base;
      answer += left;
      if (digit_counter != -1) {
        part_of_divided -= divider * left;
        part_of_divided *= base;
        part_of_divided += static_cast<int>(number[digit_counter]);
      }
      --digit_counter;
    }

    *this = answer;
    isMinus = minus;
    resize();
    return *this;
  }

  BigInteger& operator%=(const BigInteger& x) {
    return (*this = (*this - x * (*this / x)));
  }

  BigInteger operator+(const BigInteger& x1) const {
    BigInteger x(*this);
    return (x += x1);
  }

  BigInteger operator*(const BigInteger& x1) const {
    BigInteger x(*this);
    return (x *= x1);
  }

  BigInteger operator-(const BigInteger& x1) const {
    BigInteger x(*this);
    return (x -= x1);
  }

  BigInteger operator/(const BigInteger& x1) const {
    BigInteger x(*this);
    return (x /= x1);
  }

  BigInteger operator%(const BigInteger& x1) const {
    BigInteger x(*this);
    return (x %= x1);
  }

  BigInteger operator+() const {
    return *this;
  }

  BigInteger operator-() const {
    BigInteger r(0);
    return (r -= *this);
  }

  BigInteger& operator++() {
    return (*this += 1);
  }

  BigInteger operator++(int) {
    BigInteger x = *this;
    *this += 1;
    return x;
  }

  BigInteger& operator--() {
    return (*this -= 1);
  }

  BigInteger operator--(int) {
    BigInteger x = *this;
    *this -= 1;
    return x;
  }

  explicit operator bool() const {
    return !(*this == 0);
  }

  ~BigInteger() = default;
};

BigInteger operator+(int x1, const BigInteger& x2) {
  BigInteger x(x1);
  return (x += x2);
}

BigInteger operator*(int x1, const BigInteger& x2) {
  BigInteger x(x1);
  return (x *= x2);
}

BigInteger operator-(int x1, const BigInteger& x2) {
  BigInteger x(x1);
  return (x -= x2);
}

BigInteger operator/(int x1, const BigInteger& x2) {
  BigInteger x(x1);
  return (x /= x2);
}

BigInteger operator%(int x1, const BigInteger& x2) {
  BigInteger x(x1);
  return (x %= x2);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
  out << x.toString();
  return out;
}

std::istream& operator>>(std::istream& in, BigInteger& x) {
  x = 0;
  bool minus = false;
  char c;
  in.get(c);
  while (std::isspace(c))
    in.get(c);
  if (c == '-') {
    minus = true;
    in.get(c);
  }
  while (!std::isspace(c) && !in.eof()) {
    x *= 10;
    x += (c - '0');
    in.get(c);
  }
  if (minus)
    x *= -1;
  return in;
}



class Rational {
 private:
  BigInteger num;
  BigInteger denom;

  void short_rational() {
    BigInteger n = num.abs();
    BigInteger d = denom.abs();
    while ((n != 0) && (d != 0)) {
      if (n >= d)
        n %= d;
      else
        d %= n;
    }
    BigInteger gcd = n + d;
    if (gcd != 0) {
      num /= gcd;
      denom /= gcd;
    } else {
      denom = 1;
    }

    if (denom < 0) {
      num = -num;
      denom = -denom;
    }
  }

 public:
  Rational(const BigInteger& x): num(x), denom(1) {}

  Rational(int x): num(x), denom(1) {}

  Rational(): num(0), denom(1) {}

  [[nodiscard]] std::string toString() const{
    std::string str;
    str += num.toString();
    if (denom != 1) {
      str += '/';
      str += denom.toString();
    }
    return str;
  }

  [[nodiscard]] std::string asDecimal(size_t precision=0) const {
    std::string str1;
    BigInteger num1 = num.abs();
    bool minus = (num < 0);
    for (size_t i = 0; i < precision; ++i)
      num1 *= 10;
    str1 = (num1 / denom).toString();
    if (str1.size() < precision + 1)
      str1 = std::string(precision + 1 - str1.size(), '0') + str1;

    std::string str = str1.substr(0, str1.size() - precision);
    if (precision != 0) {
      str += '.';
      str += str1.substr(str1.size() - precision, precision);
    }
    if (minus)
      str = '-' + str;
    return str;
  }

  Rational& operator=(const Rational& x) = default;

  Rational operator+() const {
    return *this;
  }

  Rational operator-() const {
    Rational r(0);
    return (r -= *this);
  }

  Rational& operator+=(const Rational& x) {
    num *= x.denom;
    num += x.num * denom;
    denom *= x.denom;
    short_rational();
    return *this;
  }

  Rational& operator-=(const Rational& x) {
    num *= x.denom;
    num -= x.num * denom;
    denom *= x.denom;
    short_rational();
    return *this;
  }

  Rational& operator*=(const Rational& x) {
    num *= x.num;
    denom *= x.denom;
    short_rational();
    return *this;
  }

  Rational& operator/=(const Rational& x) {
    num *= x.denom;
    denom *= x.num;
    short_rational();
    return *this;
  }

  bool operator<=(const Rational& x) const {
    BigInteger num1 = num * x.denom;
    BigInteger num2 = x.num * denom;
    return (num1 <= num2);
  }

  bool operator>=(const Rational& x) const {
    return (x <= *this);
  }

  bool operator==(const Rational& x) const {
    return ((*this <= x) && (x <= *this));
  }

  bool operator!=(const Rational& x) const {
    return ((*this <= x) xor (x <= *this));
  }

  bool operator<(const Rational& x) const {
    return !(x <= *this);
  }

  bool operator>(const Rational& x) const {
    return !(*this <= x);
  }

  explicit operator double() const {
    // what return if rational overflow of double ???
    return 1;
  }

  friend Rational operator+(const Rational& r1, const Rational& r2);
  friend Rational operator-(const Rational& r1, const Rational& r2);
  friend Rational operator*(const Rational& r1, const Rational& r2);
  friend Rational operator/(const Rational& r1, const Rational& r2);

  ~Rational() = default;
};

Rational operator+(const Rational& r1, const Rational& r2) {
  Rational r = r1;
  r += r2;
  r.short_rational();
  return r;
}

Rational operator-(const Rational& r1, const Rational& r2) {
  Rational r = r1;
  r -= r2;
  r.short_rational();
  return r;
}

Rational operator*(const Rational& r1, const Rational& r2) {
  Rational r = r1;
  r *= r2;
  r.short_rational();
  return r;
}

Rational operator/(const Rational& r1, const Rational& r2) {
  Rational r = r1;
  r /= r2;
  r.short_rational();
  return r;
}
