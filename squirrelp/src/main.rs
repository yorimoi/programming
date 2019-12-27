use std::collections::VecDeque;

fn tokenize(input: &str) -> VecDeque<String> {
    let mut tokens = VecDeque::new();
    for t in input.replace("(", " ( ").replace(")", " ) ").split_whitespace() {
        tokens.push_back(t.to_string());
    }
    tokens
}

fn read_from(tokens: &mut VecDeque<String>) ->VecDeque<String> {
    if tokens.len() == 0 {
        panic!("unexpected EOF while reading");
    }

    let token = tokens.pop_front().expect("nothing token");
    if "(" == token {
        let mut l: VecDeque<String> = VecDeque::new();
        while tokens[0] != ")" {
            l.append(&mut read_from(tokens));
        }
        tokens.pop_front();  // pop ')'
        l
    } else if ")" == token {
        panic!("SyntaxError: unexpected ')'");
    } else {
        VecDeque::from(vec![token])
    }
}

fn eval(mut x: &mut VecDeque<String>) -> Option<i32> {
    if let Some(token) = x.pop_front() {
        if token  == "print" {
            print!("{}", eval(&mut x).unwrap());
        } else if token == "+" {
            return Some(eval(&mut x).unwrap() + eval(&mut x).unwrap());
        } else if let Ok(num) = token.parse::<i32>() {
            return Some(num);
        } else {
            return None;
        }
    } else {
        return None;
    }

    eval(&mut x)
}

fn main() {
    let input = "(print (+ (+ 1 2) (+ 3 4)))";  // 10

    let mut tokens = tokenize(&input);
    let mut parse = read_from(&mut tokens);
    eval(&mut parse);
}

