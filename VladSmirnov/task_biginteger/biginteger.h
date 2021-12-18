#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::cin;

class BigInteger {
private:
    const size_t COUNT = 9;
    const size_t SIZE = 1e9; // 10^COUNT
    bool is_positive_ = true;
    vector<size_t> number_;
    size_t size_ = 0;
    size_t rank_ = 0;
    
    void swap(BigInteger&);
    void fix_number();

public:
    BigInteger() {}
    BigInteger(int);
    explicit BigInteger(size_t size, size_t rank) : number_(size), size_(size), rank_(rank) {}
    BigInteger(const BigInteger& arg_number) : 
        is_positive_(arg_number.is_positive_), number_(arg_number.number_), size_(arg_number.size_), 
        rank_(arg_number.rank_) {}
    explicit BigInteger(size_t is_positive, const vector<size_t>& number, size_t size, size_t rank) :
        is_positive_(is_positive), number_(number), size_(size), rank_(rank) {}
    explicit BigInteger(const string&);
  
    operator int()const; // ВНИМАНИЕ! НЕЯВНОЕ ПРЕОБРАЗОВАНИЕ
    operator bool()const { return rank_ || number_[0]; } // ВНИМАНИЕ! НЕЯВНОЕ ПРЕОБРАЗОВАНИЕ
    string toString()const;

    friend BigInteger abs(const BigInteger&);
    friend BigInteger min(const BigInteger&, const BigInteger&);
    friend BigInteger max(const BigInteger&, const BigInteger&);

    BigInteger& operator=(const BigInteger&);
    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);

    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);    

    friend BigInteger operator-(const BigInteger&);
    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator==(const BigInteger&, const BigInteger&);
    
    // если не сделать их friend'ом, то они не будут вызываться???
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);

    friend std::ostream& operator<<(std::ostream&, const BigInteger&);
    friend std::istream& operator>>(std::istream&, const BigInteger&);
};

BigInteger::BigInteger(int arg_number) : is_positive_(arg_number >= 0), number_(2), size_(2), rank_(0) {
    arg_number = std::abs(arg_number);
    do {
        if (rank_ == size_) {
            size_ *= 2;
            number_.resize(size_);
        }
        number_[rank_] = arg_number % SIZE;
        arg_number /= SIZE;
        if (arg_number != 0) {
            ++rank_;
        }
    } while (arg_number != 0);
}

BigInteger::BigInteger(const string& string_number) : size_(2), rank_(0) {
    if (string_number.size() > 0) {
        is_positive_ = (string_number[0] != '-');
        string current_rank;
        size_t last_pos = string_number.size() - 1;
        while (last_pos >= COUNT) {
            last_pos -= COUNT;
            current_rank = string_number.substr(last_pos + 1, COUNT);
            number_.push_back(stoi(current_rank));
        }
        current_rank = string_number.substr(1 - is_positive_, last_pos + is_positive_);
        number_.push_back(stoi(current_rank));
        rank_ = number_.size() - 1;
        while(size_ * COUNT < string_number.size()) {
            size_ *= 2;
        } 
        number_.resize(size_);
    }
}

BigInteger::operator int()const {
    int result = 0;
    for (size_t current_rank = rank_ + 1; current_rank > 0; --current_rank) {
        result *= SIZE;
        result += number_[current_rank - 1];
    }
    return result;
}

string BigInteger::toString()const {
    string result;
    if (!is_positive_) {
        result += '-';
    }
    for (size_t current_rank = rank_ + 1; current_rank > 0; --current_rank) {
        string string_part = std::to_string(number_[current_rank - 1]);
        size_t leading_zeros_count = COUNT - string_part.size() + 1;
        if (leading_zeros_count > 1 && current_rank != rank_ + 1) {
            while (leading_zeros_count > 1) {
                result += '0';
                --leading_zeros_count;
            }
        }
        result += string_part;
    }
    return result;
}

