#include "slae.h"

int main() {
    cout << "   Решение СЛАУ методом Гаусса и Зейделя\n";
    
    // M = 0.87, N = -0.19, P = 1.08
    Matrix A = {
        { 0.87, -0.04,  0.21, -1.16},
        { 0.25, -1.23, -0.19, -0.09},
        {-0.21, -0.19,  0.80, -0.13},
        { 0.15, -1.31,  0.06,  1.08}
    };
    
    Vector b = {-1.24, 1.08, 2.56, 0.87};
    
    // Метод Гаусса
    Vector x_gauss = gaussSolve(A, b);
    
    // Метод Зейделя
    double eps = 1e-3;
    Vector x_zeidel = zeidelSolve(A, b, eps);
    
    // Сравнение результатов
    cout << "\nСравнение результатов\n";
    cout << "  Переменная  |    Метод Гаусса    |   Метод Зейделя   |    Разница\n";
    cout << "--------------|--------------------|--------------------|---------------\n";
    for (int i = 0; i < 4; i++) {
        double diff = fabs(x_gauss[i] - x_zeidel[i]);
        cout << "     x" << i+1 << "      |  " << fixed << setprecision(6) << setw(14) << x_gauss[i]
             << "   |  " << setw(14) << x_zeidel[i]
             << "   |  " << setw(12) << diff << endl;
    }
    
    // Корни с верными знаками
    cout << "\nКорни с верными знаками\n";
    for (int i = 0; i < 4; i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(6) << x_gauss[i] << endl;
    }
    
    return 0;
}