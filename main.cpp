#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>

std::ifstream myFile;

int generateRandomNumber (int from, int to) {
    int randomNumber = rand() % (to - from) + from;
    return randomNumber;
}

double generateRandomDoubleNumber (double from, double to) {
    double randomNumber = (double)rand() / RAND_MAX;
    return from + randomNumber * (from - to);
}

void writeAndPrintDataFile(int numberOfCities, std::vector<std::vector<int>>& matrix, std::vector<int>& perm) {
    if (!myFile.good())
        std::cout << "Can't open file" << std::endl;
    else {
        int j = 0;
        while (myFile.good() && j != numberOfCities + 2) {
            std::string line;
            getline(myFile, line);
            std::istringstream lline (line);
            for (int i = 0; i < numberOfCities; i++) {
                lline >> matrix[j][i];
            }
            j++;
        }
        for (int i = 0; i < numberOfCities+2; i++) {
            for (int j = 0; j < numberOfCities; j++)
                std::cout << matrix[i][j] << " ";
            std::cout << std::endl;
        }
        for (unsigned long i = 0; i < perm.size(); i++)
            perm[i] = matrix[numberOfCities + 1][i];
        for (unsigned long i = 0; i < perm.size(); i++)
            std::cout << perm[i] << " ";
    }
}

void calculatePermutationResult(std::vector<std::vector<int>>& matrix, std::vector<int>& perm, int numberOfCities, int& result) {
    result = 0;
    for (int i = 0; i < numberOfCities; i++) {
        result += matrix[perm[i]+1][perm[i+1]];
    }
}

void calculateBestResult(std::vector<std::vector<int>>& matrix, std::vector<int>& bestPerm, int numberOfCities, unsigned int& bestResult, unsigned int& bestBestResult) {
    do {
        for (int i = 0; i < numberOfCities; i++) {
            bestResult += matrix[bestPerm[i]+1][bestPerm[i+1]];
        }
        if(bestResult < bestBestResult)
            bestBestResult = bestResult;
        bestResult = 0;
    } while(std::next_permutation(bestPerm.begin()+1, bestPerm.end()));
}

void initializePerm(std::vector<int>& perm, int numberOfCities) {
    for (int i = 0; i < numberOfCities; i++) {
        perm[i] = i;
    }
}

void checkIfEqual(int firstRandomNumber, int secondRandomNumber) {
    if (firstRandomNumber == secondRandomNumber) {
        while (firstRandomNumber == secondRandomNumber) {
            secondRandomNumber = generateRandomNumber(1, 8);
        }
    }
}

void switchPlaces(int firstPosition, int secondPosition, std::vector<int>& perm) {
    int tempPosition = perm[secondPosition];
    perm[secondPosition] = perm[firstPosition];
    perm[firstPosition] = tempPosition;
}

void calculateSAResult(int numberOfCities, std::vector<std::vector<int>> matrix, int& saResult) {
    // variables initialization
    std::vector <int> perm (numberOfCities);
    initializePerm(perm, numberOfCities);
    int initialResult; // x0
    calculatePermutationResult(matrix, perm, numberOfCities, initialResult);
    int currentResult = initialResult; // x
    int bestResult = currentResult;
    int newResult = currentResult;
    double initialTemperature = 1000;
    double currentTemperature = initialTemperature;
    double finalTemperature = 0;
    double delta;
    double p;
    double z;
    double coolingRate = 0.03;
    // Simulated Annealing
    while (currentTemperature > finalTemperature) {
        int firstRandomNumber;
        firstRandomNumber = generateRandomNumber(1, numberOfCities);
        int secondRandomNumber;
        secondRandomNumber = generateRandomNumber(1, numberOfCities);
        checkIfEqual(firstRandomNumber, secondRandomNumber);
        switchPlaces(firstRandomNumber, secondRandomNumber, perm);
        calculatePermutationResult(matrix, perm, numberOfCities, newResult);
        if (bestResult > newResult)
            bestResult = newResult;
        if (newResult <= currentResult)
            currentResult = newResult;
        else {
            delta = newResult - currentResult;
            p = exp(-delta/currentTemperature);
            z = -generateRandomDoubleNumber(0, 1);
            if (z < p)
                currentResult = newResult;
            currentTemperature -= coolingRate;
        }
    }
    saResult = bestResult;
}


void printResults(int result, int bestBestResult, int saResult) {
    std::cout << std::endl;
    std::cout << std::endl << "Set permutation result: " << result << std::endl;
    std::cout << "Best result: " << bestBestResult << std::endl;
    std::cout << "Simulated annealing result: " << saResult << std::endl;
}

int main() {
    // variables initialization
    int numberOfCities = 0;
    int result = 0;
    unsigned int bestResult = 0;
    unsigned int bestBestResult = 50000;
    int saResult = 0;

    srand(time(NULL));
    myFile.open("data.txt");
    std::string s;
    myFile >> s;
    std::istringstream (s) >> numberOfCities;
    std::vector <std::vector <int>> matrix (numberOfCities+2, std::vector<int>(numberOfCities));
    std::vector <int> perm (numberOfCities);
    std::vector <int> bestPerm (numberOfCities);
    for (int i = 0; i < numberOfCities; i++) {
        bestPerm[i] = i;
    }
    // problem solving
    writeAndPrintDataFile(numberOfCities, matrix, perm);
    myFile.close();
    calculatePermutationResult(matrix, perm, numberOfCities, result);
    calculateBestResult(matrix, bestPerm, numberOfCities, bestResult, bestBestResult);
    calculateSAResult(numberOfCities, matrix, saResult);
    printResults(result, bestBestResult, saResult);
}
