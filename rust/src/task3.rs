type Matrix = Vec<Vec<f64>>;
type Vector = Vec<f64>;

//Утилиты

fn print_matrix(a: &Matrix, b: &Vector, title: &str) {
    println!("\n{}:", title);
    for i in 0..a.len() {
        for j in 0..a[i].len() {
            print!("{:10.4} ", a[i][j]);
        }
        println!("| {:10.4}", b[i]);
    }
}

fn print_vector(x: &Vector, title: &str) {
    println!("\n{}:", title);
    for i in 0..x.len() {
        println!("x{} = {:.6}", i + 1, x[i]);
    }
}

// Метод Гаусса

fn gauss_solve(mut a: Matrix, mut b: Vector) -> Vector {
    let n = a.len();
    
    println!("\nМетод Гаусса с выбором главного элемента");
    print_matrix(&a, &b, "Исходная система");
    
    // Прямой ход
    for k in 0..n - 1 {
        // Поиск главного элемента
        let mut max_row = k;
        let mut max_val = a[k][k].abs();
        for i in k + 1..n {
            if a[i][k].abs() > max_val {
                max_val = a[i][k].abs();
                max_row = i;
            }
        }
        
        // Перестановка строк
        if max_row != k {
            a.swap(k, max_row);
            b.swap(k, max_row);
            println!("\nПерестановка строк {} и {}", k + 1, max_row + 1);
            print_matrix(&a, &b, "После перестановки");
        }
        
        // Исключение
        for i in k + 1..n {
            let factor = a[i][k] / a[k][k];
            for j in k..n {
                a[i][j] -= factor * a[k][j];
            }
            b[i] -= factor * b[k];
        }
        print_matrix(&a, &b, &format!("После шага {}", k + 1));
    }
    
    // Обратный ход
    let mut x = vec![0.0; n];
    for i in (0..n).rev() {
        let mut sum = 0.0;
        for j in i + 1..n {
            sum += a[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / a[i][i];
    }
    
    println!();
    print_vector(&x, "Результат Гаусса");
    
    x
}

// Метод Зейделя

fn vector_norm(a: &Vector, b: &Vector) -> f64 {
    a.iter()
        .zip(b.iter())
        .map(|(ai, bi)| (ai - bi).abs())
        .fold(0.0, |max, diff| max.max(diff))
}

fn zeidel_solve(a: &Matrix, b: &Vector, eps: f64, max_iter: usize) -> Vector {
    let n = a.len();
    
    println!("\nМетод Зейделя");
    
    // Канонический вид x = C*x + f
    let mut c = vec![vec![0.0; n]; n];
    let mut f = vec![0.0; n];
    
    for i in 0..n {
        for j in 0..n {
            if i != j {
                c[i][j] = -a[i][j] / a[i][i];
            }
        }
        f[i] = b[i] / a[i][i];
    }
    
    println!("Матрица C (x = C*x + f):");
    for i in 0..n {
        for j in 0..n {
            print!("{:10.4} ", c[i][j]);
        }
        println!();
    }
    
    println!("\nВектор f:");
    for i in 0..n {
        println!("f{} = {:.4}", i + 1, f[i]);
    }
    
    // Проверка условия сходимости
    println!("\nПроверка условия сходимости");
    let mut norm = 0.0;
    for i in 0..n {
        let row_sum: f64 = c[i].iter().map(|x| x.abs()).sum();
        println!("Строка {}: сумма модулей = {:.4} {}", 
                 i + 1, row_sum, if row_sum < 1.0 { "< 1 (хорошо)" } else { ">= 1 (может не сойтись)" });
        if row_sum > norm {
            norm = row_sum;
        }
    }
    println!("Норма матрицы C = {:.4}", norm);
    
    // Итерации Зейделя
    let mut x = vec![0.0; n];
    let mut x_prev = vec![0.0; n];
    
    println!("\nТаблица итераций");
    println!("  N  |     x1       |     x2       |     x3       |     x4       |     ε");
    println!("-----|--------------|--------------|--------------|--------------|------------");
    
    print!("{:5} ", 0);
    for i in 0..n {
        print!("{:12.6}  ", x[i]);
    }
    println!("{:12}", "---");
    
    for iter in 1..=max_iter {
        x_prev.clone_from(&x);
        
        for i in 0..n {
            let mut sum = 0.0;
            for j in 0..n {
                if j != i {
                    sum += c[i][j] * x[j];
                }
            }
            x[i] = sum + f[i];
        }
        
        let error = vector_norm(&x, &x_prev);
        
        print!("{:5} ", iter);
        for i in 0..n {
            print!("{:12.6}  ", x[i]);
        }
        println!("{:12.6}", error);
        
        if error < eps {
            println!("\nДостигнута точность ε = {:.6} на итерации {}", eps, iter);
            break;
        }
    }
    
    println!();
    print_vector(&x, "Результат Зейделя");
    
    x
}

// main

fn main() {
    println!("   Решение Слау методом Гаусса и Зейделя");
    
    // M = 0.87, N = -0.19, P = 1.08
    let a: Matrix = vec![
        vec![ 0.87, -0.04,  0.21, -1.16],
        vec![ 0.25, -1.23, -0.19, -0.09],
        vec![-0.21, -0.19,  0.80, -0.13],
        vec![ 0.15, -1.31,  0.06,  1.08],
    ];
    
    let b: Vector = vec![-1.24, 1.08, 2.56, 0.87];
    
    // Метод Гаусса
    let x_gauss = gauss_solve(a.clone(), b.clone());
    
    // Метод Зейделя
    let eps = 1e-3;
    let x_zeidel = zeidel_solve(&a, &b, eps, 1000);
    
    // Сравнение результатов
    println!("\nСравнение результатов");
    println!("  Переменная  |    Метод Гаусса    |   Метод Зейделя   |    Разница");
    println!("--------------|--------------------|--------------------|---------------");
    for i in 0..4 {
        let diff = (x_gauss[i] - x_zeidel[i]).abs();
        println!("     x{}      |  {:14.6}   |  {:14.6}   |  {:12.6}", 
                 i + 1, x_gauss[i], x_zeidel[i], diff);
    }
    
    // Корни с верными знаками
    println!("\nКорни с верными знаками");
    for i in 0..4 {
        println!("x{} = {:.6}", i + 1, x_gauss[i]);
    }
}