use std::io::*;

fn main() {
    'outer: loop {
        print!(">> ");
        stdout().flush().unwrap();

        let mut input = String::new();
        stdin().read_line(&mut input).unwrap();

        let mut stack: Vec<i32> = Vec::new();
        for token in input.split_whitespace() {
            if let Ok(num) = token.parse::<i32>() {
                stack.push(num);
            } else {
                match token {
                    "+" => calc(&mut stack, |x, y| x + y),
                    "-" => calc(&mut stack, |x, y| x - y),
                    "*" => calc(&mut stack, |x, y| x * y),
                    "/" => calc(&mut stack, |x, y| x / y),
                    "%" => calc(&mut stack, |x, y| x % y),
                    "quit" | "exit" => break 'outer,
                    _ => panic!("Invalid operator: \"{}\"", token),
                }
            }
        }

        println!("{}", stack.pop().unwrap());
    }
}

fn calc<F>(stack: &mut Vec<i32>, f: F)
where
    F: Fn(i32, i32) -> i32,
{
    if let (Some(x), Some(y)) = (stack.pop(), stack.pop()) {
        stack.push(f(y, x));
    } else {
        panic!("Stack underflow");
    }
}
