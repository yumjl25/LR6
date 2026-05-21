use std::io::{self, Write};

// Вывод поля
fn print_field(field: &Vec<String>, title: &str) {
    if !title.is_empty() {
        println!("{}:", title);
    }
    for row in field {
        for c in row.chars() {
            print!("{} ", c);
        }
        println!();
    }
    println!();
}

// Генерация целевой рамки
fn generate_target(n: usize, m: usize, edge_value: char) -> Vec<String> {
    let inner_value = if edge_value == '0' { '1' } else { '0' };
    let mut target = vec![edge_value.to_string().repeat(m); n];
    
    for i in 1..n-1 {
        for j in 1..m-1 {
            target[i].replace_range(j..j+1, &inner_value.to_string());
        }
    }
    target
}

// Решение задачи
fn solve_frame_problem(field: &mut Vec<String>) -> (i32, Vec<Vec<String>>, char) {
    let n = field.len();
    let m = field[0].len();
    
    let edges = ['0', '1'];
    let mut best_moves = -1;
    let mut best_edge = '0';
    let mut best_field = field.clone();
    let mut best_steps = Vec::new();
    
    for &edge in &edges {
        let mut temp_field = field.clone();
        let target = generate_target(n, m, edge);
        let mut temp_steps = Vec::new();
        temp_steps.push(temp_field.clone());
        let mut moves = 0;
        
        for i in 0..n {
            for j in 0..m {
                if temp_field[i].chars().nth(j).unwrap() != target[i].chars().nth(j).unwrap() {
                    if j + 1 < m {
                        // Переворот с соседом справа
                        let ch = temp_field[i].chars().nth(j).unwrap();
                        let new_ch = if ch == '0' { '1' } else { '0' };
                        temp_field[i].replace_range(j..j+1, &new_ch.to_string());
                        
                        let ch2 = temp_field[i].chars().nth(j+1).unwrap();
                        let new_ch2 = if ch2 == '0' { '1' } else { '0' };
                        temp_field[i].replace_range(j+1..j+2, &new_ch2.to_string());
                        
                        moves += 1;
                        temp_steps.push(temp_field.clone());
                    }
                    else if i + 1 < n {
                        // Переворот с соседом снизу
                        let ch = temp_field[i].chars().nth(j).unwrap();
                        let new_ch = if ch == '0' { '1' } else { '0' };
                        temp_field[i].replace_range(j..j+1, &new_ch.to_string());
                        
                        let ch2 = temp_field[i+1].chars().nth(j).unwrap();
                        let new_ch2 = if ch2 == '0' { '1' } else { '0' };
                        temp_field[i+1].replace_range(j..j+1, &new_ch2.to_string());
                        
                        moves += 1;
                        temp_steps.push(temp_field.clone());
                    }
                }
            }
        }
        
        if best_moves == -1 || moves < best_moves {
            best_moves = moves;
            best_edge = edge;
            best_field = temp_field;
            best_steps = temp_steps;
        }
    }
    
    *field = best_field;
    (best_moves, best_steps, best_edge)
}

fn main() {
    
    // Ввод размеров
    let (n, m) = loop {
        print!("Введите N M: ");
        io::stdout().flush().unwrap();
        
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        
        let nums: Vec<usize> = input
            .split_whitespace()
            .filter_map(|x| x.parse().ok())
            .collect();
        
        if nums.len() == 2 && nums[0] > 0 && nums[1] > 0 {
            break (nums[0], nums[1]);
        }
        println!("Ошибка! Введите положительные целые числа.");
    };
    
    // Ввод поля
    let mut field = Vec::new();
    println!("\nВведите поле (0 и 1 через пробел):");
    
    for i in 0..n {
        loop {
            print!("Строка {}: ", i + 1);
            io::stdout().flush().unwrap();
            
            let mut line = String::new();
            io::stdin().read_line(&mut line).unwrap();
            
            // Удаляем пробелы
            let cleaned: String = line.chars().filter(|&c| c != ' ').collect();
            let trimmed = cleaned.trim();
            
            if trimmed.len() != m {
                println!("Ошибка! Должно быть {} символов.", m);
                continue;
            }
            
            let valid = trimmed.chars().all(|c| c == '0' || c == '1');
            if !valid {
                println!("Ошибка! Только 0 и 1.");
                continue;
            }
            
            field.push(trimmed.to_string());
            break;
        }
    }
    
    println!();
    
    let (moves, steps, chosen_edge) = solve_frame_problem(&mut field);
    
    // Вывод промежуточных шагов
    for step in 1..steps.len() {
        println!("Шаг {}:", step);
        print_field(&steps[step], "");
    }
    
    // Вывод целевой рамки
    let target = generate_target(n, m, chosen_edge);
    let edge_type = if chosen_edge == '0' { "край = 0, внутри = 1" } else { "край = 1, внутри = 0" };
    println!("Целевая рамка ({}):", edge_type);
    print_field(&target, "");
    
    println!("Минимальное число ходов: {}", moves);
}