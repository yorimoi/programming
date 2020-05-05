use crate::lexer;
use crate::token;
use crate::ast;
use crate::ast::Value;

// Parser holds a Lexer, errors, the current_token, and the peek peek_token (next token).
// Parser methods handle iterating through tokens and building and AST.
struct Parser {
    lexer: lexer::Lexer,
    errors: Vec<String>,
    current_token: token::Token,
    peek_token: token::Token,
}

impl Parser {
    // New takes a Lexer, creates a Parser with that Lexer, sets the current and
    // peek tokens, and returns the Parser.
    fn new(lexer: lexer::Lexer) -> Self {
        let mut p = Parser {
            lexer,
            errors: Vec::new(),
            current_token: token::Token::new_null(),
            peek_token: token::Token::new_null(),
        };

        // Read two tokens, so current_token and peek_token are both set.
        p.next_token();
        p.next_token();
        p
    }

    // next_token sets our current token to the peek token and the peek token to
    // p.lexer.next_token() which ends up scanning and returning the next token
    fn next_token(&mut self) {
        self.current_token = self.peek_token.clone();
        self.peek_token = self.lexer.next_token();
    }

    // parse_program parses tokens and creates an AST. It returns the RootNode
    // which holds a slice of Values (and in turn, the rest of the tree)
    fn parse_program(&mut self) -> Result<ast::RootNode, String> {
        let mut root_node = if self.current_token_type_is(token::LEFTBRACKET) {
            ast::RootNode { value: Value::Null, root_node_type: ast::RootNodeType::ArrayRoot }
        } else {
            ast::RootNode { value: Value::Null, root_node_type: ast::RootNodeType::ObjectRoot }
        };

        let val = self.parse_value();
        if val == Value::Null {
            return Err(format!("Error parsing JSON expected a value, got: {}",
                    self.current_token.literal));
        }

        root_node.value = val;

        Ok(root_node)
    }

    fn current_token_type_is(&self, t: token::Type) -> bool {
        self.current_token.token_type == t
    }

    // parse_value is our dynamic entrypoint to parsing JSON values. All scenarios for
    // this parser fall under these 3 actions.
    fn parse_value(&self) -> ast::Value {
        match self.current_token.token_type {
            token::LEFTBRACE => self.parse_JSON_Object(),
            token::LEFTBRACKET => self.parse_JSON_Array(),
            _ => self.parse_JSON_Literal(),
        }
    }

    // parse_JSON_Object is called when an open left brace `{` token is found
    fn parse_JSON_Object(&self) -> ast::Value {
        todo!();
    }

    // parse_JSON_Array is called when an open left bracket `[` token is found
    fn parse_JSON_Array(&self) -> ast::Value {
        todo!();
    }

    // parse_JSON_Literal switches on the current token's type, sets the Value on a return val and
    // returns it.
    fn parse_JSON_Literal(&self) -> ast::Value {
        todo!();
    }

    // parse_property is used to parse an object property and in doing so handles setting the
    // `key`:`value` pair.
    fn parse_property(&self) -> ast::Property {
        todo!();
    }

    fn parse_string(&self) -> String {
        todo!();
    }

    // expect_peek_type checks the next token type against the one passed in. If it matches,
    // we call p.next_token() to set us to the expected token and return true. If the expected
    // type does not match, we add a peek error and return false.
    fn expect_peek_type(&self, t: token::Type) -> bool {
        todo!();
    }

    fn peek_token_type_is(&self, t: token::Type) -> bool {
        todo!();
    }

    // peek_error is a small wrapper to add a peek error to our parser's errors field.
    fn peek_error(&self, t: token::Type) {
        todo!();
    }

    // parse_error is very similar to `peek_error`, except it simply takes a string message that
    // gets appended to the parser's errors
    fn parse_error(&self, msg: &str) {
        todo!();
    }

    // errors is simply a helper function that returns the parser's errors
    fn errors(&self) -> String {
        todo!();
    }
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_xxx() {
    }
}
