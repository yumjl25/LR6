#include "slae.h"

// Норма разности двух векторов (максимальная разница)
double vectorNorm(const Vector& a, const Vector& b) {
    double maxDiff = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        double diff = fabs(a[i] - b[i]);
        if (diff > maxDiff) maxDiff = diff;
    }
    return maxDiff;
}

// Приведение к каноническому виду и проверка сходимости
void toCanonicalForm(const Matrix& A, const Vector& b, Matrix& C, Vector& f) {
    int n = A.size();
    C.assign(n, Vector(n, 0.0));
    f.assign(n, 0.0);
    
    cout << "\nПриведение к каноническому виду\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                C[i][j] = -A[i][j] / A[i][i];
            }
        }
        f[i] = b[i] / A[i][i];
    }
    
    cout << "Матрица C (x = C*x + f):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << fixed << setw(10) << setprecision(4) << C[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nВектор f:\n";
    for (int i = 0; i < n; i++) {
        cout << "f" << i+1 << " = " << fixed << setprecision(4) << f[i] << endl;
    }
    
    // Проверка условия сходимости
    cout << "\nПроверка условия сходимости\n";
    double maxRowSum = 0.0;
    for (int i = 0; i < n; i++) {
        double rowSum = 0.0;
        for (int j = 0; j < n; j++) {
            rowSum += fabs(C[i][j]);
        }
        cout << "Строка " << i+1 << ": сумма модулей = " << rowSum;
        if (rowSum < 1.0) cout << " < 1 (хорошо)";
        else cout << " >= 1 (может не сойтись)";
        cout << endl;
        if (rowSum > maxRowSum) maxRowSum = rowSum;
    }
    cout << "Норма матрицы C = " << maxRowSum << endl;
}

Vector zeidelSolve(const Matrix& A, const Vector& b, double eps, int maxIter) {
    cout << "\nМетод Зейделя\n";
    
    Matrix C;
    Vector f;
    toCanonicalForm(A, b, C, f);
    
    int n = A.size();
    Vector x(n, 0.0);      // начальное приближение
    Vector x_prev(n, 0.0);
    
    cout << "\nТаблица итераций\n";
    cout << "  N  |     x1       |     x2       |     x3       |     x4       |     ε\n";
    cout << "-----|--------------|--------------|--------------|--------------|------------\n";
    
    // Вывод начального приближения
    cout << fixed;
    cout << setw(5) << 0 << "  ";
    for (int i = 0; i < n; i++) {
        cout << setw(12) << setprecision(6) << x[i] << "  ";
    }
    cout << setw(12) << "---" << endl;
    
    for (int iter = 1; iter <= maxIter; iter++) {
        x_prev = x;
        
        // Итерация Зейделя
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum += C[i][j] * x[j];
                }
            }
            x[i] = sum + f[i];
        }
        
        double error = vectorNorm(x, x_prev);
        
        cout << setw(5) << iter << "  ";
        for (int i = 0; i < n; i++) {
            cout << setw(12) << setprecision(6) << x[i] << "  ";
        }
        cout << setw(12) << setprecision(6) << error << endl;
        
        if (error < eps) {
            cout << "\nДостигнута точность ε = " << eps << " на итерации " << iter << endl;
            break;
        }
    }
    
    cout << "\n";
    printVector(x, "Результат Зейделя");
    
    return x;
}