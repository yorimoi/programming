
#[derive(Debug, PartialEq)]
pub enum TokenKind {
    PLUS,      // +
    MINUS,     // -
    ASTERISK,  // *
    SLASH,     // /
    LPAREN,    // (
    RPAREN,    // )
    NUM(u64),  // [0-9][0-9]*
    //LITERAL(String),

    ILLEGAL(String),
    EOF,
}

#[derive(Debug)]
pub struct Token {
    pub kind: TokenKind,
    pub cur: usize,
    // pub line: u32,
    //pub literal: String,
}
