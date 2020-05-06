
pub type Keyword = &'static str;
pub const SELECT: Keyword = "select";
pub const FROM  : Keyword = "from";
pub const AS    : Keyword = "as";
pub const TABLE : Keyword = "table";
pub const CREATE: Keyword = "create";
pub const INSERT: Keyword = "insert";
pub const INTO  : Keyword = "into";
pub const VALUES: Keyword = "values";
pub const INT   : Keyword = "int";
pub const TEXT  : Keyword = "text";
pub const NONE  : Keyword = "none";

pub type Symbol = &'static str;
pub const SEMICOLON : Symbol = ";";
pub const ASTERISK  : Symbol = "*";
pub const COMMA     : Symbol = ",";
pub const LEFTPAREN : Symbol = "(";
pub const RIGHTPAREN: Symbol = ")";

pub enum TokenKind {
    Keyword,
    Symbol,
    Identifier,
    String,
    Number,
    EOF,
}

pub struct Token {
    literal: String,
    kind: TokenKind,
    line: usize,
}
