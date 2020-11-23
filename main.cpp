#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

std::ifstream myFile;

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

void printResults(int result, int bestBestResult) {
    std::cout << std::endl << "Set Permutation Result: " << result;
    std::cout << std::endl << "Best Result: " << bestBestResult << std::endl;
}

int main() {
    // variables initialization
    int numberOfCities = 0;
    int result = 0;
    unsigned int bestResult = 0;
    unsigned int bestBestResult = 999999;

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
    printResults(result, bestBestResult);
}
