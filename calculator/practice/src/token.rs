
#[derive(Clone, Debug, PartialEq)]
pub enum TokenKind {
    PLUS,      // +
    MINUS,     // -
    ASTERISK,  // *
    SLASH,     // /
    LPAREN,    // (
    RPAREN,    // )
    NUM(f64),  // [0-9][0-9]*.[0-9][0-9]*
    //LITERAL(String),

    ILLEGAL(String),
    EOF,
}

#[derive(Clone, Debug)]
pub struct Token {
    pub kind: TokenKind,
    pub cur: usize,
    // pub line: u32,
    //pub literal: String,
}
