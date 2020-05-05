use crate::lexer;
use crate::token;
use crate::ast;
use crate::ast::{Value, State};

// Parser holds a Lexer, errors, the current_token, and the peek peek_token (next token).
// Parser methods handle iterating through tokens and building and AST.
pub struct Parser {
    lexer: lexer::Lexer,
    errors: Vec<String>,
    current_token: token::Token,
    peek_token: token::Token,
}

impl Parser {
    // New takes a Lexer, creates a Parser with that Lexer, sets the current and
    // peek tokens, and returns the Parser.
    pub fn new(lexer: lexer::Lexer) -> Self {
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
    pub fn parse_program(&mut self) -> Result<ast::RootNode, String> {
        let mut root_node = if self.current_token_type_is(token::LEFTBRACKET) {
            ast::RootNode { value: Value::Null, root_node_type: ast::RootNodeType::ArrayRoot }
        } else {
            ast::RootNode { value: Value::Null, root_node_type: ast::RootNodeType::ObjectRoot }
        };

        let val = self.parse_value();
        if val.is_none() {
            self.parse_error(format!("Error parsing JSON expected a value, got: {}",
                    self.current_token.literal));
            return Err(self.errors());
        }

        root_node.value = val.unwrap();

        Ok(root_node)
    }

    fn current_token_type_is(&self, t: token::Type) -> bool {
        self.current_token.token_type == t
    }

    // parse_value is our dynamic entrypoint to parsing JSON values. All scenarios for
    // this parser fall under these 3 actions.
    fn parse_value(&mut self) -> Option<ast::Value> {
        match self.current_token.token_type {
            token::LEFTBRACE => self.parse_JSON_Object(),  // {
            token::LEFTBRACKET => self.parse_JSON_Array(), // [
            _ => self.parse_JSON_Literal(),
        }
    }

    // parse_JSON_Object is called when an open left brace `{` token is found
    fn parse_JSON_Object(&mut self) -> Option<ast::Value> {
        let mut obj: ast::Object = ast::Node::new();
        let mut obj_state = State::ObjStart;

        while !self.current_token_type_is(token::EOF) {
            match obj_state {
                State::ObjStart => {
                    if self.current_token_type_is(token::LEFTBRACE) {
                        obj_state = State::ObjOpen;
                        obj.start = self.current_token.start;
                        self.next_token();
                    } else {
                        self.parse_error(format!("Error parsing JSON object Expected `{}` token, got: {}",
                                '{', self.current_token.literal));
                        return None;
                    }
                },
                State::ObjOpen => {
                    if self.current_token_type_is(token::RIGHTBRACE) {
                        self.next_token();
                        obj.end = self.current_token.end;
                        return Some(Value::Object(obj))
                    }
                    let prop = self.parse_property();
                    obj.children.push(prop);
                    obj_state = State::ObjProperty;
                },
                State::ObjProperty => {
                    if self.current_token_type_is(token::RIGHTBRACE) {
                        self.next_token();
                        obj.end = self.current_token.start;
                        return Some(Value::Object(obj))
                    } else if self.current_token_type_is(token::COMMA) {
                        obj_state = State::ObjComma;
                        self.next_token();
                    } else {
                        self.parse_error(
                            format!("Error parsing property. Expected RightBrace or Comma token, got: {}",
                                self.current_token.literal)
                        );
                        return None;
                    }
                },
                State::ObjComma => {
                    let prop = self.parse_property();
                    if prop.value != None {
                        obj.children.push(prop);
                        obj_state = State::ObjProperty;
                    }
                },
                _ => return None,
            }
        }

        obj.end = self.current_token.start;

        Some(Value::Object(obj))
    }

    // parse_JSON_Array is called when an open left bracket `[` token is found
    fn parse_JSON_Array(&mut self) -> Option<ast::Value> {
        let mut array: ast::Array = ast::Node::new();
        let mut array_state = State::ArrayStart;

        while !self.current_token_type_is(token::EOF) {
            match array_state {
                State::ArrayStart => {
                    if self.current_token_type_is(token::LEFTBRACKET) {
                        array.start = self.current_token.start;
                        array_state = State::ArrayOpen;
                        self.next_token();
                    }
                },
                State::ArrayOpen => {
                    if self.current_token_type_is(token::RIGHTBRACKET) {
                        array.end = self.current_token.end;
                        self.next_token();
                        return Some(Value::Array(array))
                    }
                    let val = self.parse_value();
                    array.children.push(val);
                    array_state = State::ArrayValue;
                    if self.peek_token_type_is(token::RIGHTBRACKET) {
                        self.next_token();
                    }
                },
                State::ArrayValue => {
                    if self.current_token_type_is(token::RIGHTBRACKET) {
                        array.end = self.current_token.end;
                        self.next_token();
                        return Some(Value::Array(array))
                    } else if self.current_token_type_is(token::COMMA) {
                        array_state = State::ArrayComma;
                        self.next_token();
                    } else {
                        self.parse_error(
                            format!("Error parsing property. Expected RightBrace or Comma token, got: {}",
                                self.current_token.literal)
                        );
                    }
                },
                State::ArrayComma => {
                    let val = self.parse_value();
                    array.children.push(val);
                    array_state = State::ArrayValue;
                },
                _ => return None,
            }
        }

        array.end = self.current_token.start;

        Some(Value::Array(array))
    }

    // parse_JSON_Literal switches on the current token's type, sets the Value on a return val and
    // returns it.
    fn parse_JSON_Literal(&mut self) -> Option<ast::Value> {
        let mut literal: ast::Literal = ast::Node::new();

        match self.current_token.token_type {
            token::STRING =>
                literal.value = Box::new(Value::String(self.parse_string())),
            token::NUMBER =>
                literal.value = Box::new(
                                    Value::Number(self.current_token.literal.parse::<i64>().unwrap())),
            token::TRUE =>
                literal.value = Box::new(Value::Bool(true)),
            token::FALSE =>
                literal.value = Box::new(Value::Bool(false)),
            _ => (), // Do nothing
        }

        self.next_token();
        Some(Value::Literal(literal))
    }

    // parse_property is used to parse an object property and in doing so handles setting the
    // `key`:`value` pair.
    fn parse_property(&mut self) -> ast::Property {
        let mut prop: ast::Property = ast::Node::new();
        let mut property_state = State::PropertyStart;

        while !self.current_token_type_is(token::EOF) {
            match property_state {
                State::PropertyStart => {
                    if self.current_token_type_is(token::STRING) {
                        let key = ast::Identifier {
                            identifier_type: "Identifier",
                            value: self.parse_string(),
                        };
                        prop.key = Some(key);
                        property_state = State::PropertyKey;
                        self.next_token();
                    } else {
                        self.parse_error(
                            format!("Error parsing property start. Expected String token, got: {}",
                                self.current_token.literal)
                        );
                    }
                },
                State::PropertyKey => {
                    if self.current_token_type_is(token::COLON) {
                        property_state = State::PropertyColon;
                        self.next_token();
                    } else {
                        self.parse_error(
                            format!("Error parsing property start. Expected Colon token, got: {}",
                                self.current_token.literal)
                        );
                    }
                },
                State::PropertyColon => {
                    let val = self.parse_value();
                    prop.value = val;
                    return prop;
                },
                _ => {
                    panic!("unreachable");
                },
            }
        }

        prop
    }

    fn parse_string(&self) -> String {
        self.current_token.literal.to_string()
    }

    // expect_peek_type checks the next token type against the one passed in. If it matches,
    // we call p.next_token() to set us to the expected token and return true. If the expected
    // type does not match, we add a peek error and return false.
    fn expect_peek_type(&mut self, t: token::Type) -> bool {
        if self.current_token_type_is(t) {
            self.next_token();
            return true;
        }
        self.peek_error(t);
        false
    }

    fn peek_token_type_is(&self, t: token::Type) -> bool {
        self.peek_token.token_type == t
    }

    // peek_error is a small wrapper to add a peek error to our parser's errors field.
    fn peek_error(&mut self, t: token::Type) {
        let msg = format!("Line: {}: Expected next token to be {}, got: {} instead",
                    self.current_token.line, t, self.peek_token.token_type);
        self.errors.push(msg);
    }

    // parse_error is very similar to `peek_error`, except it simply takes a string message that
    // gets appended to the parser's errors
    fn parse_error(&mut self, msg: String) {
        self.errors.push(msg);
    }

    // errors is simply a helper function that returns the parser's errors
    fn errors(&self) -> String {
        self.errors.join("\n")
    }
}