void BigInteger::swap(BigInteger& arg_number) {
    std::swap(is_positive_, arg_number.is_positive_);
    std::swap(number_, arg_number.number_);
    std::swap(size_, arg_number.size_);
    std::swap(rank_, arg_number.rank_);
}

BigInteger abs(const BigInteger& number) {
    BigInteger temp_number = number;
    temp_number.is_positive_ = true;
    return temp_number;
}

BigInteger min(const BigInteger& first_number, const BigInteger& second_number) {
    return (first_number <= second_number ? first_number : second_number);
}

BigInteger max(const BigInteger& first_number, const BigInteger& second_number) {
    return (first_number >= second_number ? first_number : second_number);
}

BigInteger& BigInteger::operator=(const BigInteger& arg_number) {
    BigInteger temp_number(arg_number);
    swap(temp_number);
    return *this;
}

void BigInteger::fix_number() {
    while (number_[rank_] == 0 && rank_ > 0) {
        --rank_;
    }
    while (size_ > 2 && size_ > 4 * rank_) {
        size_ /= 2;
        number_.resize(size_);
    }
    if (rank_ == 0 && number_[rank_] == 0) {
        is_positive_ = true;
    }
}

BigInteger& BigInteger::operator+=(const BigInteger& second_number) {
    if (is_positive_ == second_number.is_positive_) {
        size_t current_rank = 0, current_sum = 0;
        do {
            if (current_rank == size_) {
                size_ *= 2;
                number_.resize(size_);
            }
            current_sum += number_[current_rank]; 
            if (current_rank <= second_number.rank_) {
                current_sum += second_number.number_[current_rank];
            }
            number_[current_rank] = current_sum % SIZE;
            current_sum /= SIZE;
            rank_ = std::max(rank_, current_rank++);
        } while (current_sum > 0 || current_rank <= second_number.rank_);
    } else {
        if (abs(*this) >= abs(second_number)) {
            bool is_lend = false;
            size_t current_rank = 0;
            while (current_rank <= second_number.rank_) {
                if (number_[current_rank] >= second_number.number_[current_rank] + is_lend) {
                    number_[current_rank] -= second_number.number_[current_rank] + is_lend;
                    is_lend = false;
                } else {
                    number_[current_rank] = SIZE - is_lend + number_[current_rank] - 
                        second_number.number_[current_rank];
                    is_lend = true;
                }
                ++current_rank;
            }
            while (is_lend) {
                if (number_[current_rank] >= is_lend) {
                    number_[current_rank] -= is_lend;
                    is_lend = false;
                } else {
                    number_[current_rank] = SIZE - is_lend + number_[current_rank]; // по факту, number_[current_rank] = 0;
                }
                ++current_rank;
            }
        } else { 
            is_positive_ = !is_positive_;
            number_.resize(second_number.number_.size());
            size_ = second_number.size_;
            rank_ = second_number.rank_;
            bool is_lend = false;
            for (size_t current_rank = 0; current_rank <= rank_; ++current_rank) {
                if (second_number.number_[current_rank] >= number_[current_rank] + is_lend) {
                    number_[current_rank] = second_number.number_[current_rank] - 
                        number_[current_rank] - is_lend;
                    is_lend = false;
                } else {
                    number_[current_rank] = SIZE - is_lend + second_number.number_[current_rank] - 
                        number_[current_rank];
                    is_lend = true;
                }
            }
        }  
    }
    (*this).fix_number();
    return *this;
}

BigInteger operator+(const BigInteger& first_number, const BigInteger& second_number) {
    BigInteger temp_number(first_number);
    temp_number += second_number;
    return temp_number;
}

BigInteger operator+(const BigInteger& first_number, int second_number) {
   return first_number + BigInteger(second_number);
}

BigInteger operator+(int first_number, const BigInteger& second_number) {
    return BigInteger(first_number) + second_number;
}

