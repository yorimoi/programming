use crate::token;

// Lexer performs lexical analysis/scanning of the JSON
pub struct Lexer {
    pub input:         String,
    pub char:          Option<char>, // current char under examination
    pub position:      usize,        // current position in input (points to current char)
    pub read_position: usize,        // current reading position in input (after current char)
    pub line:          usize,        // line number for better error reporting, etc
}

// New creates and returns a pointer to the Lexer
impl Lexer {
    pub fn new(input: &str) -> Self {
        let mut l = Lexer {
            input: input.to_string(),
            char:          None,
            position:      0,
            read_position: 0,
            line:          1,
        };
        l.read_char();
        l
    }

    pub fn read_char(&mut self) {
        if self.input.len() <= self.read_position {
            // End of input (haven't read anything yet or EOF)
            self.char = None;
        } else {
            self.char = self.input.chars().nth(self.read_position);
        }

        self.position = self.read_position;
        self.read_position += 1;
    }

    // read_string sets a start position and reads through characters
    // When it finds a closing `"`, it stops consuming characters and
    // returns the string between the start and end positions.
    fn read_string(&mut self) -> String {
        let start_position = self.position + 1;

        loop {
            let prev_char = self.char;
            self.read_char();
            if (self.char == Some('"') && prev_char != Some('\\')) || self.char == None {
                break;
            }
        }

        self.input[start_position..self.position].to_string()
    }

    pub fn next_token(&mut self) -> token::Token {
        let t: token::Token = {
            self.skip_whitespace();

            match self.char {
                Some('{') =>
                    token::Token::new(token::LEFTBRACE,    self.line, self.position, self.position+1, "{"),
                Some('}') =>
                    token::Token::new(token::RIGHTBRACE,   self.line, self.position, self.position+1, "}"),
                Some('[') =>
                    token::Token::new(token::LEFTBRACKET,  self.line, self.position, self.position+1, "["),
                Some(']') =>
                    token::Token::new(token::RIGHTBRACKET, self.line, self.position, self.position+1, "]"),
                Some(':') =>
                    token::Token::new(token::COLON,        self.line, self.position, self.position+1, ":"),
                Some(',') =>
                    token::Token::new(token::COMMA,        self.line, self.position, self.position+1, ","),
                Some('"') =>
                    token::Token::new(token::STRING,       self.line, self.position, self.position+1, self.read_string()),
                None => token::Token::new(token::EOF,      self.line,             0,               0,  ""),
                // Other
                Some(ch) => {
                    if is_letter(ch) {
                        let start_position = self.position;
                        let ident = self.read_identifier();
                        match token::lookup_identifier(&ident) {
                            Ok(t) => {
                                return token::Token::new(t, self.line, start_position, self.position, &ident);
                            },
                            Err(e) => panic!(e),
                        }
                    } else if is_number(ch) {
                        let start_position = self.position;
                        let value = self.read_number();
                        return token::Token::new(token::NUMBER, self.line, start_position, self.position, &value);
                    }

                    token::Token::new(token::ILLEGAL, self.line, 0, 0,  ch.to_string())
                }
            }
        };

        self.read_char();
        t
    }

    fn skip_whitespace(&mut self) {
        while self.char == Some(' ') || self.char == Some('\t') || self.char == Some('\n') || self.char == Some('\r') {
            if self.char == Some('\n') {
                self.line += 1;
            }
            self.read_char();
        }
    }

    // read_number sets a start position and reads through characters. When it
    // finds a char that isn't a number, it stops consuming characters and
    // returns the string between the start and end positions.
    fn read_number(&mut self) -> String {
        let start_position = self.position;

        while self.char.is_some() {
            if !is_number(self.char.unwrap()) {
                break;
            }
            self.read_char();
        }

        self.input[start_position..self.position].to_string()
    }

    fn read_identifier(&mut self) -> String {
        let start_position = self.position;

        while is_letter(self.char.unwrap()) {
            self.read_char();
        }

        self.input[start_position..self.position].to_string()
    }
}

fn is_letter(ch: char) -> bool {
    ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z')
}

fn is_number(ch: char) -> bool {
    '0' <= ch && ch <= '9' || ch == '.' || ch == '-'
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_letter() {
        assert!(is_letter('A'));
        assert!(is_letter('Z'));
        assert!(is_letter('a'));
        assert!(is_letter('z'));
        assert!(!is_letter('@'));
        assert!(!is_letter('['));
        assert!(!is_letter('`'));
        assert!(!is_letter('{'));
    }

    #[test]
    fn test_is_number() {
        assert!(is_number('0'));
        assert!(is_number('9'));
        assert!(is_number('.'));
        assert!(is_number('-'));
        assert!(!is_number('/'));
        assert!(!is_number(':'));
    }

    #[test]
    fn test_read_char() {
        let input = r#"{ "someKey": "someValue", "anotherKey": [123, 456, "789"] }"#;
        let mut l = Lexer::new(&input);

        assert_eq!(l.char, Some('{'));
        assert_eq!(l.position, 0);
        assert_eq!(l.read_position, 1);
        assert_eq!(l.line, 1);

        l.read_char();
        assert_eq!(l.char, Some(' '));
        assert_eq!(l.position, 1);
        assert_eq!(l.read_position, 2);
        assert_eq!(l.line, 1);
    }

    #[test]
    fn test_read_string() {
        let input = r#"{ "someKey": "someValue" }"#;
        let mut l = Lexer::new(&input);

        l.read_char(); l.read_char();
        assert_eq!(l.read_string(), "someKey");

        l.read_char(); l.read_char(); l.read_char();
        assert_eq!(l.read_string(), "someValue");
    }

    #[test]
    fn test_read_number() {
        let input = r#"[123, 456, "789"]"#;
        let mut l = Lexer::new(&input);

        l.read_char();
        assert_eq!(l.read_number(), "123");
    }

    #[test]
    fn test_read_identifier() {
        let input = r#"[true, false, null]"#;
        let mut l = Lexer::new(&input);

        l.read_char();
        assert_eq!(l.read_identifier(), "true");
        l.read_char(); l.read_char();
        assert_eq!(l.read_identifier(), "false");
        l.read_char(); l.read_char();
        assert_eq!(l.read_identifier(), "null");
    }

    #[test]
    fn test_skip_whitespace() {
        let input = r#"{ "someKey":     "someValue" }"#;
        let mut l = Lexer::new(&input);

        l.read_char(); l.skip_whitespace();
        assert_eq!(l.read_string(), "someKey");

        l.read_char(); l.read_char(); l.skip_whitespace();
        assert_eq!(l.read_string(), "someValue");
    }

    #[test]
    fn test_next_token() {
        let input = r#" { } [ ] : , "str" true 123"#;
        let mut l = Lexer::new(&input);

        assert_eq!(l.next_token().token_type, token::LEFTBRACE);
        assert_eq!(l.next_token().token_type, token::RIGHTBRACE);
        assert_eq!(l.next_token().token_type, token::LEFTBRACKET);
        assert_eq!(l.next_token().token_type, token::RIGHTBRACKET);
        assert_eq!(l.next_token().token_type, token::COLON);
        assert_eq!(l.next_token().token_type, token::COMMA);
        assert_eq!(l.next_token().token_type, token::STRING);
        assert_eq!(l.next_token().token_type, token::TRUE);
        assert_eq!(l.next_token().token_type, token::NUMBER);
        assert_eq!(l.next_token().token_type, token::EOF);
    }
}

