#include "frame.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

void printField(const vector<string>& field) {
    for (const string& row : field) {
        for (char c : row) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    int N, M;
    
    while (true) {
        cout << "Введите N M: ";
        cin >> N >> M;
        
        if (cin.fail() || N <= 0 || M <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите положительные целые числа.\n";
        } else {
            break;
        }
    }
    
    cin.ignore();
    
    vector<string> field(N);
    cout << "Введите поле (0 и 1 через пробел):\n";
    
    for (int i = 0; i < N; i++) {
        while (true) {
            cout << "Строка " << i + 1 << ": ";
            string line;
            getline(cin, line);
            
            string cleaned;
            for (char c : line) {
                if (c != ' ') cleaned += c;
            }
            
            if ((int)cleaned.length() != M) {
                cout << "Ошибка! Должно быть " << M << " символов.\n";
                continue;
            }
            
            bool valid = true;
            for (char c : cleaned) {
                if (c != '0' && c != '1') {
                    valid = false;
                    break;
                }
            }
            
            if (!valid) {
                cout << "Ошибка! Только 0 и 1.\n";
                continue;
            }
            
            field[i] = cleaned;
            break;
        }
    }
    
    
    vector<vector<string>> steps;
    char chosenEdge;
    int moves = solveFrameProblem(field, steps, chosenEdge);
    
    // Выводим промежуточные шаги
    for (size_t step = 1; step < steps.size(); step++) {
        cout << "Шаг " << step << ":\n";
        printField(steps[step]);
    }
    
    // Выводим целевую рамку
    vector<string> target = generateTarget(N, M, chosenEdge);
    string edgeType = (chosenEdge == '0') ? "край = 0, внутри = 1" : "край = 1, внутри = 0";
    cout << "Целевая рамка (" << edgeType << "):\n";
    printField(target);
    
    cout << "Минимальное число ходов: " << moves << endl;
    
    return 0;
}