BigInteger& BigInteger::operator-=(const BigInteger& second_number) { 
    return (*this += -second_number); 
}

BigInteger operator-(const BigInteger& first_number, const BigInteger& second_number) {
    BigInteger temp_number(first_number);
    temp_number -= second_number;
    return temp_number;
}

BigInteger operator-(const BigInteger& first_number, int second_number) {
    return first_number - BigInteger(second_number);
}

BigInteger operator-(int first_number, const BigInteger& second_number) {
    return BigInteger(first_number) - second_number;
}

BigInteger& BigInteger::operator*=(const BigInteger& second_number) {
    size_t max_size = std::max(size_, second_number.size_) * 2;
    vector<size_t> new_number(max_size, 0);
    for (size_t first_rank = 0; first_rank <= rank_; ++first_rank) {
        for (size_t second_rank = 0; second_rank <= second_number.rank_; ++second_rank) {
            new_number[second_rank + first_rank] += number_[first_rank] * second_number.number_[second_rank];
            new_number[second_rank + first_rank + 1] += new_number[second_rank + first_rank] / SIZE;
            new_number[second_rank + first_rank] %= SIZE;
        }
    }
    is_positive_ = (is_positive_ == second_number.is_positive_);
    number_ = new_number;
    size_ = max_size;
    rank_ = max_size - 1;
    (*this).fix_number();
    return *this;
}

BigInteger operator*(const BigInteger& first_number, const BigInteger& second_number) {
    BigInteger temp_number(first_number);
    temp_number *= second_number;
    return temp_number;
}

BigInteger operator*(const BigInteger& first_number, int second_number) {
    return first_number * BigInteger(second_number);
}

BigInteger operator*(int first_number, const BigInteger& second_number) {
    return BigInteger(first_number) * second_number;
}

BigInteger& BigInteger::operator/=(const BigInteger& second_number) {
    BigInteger result(0), subnumber(0), abs_second_number = abs(second_number);
    size_t current_rank = rank_ + 1;
    while (subnumber < abs_second_number && current_rank > 0) {
        subnumber *= SIZE;
        subnumber += number_[--current_rank];
    }
    while (true) {
        size_t left_value = 0, right_value = SIZE;
        while (left_value < right_value - 1) {
            size_t middle_value = (left_value + right_value) >> 1;
            if (abs_second_number * BigInteger(middle_value) <= subnumber) { // почему возникает сообщение от компилятора, если нет явного преобразования? Оператор ведь перегружен для BigInteger * int.
                left_value = middle_value;
            } else {
                right_value = middle_value;
            }
        }
        subnumber -= abs_second_number * BigInteger(left_value);
        result *= SIZE;
        result += left_value;
        if (current_rank > 0) {
            subnumber *= SIZE;
            subnumber += number_[--current_rank];
        } else {
            break;
        }
    }
    result.is_positive_ = (is_positive_ == second_number.is_positive_);
    *this = result;
    (*this).fix_number();
    return *this;
}

BigInteger operator/(const BigInteger& first_number, const BigInteger& second_number) {
    BigInteger temp_number(first_number);
    temp_number /= second_number;
    return temp_number;
}

BigInteger operator/(const BigInteger& first_number, int second_number) {
    return first_number / BigInteger(second_number);
}

BigInteger operator/(int first_number, const BigInteger& second_number) {
    return BigInteger(first_number) / second_number;
}

BigInteger& BigInteger::operator%=(const BigInteger& second_number) {
    *this = *this - (*this / second_number) * second_number;
    return *this;
}

BigInteger operator%(const BigInteger& first_number, const BigInteger& second_number) {
    BigInteger temp_number = first_number;
    temp_number %= second_number;
    return temp_number;
} 

BigInteger operator%(const BigInteger& first_number, size_t second_number) {
    return first_number % BigInteger(second_number);
}

BigInteger operator%(int first_number, const BigInteger& second_number) {
    return BigInteger(first_number) % second_number;
}

