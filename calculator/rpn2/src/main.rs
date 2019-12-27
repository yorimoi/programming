
fn tokenize(input: &str) -> Vec<&str> {
    input.split_whitespace().collect::<Vec<_>>()
}

fn parse(tokens: Vec<&str>) -> i32 {
    let mut stack = Vec::new();

    for token in tokens {
        if let Ok(num) = token.parse::<i32>() {
            stack.push(num);
        } else {
            match token {
                "+" => eval(&mut stack, |x, y| x + y),
                "-" => eval(&mut stack, |x, y| x - y),
                "*" => eval(&mut stack, |x, y| x * y),
                "/" => eval(&mut stack, |x, y| x / y),
                "%" => eval(&mut stack, |x, y| x % y),
                _   => panic!("unknown operator: {}", token),
            }
        }
    }

    stack.pop().expect("stack underflow")
}

fn eval<F>(stack: &mut Vec<i32>, fun: F)
where
    F: Fn(i32, i32) -> i32
{
    if let (Some(y), Some(x)) = (stack.pop(), stack.pop()) {
        let z = fun(x, y);
        stack.push(z);
    } else {
        panic!("stack underflow");
    }
}

fn main() {
    let input = "3 1 2 + 4 + 5 - 4 * * 6 / 3 %";

    let tokens = tokenize(input);
    let val = parse(tokens);

    println!("{:?}", val);
}

