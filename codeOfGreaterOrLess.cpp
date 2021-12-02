#include <iostream>

size_t dotNumber(char* searchingString) {
    size_t index = strlen(searchingString);
    for(int i = 0; i < index; i++) {
        if (searchingString[i] == '.')
            index = i + 1;
    }
    return index;
}

void removeNull(char* forRemoveChar) {
    size_t index = strlen(forRemoveChar) - 1;
    while (forRemoveChar[index] == '0' && index > 0) {
        forRemoveChar[index] = '\0';
        index--;
    }
    if (forRemoveChar[index] == '.' && index > 0) forRemoveChar[index] = '\0';
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
