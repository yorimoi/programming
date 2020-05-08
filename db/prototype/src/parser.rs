use crate::token;
use crate::lexer;
use crate::ast::*;

pub fn parse(source: &str) -> Result<Ast, String> {
    let mut tokens = lexer::Lexer::new(source);
    let mut node: Ast = Default::default();

    while !tokens.token().expect(token::TokenKind::EOF) {
        let stmt = match parse_statement(&mut tokens) {
            Ok(stmt) => stmt,
            Err(e) => return Err(e),
        };

        node.push_statement(stmt);

        if !tokens.token().expect(token::TokenKind::Symbol(token::SEMICOLON)) {
            return Err(error_message(token::TokenKind::Symbol(token::SEMICOLON), tokens.token()));
        }
    }

    Ok(node)
}

fn parse_statement(mut tokens: &mut lexer::Lexer) -> Result<Statement, String> {
    match tokens.token().kind {
        // SELECT
        token::TokenKind::Keyword(token::SELECT) => parse_select_statement(&mut tokens),
        // CREATE TABLE
        token::TokenKind::Keyword(token::CREATE) => parse_create_table_statement(&mut tokens),
        // INSERT
        token::TokenKind::Keyword(token::INSERT) => parse_insert_statement(&mut tokens),

        _ => Err(format!("{}: error: invalid keyword. got={:?}", tokens.token().line, tokens.token().kind)),
    }
}

fn parse_select_statement(mut tokens: &mut lexer::Lexer) -> Result<Statement, String> {
    tokens.next_token();
    let mut item: Vec<Expression> = Vec::new();

    if let token::TokenKind::Identifier(_) = tokens.token().kind {
        item.push(tokens.token().clone())
    } else {
        return Err(format!("{}: error: expect column name. got={:?}", tokens.token().line, tokens.token().kind));
    }

    tokens.next_token();
    while !tokens.token().expect(token::TokenKind::Keyword(token::FROM)) {
        // ,
        if !tokens.token().expect(token::TokenKind::Symbol(token::COMMA)) {
            return Err(error_message(token::TokenKind::Symbol(token::COMMA), tokens.token()));
        }

        // column
        tokens.next_token();
        if let token::TokenKind::Identifier(_) = tokens.token().kind {
            item.push(tokens.token().clone())
        } else {
            return Err(format!("{}: error: expect column name. got={:?}", tokens.token().line, tokens.token().kind));
        }
        tokens.next_token();
    }

    // Table name
    tokens.next_token();
    let table = if let token::TokenKind::Identifier(_) = tokens.token().kind {
        tokens.token().clone()
    } else {
        return Err(format!("{}: error: expect table name. got={:?}", tokens.token().line, tokens.token().kind));
    };

    let stmt = SelectStatement::new(item, table);
    Ok(Statement::new_select(stmt))
}

fn parse_create_table_statement(mut tokens: &mut lexer::Lexer) -> Result<Statement, String> {
    tokens.next_token();

    todo!();
}

fn parse_insert_statement(mut tokens: &mut lexer::Lexer) -> Result<Statement, String> {
    tokens.next_token();

    todo!();
}

fn error_message(expect: token::TokenKind, actual: &token::Token) -> String {
    format!("{}: error: expect={:?}. got={:?}", actual.line, expect, actual.kind)
}


#[cfg(test)]
mod tests {
    use super::*;
    use super::token::TokenKind;

    #[test]
    fn test_error_message() {
        assert_eq!(&error_message(TokenKind::Symbol(token::SEMICOLON),
            &token::Token::new(TokenKind::Symbol(token::LPAREN), 1)),
            r#"1: error: expect=Symbol(";"). got=Symbol("(")"#);
        assert_eq!(&error_message(TokenKind::Keyword(token::SELECT),
            &token::Token::new(TokenKind::Number(42), 99)),
            r#"99: error: expect=Keyword("select"). got=Number(42)"#);
        assert_eq!(&error_message(TokenKind::Symbol(token::SEMICOLON),
            &token::Token::new(TokenKind::EOF, 100)),
            r#"100: error: expect=Symbol(";"). got=EOF"#);
    }

    #[test]
    fn test_parse_statement() {
        struct test {
            input: &'static str,
            expect: Result<Statement, String>,
        }
        let tests = vec![
            test {
                input: "",
                expect: Err(r#"1: error: invalid keyword. got=EOF"#.to_string())
            },
            test {
                input: "invalidkeyword",
                expect: Err(r#"1: error: invalid keyword. got=Identifier("invalidkeyword")"#.to_string())
            },
            test {
                input: "select id, name from users;",
                expect: Ok(Statement {
                    select: Some(
                                SelectStatement {
                                    item: vec![token::Token {
                                        kind: token::TokenKind::Identifier("id".to_string()), line: 1,
                                    }, token::Token {
                                        kind: token::TokenKind::Identifier("name".to_string()), line: 1,
                                    }],
                                    from: token::Token {
                                        kind: token::TokenKind::Identifier("users".to_string()), line: 1,
                                    },
                                }),
                    create: None,
                    insert: None,
                    kind: AstKind::Select,
                }),
            },
            test {
                input: "select from users;",
                expect: Err(r#"1: error: expect column name. got=Keyword("from")"#.to_string())
            },
            test {
                input: "select id name from users;",
                expect: Err(r#"1: error: expect=Symbol(","). got=Identifier("name")"#.to_string())
            },
            test {
                input: "select id, from users;",
                expect: Err(r#"1: error: expect column name. got=Keyword("from")"#.to_string())
            },
            test {
                input: "select id, name from ;",
                expect: Err(r#"1: error: expect table name. got=Symbol(";")"#.to_string())
            },
        ];

        for test in tests.iter() {
            let mut l = lexer::Lexer::new(&test.input);
            l.next_token();
            assert_eq!(parse_statement(&mut l), test.expect);
        }
    }
}
