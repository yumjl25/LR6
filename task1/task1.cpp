#include "task1.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int inputInt(const string& msg, int minVal, bool needEven = false) {
    int val;
    while (true) {
        cout << msg;
        cin >> val;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число.\n";
            continue;
        }
        
        cin.ignore(10000, '\n');
        
        if (val < minVal) {
            cout << "Ошибка! Число должно быть >= " << minVal << endl;
            continue;
        }
        
        if (needEven && val % 2 != 0) {
            cout << "Ошибка! Число должно быть чётным.\n";
            continue;
        }
        
        return val;
    }
}

void task1() {
    
    int N = inputInt("Введите N (чётное, >6): ", 8, true);
    int half = N / 2;
    
    // Создание и заполнение матрицы
    int** mat = new int*[N];
    for (int i = 0; i < N; i++)
        mat[i] = new int[N];
    
    srand(time(nullptr));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = rand() % 101;
    
    // Создание четверти
    int** quarter = new int*[half];
    for (int i = 0; i < half; i++)
        quarter[i] = new int[half];
    
    int sum = 0, maxVal = mat[0][0];
    
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            quarter[i][j] = mat[i][j];
            sum += quarter[i][j];
            if (quarter[i][j] > maxVal)
                maxVal = quarter[i][j];
        }
    }
    
    // Вывод
    cout << "\nИсходная матрица " << N << "x" << N << ":\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
    
    cout << "\nЛевая верхняя четверть (" << half << "x" << half << "):\n";
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++)
            cout << quarter[i][j] << "\t";
        cout << endl;
    }
    
    cout << "\nСумма элементов четверти: " << sum << endl;
    cout << "Максимальный элемент: " << maxVal << endl;
    
    // Очистка
    for (int i = 0; i < N; i++) delete[] mat[i];
    delete[] mat;
    for (int i = 0; i < half; i++) delete[] quarter[i];
    delete[] quarter;
}