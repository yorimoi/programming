use crate::token;

pub struct Lexer<'a> {
    input: &'a str,
    len: usize,
    position: usize,
    peek_position: usize,
    ch: Option<char>,
}

impl<'a> Lexer<'a> {
    pub fn new(input: &'a str) -> Self {
        let mut lexer = Self {
            input,
            len: input.len(),
            position: 0,
            peek_position: 0,
            ch: None,
        };
        lexer.read_char();

        lexer
    }

    fn read_char(&mut self) {
        if self.len <= self.peek_position {
            self.ch = None;
        } else {
            self.ch = self.input.chars().nth(self.peek_position);
        }
        self.position = self.peek_position;
        self.peek_position += 1;
    }

    fn peek_char(&self) -> Option<char> {
        self.input.chars().nth(self.peek_position)
    }

    fn char(&self) -> Option<char> {
        self.ch
    }
}


#[cfg(test)]
mod tests {
use super::*;

    #[test]
    fn test_read_peek_char() {
        let input = "'OR 1=1";
        let mut l = Lexer::new(&input);

        assert_eq!(l.char(), Some('\''));
        assert_eq!(l.peek_char(), Some('O'));
        while l.char().is_some() {
            l.read_char();
        }
        assert_eq!(l.char(), None);
    }
}