BigInteger& BigInteger::operator++() {
    (*this) += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp_number(*this);
    ++(*this);
    return temp_number;
}

BigInteger& BigInteger::operator--() {
    (*this) -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp_number(*this);
    --(*this);
    return temp_number;
}

BigInteger operator-(const BigInteger& number) {
    if (number != BigInteger(0)) {
        BigInteger temp_number(number);
        temp_number.is_positive_ = !temp_number.is_positive_;
        return temp_number;
    } else {
        return number;
    }
}

bool operator>(const BigInteger& first_number, const BigInteger& second_number) {
    if (first_number.is_positive_ != second_number.is_positive_) {
        return (first_number.is_positive_ == 1 ? true : false);
    } else {
        if (first_number.rank_ != second_number.rank_) {
            return (first_number.is_positive_ ? 
                    first_number.rank_ > second_number.rank_ : first_number.rank_ < second_number.rank_);
        } else {
            bool is_more = false;
            for (size_t current_rank = first_number.rank_ + 1; current_rank > 0; --current_rank) {
                if (first_number.number_[current_rank - 1] == second_number.number_[current_rank - 1]) {
                    continue;
                } else {
                    is_more = first_number.number_[current_rank - 1] > 
                        second_number.number_[current_rank - 1];
                    return (first_number.is_positive_ ? is_more : !is_more);
                }
            }
            return false;
        }
    }
}

bool operator<(const BigInteger& first_number, const BigInteger& second_number) {
    return second_number > first_number;
}

bool operator==(const BigInteger& first_number, const BigInteger& second_number) {
    if (first_number.rank_ != second_number.rank_ || 
            first_number.is_positive_ != second_number.is_positive_) {
        return false;
    } else {
        for (size_t current_rank = 0; current_rank <= first_number.rank_; ++current_rank) {
            if (first_number.number_[current_rank] != second_number.number_[current_rank]) {
                return false;
            }
        }
        return true;
    }
}

bool operator!=(const BigInteger& first_number, const BigInteger& second_number) {
    return !(first_number == second_number);
}

bool operator>=(const BigInteger& first_number, const BigInteger& second_number) {
    return first_number == second_number || first_number > second_number;
}

bool operator<=(const BigInteger& first_number, const BigInteger& second_number) {
    return second_number >= first_number;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& number) {
   out << number.toString();
   return out;
}

std::istream& operator>>(std::istream& in, BigInteger& number) {
   string string_number;
   in >> string_number;
   number = BigInteger(string_number);
   return in;
}

BigInteger operator "" _bi(const char* string_number) {
    return BigInteger(string_number);
}

class Rational{
private:
    BigInteger numerator_, denominator_; 
    BigInteger GCD(BigInteger&, BigInteger&);
    void swap(Rational&);
    void fix_number();

public:
    Rational() : numerator_(0), denominator_(1) {}
    Rational(const Rational& number) : 
        numerator_(number.numerator_), denominator_(number.denominator_) {}
    Rational(int numerator) : numerator_(numerator), denominator_(1) {}
    Rational(const BigInteger& number) : numerator_(number), denominator_(1) {}

    explicit operator double()const;

    string toString()const;
    string asDecimal(size_t)const;
    
    Rational& operator=(const Rational&);
    Rational& operator+=(const Rational&);
    Rational& operator-=(const Rational&);
    Rational& operator*=(const Rational&);
    Rational& operator/=(const Rational&);

    friend Rational operator-(const Rational&);
    friend bool operator>(const Rational&, const Rational&);
    friend bool operator==(const Rational&, const Rational&);
};

BigInteger Rational::GCD(BigInteger& first_number, BigInteger& second_number) {
    if (second_number == BigInteger(0)) {
        return first_number;
    } else {
        first_number %= second_number;
        return GCD(second_number, first_number);
    }
}

