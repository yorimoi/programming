use super::token::*;

pub struct Lexer {
    input: String,
    position: usize,
    read_position: usize,
    pub ch: Option<char>,
}

impl Lexer {
    pub fn new(input: &str) -> Self {
        let mut l = Lexer {
            input: input.to_string(),
            position: 0,
            read_position: 0,
            ch: None,
        };
        l.read_char();
        l
    }

    fn read_char(&mut self) {
        if self.read_position >= self.input.len() {
            self.ch = None;
        } else {
            self.ch = self.input.chars().nth(self.read_position);
        }
        self.position = self.read_position;
        self.read_position += 1;
    }

    fn peek_char(&self) -> Option<char> {
        self.input.chars().nth(self.read_position)
    }

    fn skip_space(&mut self) {
        while self.ch == Some(' ')  || self.ch == Some('\n') ||
              self.ch == Some('\r') || self.ch == Some('\t') {
            self.read_char();
        }
    }

    fn next_token(&mut self) -> Token {
        self.skip_space();

        let tok = match self.ch {
            Some('+') => new_token(TokenKind::PLUS,     self.read_position),
            Some('-') => new_token(TokenKind::MINUS,    self.read_position),
            Some('*') => new_token(TokenKind::ASTERISK, self.read_position),
            Some('/') => new_token(TokenKind::SLASH,    self.read_position),
            Some('(') => new_token(TokenKind::LPAREN,   self.read_position),
            Some(')') => new_token(TokenKind::RPAREN,   self.read_position),
            None      => new_token(TokenKind::EOF,      self.read_position),
            _ =>
            if is_digit(&self.ch) {
                let mut num = char2num(&self.ch);
                while is_digit(&self.peek_char()) {
                    num = num * 10 + char2num(&self.peek_char());
                    self.read_char();
                }
                new_token(TokenKind::NUM(num), self.read_position)
            } else {
                //eprintln!("{}^", " ".repeat(self.read_position + 1));
                //panic!("expected Num(u64), but got {}", &self.ch.unwrap());
                let str = String::from(&self.ch.unwrap().to_string());
                new_token(TokenKind::ILLEGAL(str), self.read_position + 1)
            },
        };
        self.read_char();
        tok
    }
}

fn new_token(kind: TokenKind, cur: usize) -> Token {
    Token { kind, cur }
}

fn is_digit(ch: &Option<char>) -> bool {
    let asc = match ch {
        Some(_) => ch.unwrap() as i32 - 48,
        None => -1,
    };
    0 <= asc && asc <= 9
}

fn char2num(ch: &Option<char>) -> u64 {
    ch.unwrap() as u64 - 48
}

pub fn tokenize(l: &mut Lexer) -> Vec<Token> {
    let mut tok = Vec::new();
    tok.push(l.next_token());
    while tok[tok.len()-1].kind != TokenKind::EOF {
        tok.push(l.next_token());
    }
    tok
}
