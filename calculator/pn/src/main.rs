use std::collections::VecDeque;

fn tokenize(input: &str) -> VecDeque<&str> {
    input.split_whitespace().collect::<VecDeque<_>>()
}

fn eval(mut tokens: &mut VecDeque<&str>) -> i32 {
    if let Some(token) = tokens.pop_front() {
        if let Ok(num) = token.parse::<i32>() {
            return num;
        } else {
            match token {
                "+" => return eval(&mut tokens) + eval(&mut tokens),
                "-" => return eval(&mut tokens) - eval(&mut tokens),
                "*" => return eval(&mut tokens) * eval(&mut tokens),
                "/" => return eval(&mut tokens) / eval(&mut tokens),
                "%" => return eval(&mut tokens) % eval(&mut tokens),
                _   => panic!("unknown operator: {}", token),
            }
        }
    } else {
        panic!("stack underflow");
    }
}

fn main() {
    let input = "/ - * 1 2 + 3 4 5";
    let mut tokens = tokenize(input);
    let val = eval(&mut tokens);

    println!("{}", val);
}
