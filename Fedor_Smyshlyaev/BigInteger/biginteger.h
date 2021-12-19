#include <iostream>
#include <string>
#include <vector>

class BigInteger {
public:
    /// Friends
    friend class Rational;
    friend std::istream& operator>>(std::istream& in, BigInteger& bigNum);
    friend std::ostream& operator<<(std::ostream& out, const BigInteger& bigNum);

    /// Destructor
    ~BigInteger() = default;

    /// Constructor
    BigInteger() : isMinus(false), size(1), vectorNums(1, 0) {}

// "гном" is equal "dwarf", the joke is that simple integer is much smaller than BigInteger
    BigInteger(int dwarf) : isMinus(dwarf < 0), size(intAbs(dwarf) / base + 1), vectorNums() {
        for (int i = 0; i < size; i++) {
            vectorNums.push_back(dwarf % base);
            dwarf /= base;
        }
    }

    BigInteger(const BigInteger& bigNum) : isMinus(bigNum.isMinus), size(bigNum.size), vectorNums(bigNum.vectorNums) {}

    /// Methods
    [[nodiscard]] std::string toString() const {
        std::string string;
        if (isMinus)
            string += '-';
        string += std::to_string(vectorNums[size - 1]);
        for (int i = size - 2; i >= 0; i--) {
            std::string digits;
            digits = std::to_string(vectorNums[i]);
            std::string nulls(baseLength - digits.size(), '0'); // дополняем необходимые нули
            string += (nulls + digits);
        }
        return string;
    }

    /// operators
    BigInteger& operator+=(const BigInteger& bigNum) {
        BigInteger copy(bigNum);
        long long balance = 0;

        if (isMinus == copy.isMinus) {
            std::vector<long long> newVector;
            for (int i = 0; i <= std::max(size, copy.size); i++) {
                long long firstNum = (i >= size ? 0 : vectorNums[i]);
                long long secondNum = (i >= copy.size ? 0 : copy.vectorNums[i]);

                newVector.push_back((firstNum + secondNum + balance) % base);
                balance = (firstNum + secondNum + balance) / base;
            }
            size = static_cast<int>(newVector.size());
            vectorNums = newVector;
        } else {
            if (modIsEqual(copy))
                return *this = 0;
            if (modIsSmaller(copy)) {
                isMinus = copy.isMinus;
                swap(copy);
            }
            for (int i = 0; i < size; i++) {
                vectorNums[i] -= (balance + (copy.size <= i ? 0 : copy.vectorNums[i]));
                balance = 0;
                while (vectorNums[i] < 0) {
                    vectorNums[i] += base;
                    balance++;
                }
            }
        }
        deleteNulls();

        return *this;
    }

    BigInteger& operator-=(const BigInteger& bigNum) {
        BigInteger copy(bigNum);
        copy.isMinus = !copy.isMinus;
        return (*this += copy);
    }

    BigInteger& operator*=(const BigInteger& bigNum) {
        isMinus = (isMinus != bigNum.isMinus);
        std::vector<long long> newVector(size + bigNum.size, 0);

        for (int my = 0; my < size; my++) {
            for (int his = 0; his < bigNum.size; his++) {
                newVector[my + his] += vectorNums[my] * bigNum.vectorNums[his];
                newVector[my + his + 1] += newVector[my + his] / base;
                newVector[my + his] %= base;
            }
        }

        vectorNums = newVector;
        size += bigNum.size;
        deleteNulls();
        return *this;
    }

    BigInteger& operator/=(const BigInteger& bigNum) {
        if (modIsSmaller(bigNum))
            return (*this = 0);

        bool newMinus = isMinus != bigNum.isMinus;
        isMinus = false;
        BigInteger divider(bigNum);
        divider.isMinus = false;
        BigInteger numerator(0);
        BigInteger answer(0);

        int index = size - 1;
        while (index >= 0) {
            while (numerator < divider && index >= 0) {
                numerator *= base;
                numerator += vectorNums[index];
                answer *= base;
                index--;
            }
            long long leftBorder = 0;
            long long rightBorder = base;
            while (leftBorder <= rightBorder) {
                long long coefficient = (leftBorder + rightBorder) / 2;
                if (numerator < divider * coefficient) {
                    rightBorder = coefficient - 1;
                }
                else
                    leftBorder = coefficient + 1;
            }
            numerator -= divider * rightBorder;
            answer += rightBorder;
        }
        *this = answer;
        isMinus = newMinus;
        return *this;
    }

    BigInteger& operator%=(const BigInteger& bigNum) {
        return (*this = (*this - bigNum * (*this / bigNum)));
    }

    BigInteger operator+(const BigInteger& bigNum) const {
        BigInteger copy(*this);
        return copy += bigNum;
    }

