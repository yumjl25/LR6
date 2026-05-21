#include "frame.h"

// Создание рамки: край = edgeValue, внутри = противоположное
vector<string> generateTarget(int N, int M, char edgeValue) {
    char innerValue = (edgeValue == '0') ? '1' : '0';
    vector<string> target(N, string(M, edgeValue));
    for (int i = 1; i < N-1; i++) {
        for (int j = 1; j < M-1; j++) {
            target[i][j] = innerValue;
        }
    }
    return target;
}

// Решение: перебор двух вариантов рамки
int solveFrameProblem(vector<string>& field, vector<vector<string>>& steps, char& chosenEdge) {
    int N = field.size();
    int M = field[0].size();
    
    char edges[2] = {'0', '1'};
    int bestMoves = -1;
    char bestEdge = '0';
    vector<string> bestField;
    
    // Пробуем оба варианта
    for (int e = 0; e < 2; e++) {
        vector<string> tempField = field;
        vector<string> target = generateTarget(N, M, edges[e]);
        vector<vector<string>> tempSteps;
        tempSteps.push_back(tempField);
        int moves = 0;
        
        // Жадный проход по всем клеткам
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (tempField[i][j] != target[i][j]) {
                    // Исправляем парой с соседом (справа или снизу)
                    if (j + 1 < M) {
                        tempField[i][j] = (tempField[i][j] == '0') ? '1' : '0';
                        tempField[i][j+1] = (tempField[i][j+1] == '0') ? '1' : '0';
                        moves++;
                        tempSteps.push_back(tempField);
                    }
                    else if (i + 1 < N) {
                        tempField[i][j] = (tempField[i][j] == '0') ? '1' : '0';
                        tempField[i+1][j] = (tempField[i+1][j] == '0') ? '1' : '0';
                        moves++;
                        tempSteps.push_back(tempField);
                    }
                }
            }
        }
        
        // Выбираем лучший вариант (меньше ходов)
        if (bestMoves == -1 || moves < bestMoves) {
            bestMoves = moves;
            bestEdge = edges[e];
            bestField = tempField;
            steps = tempSteps;
        }
    }
    
    chosenEdge = bestEdge;
    field = bestField;
    return bestMoves;
}