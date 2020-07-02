use lazy_static::lazy_static;
use std::collections::HashMap;

// Type is a type alias for a string
pub type Type = &'static str;

// Token is a struct representing a JSON token - It holds information like its Type and Literal, as well
// as Start, End, and Line fields. Line is used for better error handling, while Start and End are used
// to return objects/arrays from querys.
#[allow(non_snake_case)]
#[derive(Clone, PartialEq)]
pub struct Token {
    pub token_type:    Type,
    pub literal: String,
    pub line:    usize,
    pub start:   usize,
    pub end:     usize,
}

impl Token {
    pub fn new(token_type: Type, line: usize, start: usize, end: usize, literal: impl Into<String>) -> Self {
        Token { token_type, literal: literal.into(), line, start, end }
    }

    pub fn new_null() -> Self {
        Token { token_type: NULL, literal: "".to_string(), line:0, start:0, end:0 }
    }
}


// All the different tokens for supporting JSON
// Token/character we don't know about
pub const ILLEGAL: Type = "ILLEGAL";

// End of file
pub const EOF: Type = "EOF";

// Literals
pub const STRING: Type = "STRING";
pub const NUMBER: Type = "NUMBER";

// The six structural tokens
pub const LEFTBRACE:    Type = "{";
pub const RIGHTBRACE:   Type = "}";
pub const LEFTBRACKET:  Type = "[";
pub const RIGHTBRACKET: Type = "]";
pub const COMMA:        Type = ",";
pub const COLON:        Type = ":";

// Values
pub const TRUE:  Type = "TRUE";
pub const FALSE: Type = "FALSE";
pub const NULL:  Type = "NULL";

lazy_static! {
    static ref VALID_JSON_IDENTIFIERS: HashMap<&'static str, Type> = {
        let mut m = HashMap::new();
        m.insert("true",  TRUE);
        m.insert("false", FALSE);
        m.insert("null",  NULL);
        m
    };
}

// lookup_identifier checks our VALID_JSON_IDENTIFIERS map for the scanned identifier. If it finds one,
// the identifier's token type is returned. If not found, an error is returned
pub fn lookup_identifier(ident: &str) -> Result<Type, String> {
    if let Some(t) = VALID_JSON_IDENTIFIERS.get(ident) {
        Ok(t)
    } else {
        Err(format!("Expected a valid JSON identifier. But found: {}", ident))
    }
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lookup_identifier() {
        assert_eq!(lookup_identifier("true"),  Ok(TRUE));
        assert_eq!(lookup_identifier("false"), Ok(FALSE));
        assert_eq!(lookup_identifier("null"),  Ok(NULL));

        assert!(lookup_identifier("TRUE").is_err());
        assert!(lookup_identifier("hoge").is_err());
        assert!(lookup_identifier("").is_err());
    }
}

