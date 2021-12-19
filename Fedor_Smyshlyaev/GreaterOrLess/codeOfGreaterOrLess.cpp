#include <iostream>

size_t dotNumber(char* searchingString) {
    size_t index = strlen(searchingString);
    for(int i = 0; i < index; i++) {
        if (searchingString[i] == '.') {
            index = i + 1;
            break;
        }
    }
    return index;
}

void removeNull(char* strWithZerosAtTheEnd) {
    size_t index = strlen(strWithZerosAtTheEnd) - 1;
    while (strWithZerosAtTheEnd[index] == '0' && index > 0) {
        strWithZerosAtTheEnd[index] = '\0';
        index--;
    }
    if (strWithZerosAtTheEnd[index] == '.' && index > 0) strWithZerosAtTheEnd[index] = '\0';
}

char comparison(char* firstNum, char* secondNum) {
    bool reverse = false;
    char answer = '=';
    if (firstNum[0] == '-') {
        std::swap(firstNum, secondNum);
        reverse = true;
    }

    size_t firstDotNumber = dotNumber(firstNum);
    size_t secondDotNumber = dotNumber(secondNum);

    if (strlen(firstNum) > firstDotNumber) removeNull(firstNum);
    if (strlen(secondNum) > secondDotNumber) removeNull(secondNum);

    firstDotNumber = dotNumber(firstNum);
    secondDotNumber = dotNumber(secondNum);

    if (firstNum[0] != '-' && secondNum[0] == '-')
        answer = '>';
    else {
        if (firstNum[0] == '-' && secondNum[0] == '-') reverse = !reverse;
        if (firstDotNumber > secondDotNumber) answer = '>';
        else if (secondDotNumber > firstDotNumber) answer = '<';
        else {
            for (size_t i = 0; i < strlen(firstNum); i++) {
                if (firstNum[i] - '0' > secondNum[i] - '0') {
                    answer = '>';
                    break;
                } else if (firstNum[i] - '0' < secondNum[i] - '0') {
                    answer = '<';
                    break;
                }
            }
        }
    }

    if (reverse) {
        if (answer == '>') answer = '<';
        else if (answer == '<') answer = '>';
    }

    return answer;
}

int main() {
    char firstString[10100];
    char secondString[10100];

    scanf("%s", firstString);
    scanf("%s", secondString);

    std::cout << comparison(firstString, secondString);
}
