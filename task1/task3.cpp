#include "task3.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

const int DELAY_US = 400000; // 400 мс

int getGridSize() {
    int size;
    while (true) {
        cout << "Введите размер поля (10-20): ";
        cin >> size;
        
        if (cin.fail() || size < 10 || size > 20) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число от 10 до 20\n";
        } else {
            cin.ignore(10000, '\n');
            return size;
        }
    }
}

int countNeighbors(bool** grid, int size, int x, int y) {
    int cnt = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < size && ny >= 0 && ny < size)
                cnt += grid[nx][ny];
        }
    }
    return cnt;
}

void nextGeneration(bool** current, bool** next, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int neighbors = countNeighbors(current, size, i, j);
            if (current[i][j])
                next[i][j] = (neighbors == 2 || neighbors == 3);
            else
                next[i][j] = (neighbors == 3);
        }
    }
}

void printGrid(bool** grid, int size, int generation, const string& title) {
    system("clear");
    
    cout << title << endl;
    cout << "Поколение: " << generation << endl << endl;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << (grid[i][j] ? "1 " : "0 ");
        }
        cout << endl;
    }
}

void initEater(bool** grid, int size) {
    // Очистка поля
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = false;
    
    int center = size / 2;
    
    // Блок 2x2
    if (center-2 >= 0 && center-5 >= 0) {
        grid[center-2][center-5] = true;
        grid[center-2][center-4] = true;
        grid[center-1][center-5] = true;
        grid[center-1][center-4] = true;
    }
    
    // Пожиратель
    if (center+3 < size && center+2 < size) {
        grid[center][center+2] = true;
        grid[center+1][center+2] = true;
        grid[center+2][center] = true;
        grid[center+2][center+1] = true;
        grid[center+2][center+2] = true;
        grid[center+3][center+2] = true;
    }
    
    // Планер
    if (center-3 >= 0 && center+5 < size) {
        grid[center-3][center+5] = true;
        grid[center-2][center+3] = true;
        grid[center-2][center+5] = true;
        grid[center-1][center+4] = true;
        grid[center-1][center+5] = true;
    }
}

void initRandom(bool** grid, int size) {
    srand(time(nullptr));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = (rand() % 100 < 30);
}

void task3() {
    cout << "\nИгра Жизнь Конвея\n";
    
    int size = getGridSize();
    
    // Выделение памяти
    bool** grid = new bool*[size];
    bool** next = new bool*[size];
    for (int i = 0; i < size; i++) {
        grid[i] = new bool[size];
        next[i] = new bool[size];
    }
    
    // ПОЖИРАТЕЛЬ
    cout << "\nСценарий 1: Пожиратель (Eater) + Планер\n";
    cout << "Пожиратель — устойчивая фигура, уничтожающая планер\n";
    cout << "Нажмите Enter для запуска...";
    cin.get();
    
    initEater(grid, size);
    
    for (int gen = 0; gen < 40; gen++) {
        printGrid(grid, size, gen, "ПОЖИРАТЕЛЬ");
        usleep(DELAY_US);
        nextGeneration(grid, next, size);
        
        // Копирование
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                grid[i][j] = next[i][j];
    }
    
    // СЛУЧАЙНОЕ ПОЛЕ
    cout << "\nСценарий 2: Случайное поле\n";
    cout << "Нажмите Enter для запуска...";
    cin.get();
    
    initRandom(grid, size);
    
    for (int gen = 0; gen < 40; gen++) {
        printGrid(grid, size, gen, "СЛУЧАЙНОЕ ПОЛЕ");
        usleep(DELAY_US);
        nextGeneration(grid, next, size);
        
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                grid[i][j] = next[i][j];
    }
    
    // Очистка памяти
    for (int i = 0; i < size; i++) {
        delete[] grid[i];
        delete[] next[i];
    }
    delete[] grid;
    delete[] next;
    
    cout << "Демонстрация завершена!\n";
}