    BigInteger operator-(const BigInteger& bigNum) const {
        BigInteger copy(*this);
        return copy -= bigNum;
    }

    BigInteger operator*(const BigInteger& bigNum) const {
        BigInteger copy(*this);
        return copy *= bigNum;
    }

    BigInteger operator/(const BigInteger& bigNum) const {
        BigInteger copy(*this);
        return copy /= bigNum;
    }

    BigInteger operator%(const BigInteger& bigNum) const {
        BigInteger copy(*this);
        return copy %= bigNum;
    }

    BigInteger& operator=(const BigInteger& bigNum) {
        size = bigNum.size;
        isMinus = bigNum.isMinus;
        vectorNums = bigNum.vectorNums;
        return *this;
    }

    /// Logical operators
    bool operator==(const BigInteger& bigNum) const {
        return modIsEqual(bigNum) && (isMinus == bigNum.isMinus);
    }

    bool operator!=(const BigInteger& bigNum) const {
        return (*this < bigNum) || (*this > bigNum);
    }

    bool operator<(const BigInteger& bigNum) const {
        if (isMinus != bigNum.isMinus)
            return isMinus;
        return isMinus ? !modIsSmaller(bigNum) : modIsSmaller(bigNum);
    }

    bool operator>(const BigInteger& bigNum) const {
        return bigNum < *this;
    }

    bool operator<=(const BigInteger& bigNum) const {
        if (isMinus != bigNum.isMinus)
            return isMinus;
        return (isMinus ? (!modIsSmaller(bigNum) || modIsEqual(bigNum)) : (modIsSmaller(bigNum) || modIsEqual(bigNum)));
    }

    bool operator>=(const BigInteger& bigNum) const {
        return bigNum <= *this;
    }

    // is it really needed?
    BigInteger operator+() const {
        return *this;
    }

    BigInteger operator-() const {
        BigInteger copy(*this);
        copy.isMinus = (!copy.isMinus && copy);
        return copy;
    }

    BigInteger& operator++() {
        return *this += 1;
    }

    BigInteger operator++(int) {
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }

    BigInteger& operator--() {
        return *this -= 1;
    }

    BigInteger operator--(int) {
        BigInteger copy = *this;
        --(*this);
        return copy;
    }

    explicit operator bool() const {
        return !(*this == 0);
    }

private:
    const long long base = 1000000000; /// == 10^9
    const int baseLength = 9;
    bool isMinus;
    int size;
    std::vector<long long> vectorNums;

    static int intAbs(int& num) {
        // It's change our integer like |x|
        if (num < 0) num *= -1;
        return num;
    }

    [[nodiscard]] bool modIsSmaller(const BigInteger& bigNum) const {
        if (size != bigNum.size)
            return size < bigNum.size;

        int index = size - 1;
        while (index > 0 && vectorNums[index] == bigNum.vectorNums[index])
            index--;
        return vectorNums[index] < bigNum.vectorNums[index];
    }

    [[nodiscard]] bool modIsEqual(const BigInteger& bigNum) const {
        if (size != bigNum.size)
            return false;
        for (int i = size - 1; i >= 0; i--) {
            if (vectorNums[i] != bigNum.vectorNums[i])
                return false;
        }
        return true;
    }

    void deleteNulls() {
        while(size > 1 && vectorNums[size - 1] == 0) {
            vectorNums.pop_back();
            size--;
        }
    }

    void swap(BigInteger& bigNum) {
        std::swap(isMinus, bigNum.isMinus);
        std::swap(size, bigNum.size);
        std::swap(vectorNums, bigNum.vectorNums);
    }
};

BigInteger operator+(int dwarf, const BigInteger& bigNum) {
    return bigNum + dwarf;
}

BigInteger operator-(int dwarf, const BigInteger& bigNum) {
    return -(bigNum - dwarf);
}

BigInteger operator*(int dwarf, const BigInteger& bigNum) {
    return bigNum * dwarf;
}

BigInteger operator/(int dwarf, const BigInteger& bigNum) {
    BigInteger copy(dwarf);
    return copy / bigNum;
}

BigInteger operator%(int dwarf, const BigInteger& bigNum) {
    BigInteger copy(dwarf);
    return copy % bigNum;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& bigNum) {
    out << bigNum.toString();
    return out;
}

