#include "task2.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int inputInt2(const string& msg, int minVal) {
    int val;
    while (true) {
        cout << msg;
        cin >> val;
        
        if (cin.fail() || val <= minVal) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число > " << minVal << endl;
        } else {
            cin.ignore(10000, '\n');
            return val;
        }
    }
}

int sumDigits(int n) {
    int s = 0;
    n = abs(n);
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

void task2() {
    
    int M = inputInt2("Введите M (>5): ", 5);
    int N = inputInt2("Введите N (>5): ", 5);
    
    int** mat = new int*[M];
    for (int i = 0; i < M; i++)
        mat[i] = new int[N];
    
    srand(time(nullptr));
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = 1000 + rand() % 4001;
    
    cout << "\nИсходная матрица " << M << "x" << N << ":\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
    
    int* rowSums = new int[M];
    int minSum = 0, minRow = 0;
    
    cout << "\nСтрока | Сумма цифр\n";
    cout << "-------+-----------\n";
    
    for (int i = 0; i < M; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++)
            sum += sumDigits(mat[i][j]);
        rowSums[i] = sum;
        cout << "   " << i + 1 << "   |    " << sum << endl;
        
        if (i == 0 || sum < minSum) {
            minSum = sum;
            minRow = i;
        }
    }
    
    cout << "\nСтрока с минимальной суммой цифр: " << minRow + 1 << endl;
    
    int* rowArr = new int[N];
    for (int j = 0; j < N; j++)
        rowArr[j] = mat[minRow][j];
    
    cout << "Элементы этой строки: ";
    for (int j = 0; j < N; j++)
        cout << rowArr[j] << " ";
    cout << endl;
    
    // Очистка
    for (int i = 0; i < M; i++) delete[] mat[i];
    delete[] mat;
    delete[] rowSums;
    delete[] rowArr;
}