#include <iostream>

bool isIndexCorrect(size_t sizeTLen, const size_t* valuesOfIndexes) {
    bool returnAnswer = true;

    int len = static_cast<int>(sizeTLen);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (valuesOfIndexes[i] == valuesOfIndexes[j]) {
                returnAnswer = false;
                break;
            }
        }
        if (!returnAnswer) break;
    }
    return returnAnswer;
}

long long searchSum(size_t lenOfPointer, size_t* lenOfArrays, int** pointerArrays,
                         size_t numberOfStep, long long temporaryAnswer, size_t* valuesOfIndexes, long long delta) {

    if (numberOfStep == lenOfPointer) {
        if (isIndexCorrect(lenOfPointer, valuesOfIndexes))
            return delta;
        else
            return 0;
    }

    for (size_t i = 0; i < lenOfArrays[numberOfStep]; i++) {
        valuesOfIndexes[numberOfStep] = i;
        temporaryAnswer += searchSum(lenOfPointer, lenOfArrays, pointerArrays,numberOfStep + 1,
                                          0, valuesOfIndexes, delta * pointerArrays[numberOfStep][i]);
    }

    return temporaryAnswer;
}

int main(int numberStrings, char* charArray[]) {

    size_t lenOfPointer = numberStrings - 1;
    auto* lenOfArrays = new size_t[lenOfPointer];
    int** pointerArrays = new int*[lenOfPointer];

    for (size_t i = 0; i < lenOfPointer; i++) {
        lenOfArrays[i] = atoi(charArray[i + 1]);
        pointerArrays[i] = new int[lenOfArrays[i]];
    }

    for (size_t i = 0; i < lenOfPointer; i++) {
        for (size_t j = 0; j < lenOfArrays[i]; j++) {
            std::cin >> pointerArrays[i][j];
        }
    }

    auto* valueOfIndexes = new size_t[lenOfPointer];
    std::cout << searchSum(lenOfPointer, lenOfArrays, pointerArrays, 0, 0, valueOfIndexes, 1);

    for (size_t i = 0; i < lenOfPointer; i++)
        delete[] pointerArrays[i];
    delete[] pointerArrays;
    delete[] lenOfArrays;
    delete[] valueOfIndexes;
}
