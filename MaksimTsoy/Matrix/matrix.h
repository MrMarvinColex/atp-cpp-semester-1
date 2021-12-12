#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
      long long left, right;
      if (part_of_divided.size - divider.size >= 2) {
        left = base - 1;
        right = base;
      } else if (part_of_divided.size - divider.size == 1) {
        left = base / (divider.number[divider.size - 1] + 1);
        right = base;
      } else if (part_of_divided.size == divider.size) {
        left = part_of_divided.number[part_of_divided.size - 1] / (divider.number[divider.size - 1] + 1);
        right = (part_of_divided.number[part_of_divided.size - 1] + 1) / divider.number[divider.size - 1] + 1;
      } else {
        left = 0;
        right = 1;
      }
      while (left < right - 1) {
        long long middle = (left + right) / 2;
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
    double d = 0;
    d = std::stod(asDecimal(16));
    return d;
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

std::ostream& operator<<(std::ostream& out, const Rational& x) {
  out << x.toString();
  return out;
}

std::istream& operator>>(std::istream& in, Rational& x) {
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



//----------------------------------------------------------------------------------------------------------------
//    RESIDUE




template <size_t N, size_t D, bool STOP = false>
struct IsPrimeHelper {
  static const bool value = (N % D != 0) && IsPrimeHelper<N, D + 1, D * D >= N>::value;
};

template <size_t N, size_t D>
struct IsPrimeHelper<N, D, true> {
  static const bool value = true;
};


template <size_t N>
class Residue {
 private:
  size_t value;

  Residue<N> find_reverse(Residue<N> r) const {
    Residue<N> result = 1;
    size_t power = N - 2;
    while (power) {
      if (power & 1)
        result *= r;
      r *= r;
      power >>= 1;
    }
    return result;
  }

 public:
  Residue(int x): value((x >= 0) ? (x % N) : ((N - -x % N) % N)) {}

  Residue(const Residue<N>& r): value(r.value) {}

  Residue(): value(0) {}

  Residue<N> operator+=(const Residue<N>& r) {
    value = (r.value + value) % N;
    return *this;
  }

  Residue<N>& operator-=(const Residue<N>& r) {
    value += N;
    value -= r.value;
    value %= N;
    return *this;
  }

  Residue<N>& operator*=(const Residue<N>& r) {
    value = (r.value * value) % N;
    return *this;
  }

  template <typename = typename std::enable_if<IsPrimeHelper<N, 2>::value>>
  Residue<N>& operator/=(const Residue<N>& r) {
    return (*this *= find_reverse(r));
  }

  bool operator==(const Residue<N>& r) const {
    return (value == r.value);
  }

  bool operator!=(const Residue<N>& r) const {
    return (value != r.value);
  }

  explicit operator int() const {
    return value;
  }

  ~Residue() = default;
};

template <size_t N>
Residue<N> operator+(const Residue<N>& r1, const Residue<N>& r2) {
  Residue<N> r(r1);
  return (r += r2);
}

template <size_t N>
Residue<N> operator-(const Residue<N>& r1, const Residue<N>& r2) {
  Residue<N> r(r1);
  return (r -= r2);
}

template <size_t N>
Residue<N> operator*(const Residue<N>& r1, const Residue<N>& r2) {
  Residue<N> r(r1);
  return (r *= r2);
}

template <size_t N>
Residue<N> operator/(const Residue<N>& r1, const Residue<N>& r2) {
  Residue<N> r(r1);
  return (r /= r2);
}



//----------------------------------------------------------------------------------------------------------------
//    MATRIX



template <size_t M, size_t N = M, typename Field = Rational>
class Matrix {
 private:
  std::vector<std::vector<Field>> matrix;

  std::pair<Matrix<M, N, Field>, bool> find_triangle_matrix() const {
    Matrix<M, N, Field> m = *this;
    bool det_sign = true;
    for (size_t i = 0; i < std::min(M, N); ++i) {
      size_t pivot_row = i;
      while ((pivot_row < M - 1) && (m[pivot_row][i] == 0))
        ++pivot_row;
      if (pivot_row != i) {
        std::swap(m[i], m[pivot_row]);
        det_sign = !det_sign;
      }

      if (m[i][i] != 0) {
        for (size_t j = i + 1; j < M; ++j) {
          Field del = m[j][i] / m[i][i];
          for (size_t k = i; k < N; ++k)
            m[j][k] -= del * m[i][k];
        }
      }
    }

    return {m, det_sign};
  }

 public:
  template <typename = typename std::enable_if<N == M>>
  Matrix(): matrix(std::vector(M, std::vector(M, Field(0)))) {
    for (size_t i = 0; i < M; ++i)
      matrix[i][i] = Field(1);
  }

  Matrix(const std::vector<std::vector<Field>>& v): matrix(v) {}

  Matrix(std::initializer_list<std::initializer_list<int>> v): matrix(std::vector(M, std::vector(N, Field()))) {
    size_t row = 0;
    for (auto i : v) {
      size_t col = 0;
      for (auto j = i.begin(); j != i.end(); ++j)
        matrix[row][col++] = Field(*j);
      ++row;
    }
  }

  Matrix(const Field& x): matrix(std::vector(M, std::vector(N, x))) {}

  ~Matrix() = default;

  Matrix<M, N, Field>& operator=(const Matrix<M, N, Field>& m) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j)
        matrix[i][j] = m[i][j];
    }
    return *this;
  }

  Matrix<M, N, Field>& operator+=(const Matrix<M, N, Field>& m) {
    for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j)
        matrix[i][j] += m[i][j];
    return *this;
  }

  Matrix<M, N, Field>& operator-=(const Matrix<M, N, Field>& m) {
    for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j)
        matrix[i][j] -= m[i][j];
    return *this;
  }

  Matrix<M, N, Field>& operator*=(const Field& x) {
    for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j)
        matrix[i][j] *= x;
    return *this;
  }

  template <typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field>& operator*=(const Matrix<M, N, Field>& m1) {
    Matrix<M, N, Field> m = *this;
    *this = m * m1;
    return *this;
  }

  std::vector<Field>& operator[](size_t ind) {
    return matrix[ind];
  }

  std::vector<Field> operator[](size_t ind) const {
    return matrix[ind];
  }

  bool operator==(const Matrix<M, N, Field>& m) const {
    for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j)
        if (matrix[i][j] != m[i][j])
          return false;
    return true;
  }

  bool operator!=(const Matrix<M, N, Field>& m) const {
    return !(*this == m);
  }

  template <typename = typename std::enable_if<N == M>>
  Field det() const {
    std::pair<Matrix<M, N, Field>, bool> pair = find_triangle_matrix();
    Field d = Field(pair.second ? 1 : -1);
    for (size_t i = 0; i < M; ++i)
      d *= pair.first[i][i];
    return d;
  }

  Matrix<N, M, Field> transposed() const {
    std::vector<std::vector<Field>> m(N, std::vector(M, Field()));
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j)
        m[i][j] = matrix[j][i];
    }
    Matrix<N, M, Field> matrix1(m);
    return matrix1;
  }

  template <typename = typename std::enable_if<N == M>>
  size_t rank() const {
    std::pair<Matrix<M, N, Field>, bool> pair = find_triangle_matrix();
    size_t r = 0;
    size_t col = 0;
    while ((col != N) && (r != M)) {
      if (pair.first[r][col] != 0)
        ++r;
      ++col;
    }
    return r;
  }

  template <typename = typename std::enable_if<N == M>>
  Field trace() const {
    Field tr(0);
    for (size_t i = 0; i < N; ++i)
      tr += matrix[i][i];
    return tr;
  }

  std::vector<Field> getRow(size_t row) const {
    return matrix[row];
  }

  std::vector<Field> getColumn(size_t col) const {
    std::vector<Field> v(M);
    for (size_t i = 0; i < M; ++i)
      v[i] = matrix[i][col];
    return v;
  }

  template <typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field>& invert() {
    Matrix<M, M, Field> E;
    for (size_t i = 0; i < M; ++i) {
      size_t pivot_row = i;
      while ((pivot_row < M - 1) && (matrix[pivot_row][i] == Field(0))) {
        ++pivot_row;
      }
      if (pivot_row != i) {
        std::swap(matrix[i], matrix[pivot_row]);
        std::swap(E[i], E[pivot_row]);
      }

      if (matrix[i][i] != Field(0)) {
        Field multiply = matrix[i][i];
        for (size_t j = 0; j < M; ++j) {
          matrix[i][j] /= multiply;
          E[i][j] /= multiply;
        }

        for (size_t j = i + 1; j < M; ++j) {
          Field del = matrix[j][i] / matrix[i][i];
          for (size_t k = 0; k < N; ++k) {
            matrix[j][k] -= del * matrix[i][k];
            E[j][k] -= del * E[i][k];
          }
        }
      } else {
        return *this;
      }
    }
    for (size_t j = M - 1; j >= 1; --j) {
      for (size_t k = 0; k < j; ++k) {
        Field multiply = matrix[k][j];
        matrix[k][j] = Field(0);
        for (size_t l = 0; l < M; ++l) {
          E[k][l] -= multiply * E[j][l];
        }
      }
    }

    *this = E;
    return *this;
  }

  template <typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field> inverted() {
    Matrix<M, N, Field> inv = *this;
    return inv.invert();
  }
};

template <size_t M, typename Field = Rational>
using SquareMatrix = Matrix<M, M, Field>;

template <size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2) {
  Matrix<M, N, Field> m = m1;
  return (m += m2);
}

template <size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2) {
  Matrix<M, N, Field> m = m1;
  return (m -= m2);
}

template <size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator*(const Field& x, const Matrix<M, N, Field>& m1) {
  Matrix<M, N, Field> m = m1;
  return (m *= x);
}

template <size_t M, size_t N, size_t K, typename Field = Rational>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& m1, const Matrix<N, K, Field>& m2) {
  Matrix<M, K, Field> m(Field(0));
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < K; ++j)
      for (size_t k = 0; k < N; ++k)
        m[i][j] += m1[i][k] * m2[k][j];
  }
  return m;
}