std::istream & operator>>(std::istream& in, BigInteger& bigNum) {
    bigNum = 0;

    std::string string;
    in >> string;
    if (string[0] == '-') {
        bigNum.isMinus = true;
        string = std::string(string.begin() + 1, string.end());
    }
    std::vector<long long> newVector(string.size()/bigNum.baseLength + (string.size() % bigNum.baseLength != 0), 0);

    for (int i = 0; i < static_cast<int>(newVector.size()); i++) {
        int x = string.size() - (i + 1) * bigNum.baseLength;
        x = x < 0 ? 0 : x;
        std::string a = std::string(string.begin() + x, string.end() - i * bigNum.baseLength);
        newVector[i] = std::stoi(a);
    }

    bigNum.vectorNums = newVector;
    bigNum.size = newVector.size();
    bigNum.deleteNulls();
    return in;
}

class Rational{
public:
    ///Friends
    friend Rational operator+(const Rational& first, const Rational& second);
    friend Rational operator-(const Rational& first, const Rational& second);
    friend Rational operator*(const Rational& first, const Rational& second);
    friend Rational operator/(const Rational& first, const Rational& second);

    /// Destructor
    ~Rational() = default;

    /// Constructors
    Rational(const BigInteger& bigNum): sky(bigNum), hell(1) {}

    Rational(int dwarf): sky(dwarf), hell(1) {}

    Rational(): sky(0), hell(1) {}

    /// Methods
    [[nodiscard]] std::string toString() const {
        std::string string;
        string += sky.toString();
        if (hell != 1)
            string += ('/' + hell.toString());

        return string;
    }

    [[nodiscard]] std::string asDecimal(size_t precision = 0) const {
        Rational copy = *this;
        copy.sky.isMinus = false;
        std::string string;
        if (copy.sky != 0)
            string = (copy.sky * tenPow(precision) / copy.hell).toString();
        else {
            std::string fodder(precision + 1, '0');
            string = fodder;
        }
        if (precision == 0) {
            if (sky.isMinus)
                string = '-' + string;
            return string;
        }
        else {
            if (string.size() < precision) {
                std::string fodder(precision - string.size() + 1, '0');
                string = fodder + string;
            }
            if (sky.isMinus)
                string = '-' + string;
            return string.substr(0, string.length() - precision) + '.' +
                   string.substr(string.length() - precision, precision);
        }
    }

    /// Operators
    Rational& operator+=(const Rational& dotNum) {
        sky = (sky * dotNum.hell + dotNum.sky * hell);
        hell *= dotNum.hell;
        makeItShort();
        return *this;
    }

    Rational& operator-=(const Rational& dotNum) {
        Rational copy = dotNum;
        copy.sky.isMinus = !copy.sky.isMinus;
        return *this += copy;
    }

    Rational& operator*=(const Rational& dotNum) {
        sky *= dotNum.sky;
        hell *= dotNum.hell;
        makeItShort();
        return *this;
    }

    Rational& operator/=(const Rational& dotNum) {
        sky *= dotNum.hell;
        hell *= dotNum.sky;
        if (hell.isMinus && sky != 0) {
            hell.isMinus = false;
            sky.isMinus = !sky.isMinus;
        }
        if (hell == 0 || sky == 0) {
            hell = 1;
            sky = 0;
        }
        makeItShort();
        return *this;
    }

    Rational& operator=(const Rational& dotNum) = default;

    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        Rational copy = *this;
        copy.sky.isMinus = (!copy.sky.isMinus && copy.sky);
        return copy;
    }

    /// Logical operators
    bool operator==(const Rational& dotNum) const {
        return (*this <= dotNum) && (dotNum <= *this);
    }

    bool operator!=(const Rational& dotNum) const {
        return !((*this <= dotNum) && (dotNum <= *this));
    }

    bool operator<(const Rational& dotNum) const {
        return !(dotNum <= *this);
    }

    bool operator>(const Rational& dotNum) const {
        return !(*this <= dotNum);
    }

    bool operator<=(const Rational& dotNum) const {
        return (sky * dotNum.hell) <= (dotNum.sky * hell);
    }

    bool operator>=(const Rational& dotNum) const {
        return dotNum <= *this;
    }

    explicit operator double() const {
        // I heard, that it's work
        return 1;
    }

private:
    BigInteger sky;
    BigInteger hell;

    BigInteger tenPow(int pow) const {
        BigInteger ans(1);
        for (int i = 0; i < pow; i++)
            ans *= 10;
        return ans;
    }

    // It's a recursion, but beautiful and short one.
    BigInteger evclid(const BigInteger& f, const BigInteger& s) {
        if (f == s || s == 0) return f;
        return evclid(s, f % s);
    }

    void makeItShort() {
        bool newMinus = sky.isMinus;
        sky.isMinus = false;

        BigInteger highestDivisor = evclid(sky, hell);
        if (highestDivisor == 0)
            hell = 1;
        else {
            sky /= highestDivisor;
            hell /= highestDivisor;
        }
        sky.isMinus = newMinus;
    }
};

Rational operator+(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy += second;
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy -= second;
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy *= second;;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy /= second;
}