Rational::operator double()const {
    string result = (*this).asDecimal(16); // по факту, лишняя линяя. Но можно сделать, как в asDecimal
    return stod(result);
}

void Rational::swap(Rational& arg_number) {
    std::swap(numerator_, arg_number.numerator_);
    std::swap(denominator_, arg_number.denominator_);
}

void Rational::fix_number() {
    BigInteger first_number = abs(numerator_), 
               second_number = denominator_;
    BigInteger gcd = GCD(first_number, second_number);
    numerator_ /= gcd;
    denominator_ /= gcd;
    if (denominator_ < BigInteger(0)) {
        numerator_ *= -1;
        denominator_ *= -1;
    }
}

string Rational::toString()const {
   string result = numerator_.toString();
   if (denominator_ != BigInteger(1)) {
       result += '/' + denominator_.toString();
   }
   return result;
}

string Rational::asDecimal(size_t precision = 0)const {
   BigInteger quotient = numerator_ / denominator_;
   BigInteger remainder = abs(numerator_ - quotient * denominator_);
   string result;
   if (numerator_ < BigInteger(0)) {
       result += '-';
   }
   result += quotient.toString();
   if (precision != 0) {
       result += '.';
       while (precision-- > 0) {
           remainder *= 10;
           result += (remainder / denominator_).toString();
           remainder %= denominator_;
       }
   }
   return result;
}

Rational& Rational::operator=(const Rational& arg_number) {
    Rational temp_number(arg_number);
    swap(temp_number);
    return *this;
}

Rational& Rational::operator+=(const Rational& second_number) {
    numerator_ = numerator_ * second_number.denominator_ + denominator_ * second_number.numerator_;
    denominator_ *= second_number.denominator_;
    (*this).fix_number();
    return *this;
}

Rational operator+(const Rational& first_number, const Rational& second_number) {
    Rational temp_number(first_number);
    temp_number += second_number;
    return temp_number;
}

Rational& Rational::operator-=(const Rational& second_number) { 
    return (*this += -second_number); 
}

Rational operator-(const Rational& first_number, const Rational& second_number) {
    Rational temp_number(first_number);
    temp_number -= second_number;
    return temp_number;
}

Rational& Rational::operator*=(const Rational& second_number) {
   numerator_ *= second_number.numerator_;
   denominator_ *= second_number.denominator_;
   (*this).fix_number();
   return *this;
}

Rational operator*(const Rational& first_number, const Rational& second_number) {
    Rational temp_number(first_number);
    temp_number *= second_number;
    return temp_number;
}

Rational& Rational::operator/=(const Rational& second_number) {
    numerator_ *= second_number.denominator_; 
    denominator_ *= second_number.numerator_;
    (*this).fix_number();
    return *this;
}

Rational operator/(const Rational& first_number, const Rational& second_number) {
    Rational temp_number(first_number);
    temp_number /= second_number;
    return temp_number;
}

Rational operator-(const Rational& number) {
    Rational temp_number(number);
    temp_number.numerator_ = -temp_number.numerator_;
    return temp_number;
}

bool operator>(const Rational& first_number, const Rational& second_number) {
    BigInteger first_numerator = first_number.numerator_ * second_number.denominator_;
    BigInteger second_numerator = first_number.denominator_ * second_number.numerator_;
    return first_numerator > second_numerator;
}

bool operator<(const Rational& first_number, const Rational& second_number) {
    return second_number > first_number;
}

bool operator==(const Rational& first_number, const Rational& second_number) {
  return first_number.numerator_ == second_number.numerator_ && 
      first_number.denominator_ == second_number.denominator_;
}

bool operator!=(const Rational& first_number, const Rational& second_number) {
    return !(first_number == second_number);
}

bool operator>=(const Rational& first_number, const Rational& second_number) {
    return first_number == second_number || first_number > second_number;
}

bool operator<=(const Rational& first_number, const Rational& second_number) {
    return second_number >= first_number;
}
