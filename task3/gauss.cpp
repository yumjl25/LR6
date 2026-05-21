#include "slae.h"

void printMatrix(const Matrix& A, const Vector& b, const string& title) {
    cout << "\n" << title << ":\n";
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            cout << fixed << setw(10) << setprecision(4) << A[i][j] << " ";
        }
        cout << " | " << fixed << setw(10) << setprecision(4) << b[i] << endl;
    }
}

void printVector(const Vector& x, const string& title) {
    cout << title << ":\n";
    for (size_t i = 0; i < x.size(); i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(6) << x[i] << endl;
    }
}

Vector gaussSolve(Matrix A, Vector b) {
    int n = A.size();
    
    cout << "\nМетод Гаусса с выбором главного элемента\n";
    printMatrix(A, b, "Исходная система");
    
    // Прямой ход
    for (int k = 0; k < n - 1; k++) {
        // Поиск главного элемента
        int maxRow = k;
        double maxVal = fabs(A[k][k]);
        for (int i = k + 1; i < n; i++) {
            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }
        
        // Перестановка строк
        if (maxRow != k) {
            swap(A[k], A[maxRow]);
            swap(b[k], b[maxRow]);
            cout << "\nПерестановка строк " << k + 1 << " и " << maxRow + 1 << endl;
            printMatrix(A, b, "После перестановки");
        }
        
        // Исключение
        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
        printMatrix(A, b, "После шага " + to_string(k+1));
    }
    
    // Обратный ход
    Vector x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
    
    cout << "\n";
    printVector(x, "Результат Гаусса");
    
    return x;
}