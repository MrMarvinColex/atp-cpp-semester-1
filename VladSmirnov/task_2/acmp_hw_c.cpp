#include <iostream>
#include <stdio.h>
#include <string.h>

bool isTrivialEnd(const char* number) {
    for (int i = 0; number[i] != '\0'; ++i) {
        if (number[i] != '0') return false;
    }
    return true;
}

void firstIsLess() {
    printf("%c\n", '<');
}

void secondIsLess() {
    printf("%c\n", '>');
}

void firstAndSecondAreEqual() {
    printf("%c\n", '=');
}

unsigned int getPointPos(char* number) {
    unsigned int currentPosition = 0;
    while (number[currentPosition] != '\0') {
        if (number[currentPosition] == '.') return currentPosition + 1;
        ++currentPosition;
    }
    number[currentPosition] = '.';
    number[currentPosition + 1] = '0';
    number[currentPosition + 2] = '\0';
    return currentPosition + 1;
}

void compareNumbers(char* firstNumber, char* secondNumber) {
    if (firstNumber[0] == '-' && secondNumber[0] == '-') {
        for (int i = 0; firstNumber[i] != '\0'; ++i) {
            firstNumber[i] = firstNumber[i + 1];
        }
        for (int i = 0; secondNumber[i] != '\0'; ++i) {
            secondNumber[i] = secondNumber[i + 1];
        }
        std::swap(firstNumber, secondNumber);
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

    unsigned int currentPosition = 0;
    size_t firstNumberSize, secondNumberSize;
    do {
        firstNumber[currentPosition] = firstNumber[currentPosition + commonPointPosition];
        firstNumberSize = currentPosition;
    } while(firstNumber[currentPosition++] != '\0');
    currentPosition = 0;
    do {
        secondNumber[currentPosition] = secondNumber[currentPosition + commonPointPosition];
        secondNumberSize = currentPosition;
    } while(secondNumber[currentPosition++] != '\0');

    currentPosition = 0;
    while (currentPosition < std::min(firstNumberSize, secondNumberSize)) {
        if (firstNumber[currentPosition] > secondNumber[currentPosition]) {
            secondIsLess();
            return;
        } else if (firstNumber[currentPosition] < secondNumber[currentPosition]) {
            firstIsLess();
            return;
        }
        ++currentPosition;
    };

    unsigned int lastPosition = currentPosition;
    if (lastPosition < firstNumberSize) {
        do {
            firstNumber[currentPosition - lastPosition] = firstNumber[currentPosition];
        } while(firstNumber[currentPosition++] != '\0');
        if (isTrivialEnd(firstNumber)) {
            firstAndSecondAreEqual();
            return;
        } else {
            secondIsLess();
            return;
        }
    }

    currentPosition = lastPosition;
    if (lastPosition < secondNumberSize) {
        do {
            secondNumber[currentPosition - lastPosition] = secondNumber[currentPosition];
        } while(secondNumber[currentPosition++] != '\0');
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
    char firstNumber[10000 + 10], secondNumber[10000 + 10];
    scanf("%s%s", &firstNumber, &secondNumber);
    compareNumbers(firstNumber, secondNumber);

    return 0;
}

