#include <iostream>
#include <string>

bool isTrivialEnd(const std::string& number) {
    unsigned int sizeNumber = number.size();
    for (int i = 0; i < sizeNumber; ++i) {
        if (number[i] != '0') return false;
    }
    return true;
}

void firstIsLess() {
    std::cout << '<' << '\n';
}

void secondIsLess() {
    std::cout << '>' << '\n';
}

void firstAndSecondAreEqual() {
    std::cout << '=' << '\n';
}

unsigned int getPointPos(std::string& number) {
    size_t sizeNumber = number.size();
    for (int i = 0; i < sizeNumber; ++i) {
        if (number[i] == '.') return i + 1;
    }
    number += ".0";
    return sizeNumber + 1;
}

void compareNumbers(std::string& firstNumber, std::string& secondNumber) {
    if (firstNumber[0] == '-' && secondNumber[0] == '-') {
        std::swap(firstNumber, secondNumber);
        firstNumber.erase(firstNumber.begin());
        secondNumber.erase(secondNumber.begin());
    } else if (firstNumber[0] == '-' && secondNumber[0] != '-') {
        firstIsLess();
        return;
    } else if (firstNumber[0] != '-' && secondNumber[0] == '-') {
        secondIsLess();
        return;
    }

    unsigned int firstNumberPoint = getPointPos(firstNumber),
                 secondNumberPoint = getPointPos(secondNumber);
    if (firstNumberPoint < secondNumberPoint) {
        firstIsLess();
        return;
    } else if (firstNumberPoint > secondNumberPoint) {
        secondIsLess();
        return;
    }

    unsigned int commonPointPosition = firstNumberPoint;
    for (int i = 0; i < commonPointPosition; ++i) {
        if (firstNumber[i] > secondNumber[i]) {
            secondIsLess();
            return;
        } else if (firstNumber[i] < secondNumber[i]) {
            firstIsLess();
            return;
        }
    }
    firstNumber.erase(firstNumber.begin(), firstNumber.begin() + commonPointPosition);
    secondNumber.erase(secondNumber.begin(), secondNumber.begin() + commonPointPosition);

    unsigned int currentPosition = 0;
    while (currentPosition < std::min(firstNumber.size(), secondNumber.size())) {
        if (firstNumber[currentPosition] > secondNumber[currentPosition]) {
            secondIsLess();
            return;
        } else if (firstNumber[currentPosition] < secondNumber[currentPosition]) {
            firstIsLess();
            return;
        }
        ++currentPosition;
    }

    if (currentPosition + 1 < firstNumber.size()) {
        firstNumber.erase(firstNumber.begin(), firstNumber.begin() + currentPosition);
        if (isTrivialEnd(firstNumber)) {
            firstAndSecondAreEqual();
            return;
        } else {
            secondIsLess();
            return;
        }
    }

    if (currentPosition + 1 < secondNumber.size()) {
        secondNumber.erase(secondNumber.begin(), secondNumber.begin() + currentPosition);
        if (isTrivialEnd(secondNumber)) {
            firstAndSecondAreEqual();
            return;
        } else {
            firstIsLess();
            return;
        }
    }
    firstAndSecondAreEqual();
}

int main(int argc, char* argv[]) {
    std::string firstNumber, secondNumber;
    std::cin >> firstNumber >> secondNumber;
    compareNumbers(firstNumber, secondNumber);

    return 0;
}

