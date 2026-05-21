#ifndef SLAE_H
#define SLAE_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

// Утилиты
void printMatrix(const Matrix& A, const Vector& b, const string& title);
void printVector(const Vector& x, const string& title);

// Метод Гаусса
Vector gaussSolve(Matrix A, Vector b);

// Метод Зейделя
Vector zeidelSolve(const Matrix& A, const Vector& b, double eps, int maxIter = 1000);

#endif