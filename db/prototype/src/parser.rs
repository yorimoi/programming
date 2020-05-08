use crate::token;
use crate::lexer;
use crate::ast::*;

pub fn parse(source: &str) -> Result<Ast, String> {
    let tokens = lexer::Lexer::new(source);
    let mut node: Ast = Default::default();

    while tokens.token().kind != token::TokenKind::EOF {
        let stmt = match parse_statement(&tokens) {
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

fn parse_statement(tokens: &lexer::Lexer) -> Result<Statement, String> {
    match tokens.token().kind {
        // SELECT
        token::TokenKind::Keyword(token::SELECT) => parse_select_statement(tokens),
        // CREATE TABLE
        token::TokenKind::Keyword(token::CREATE) => parse_create_table_statement(tokens),
        // INSERT
        token::TokenKind::Keyword(token::INSERT) => parse_insert_statement(tokens),

        _ => Err(format!("{}: error: {:?}", tokens.token().line, tokens.token().kind)),
    }
}

fn parse_select_statement(tokens: &lexer::Lexer) -> Result<Statement, String> {
    todo!();
}

fn parse_create_table_statement(tokens: &lexer::Lexer) -> Result<Statement, String> {
    todo!();
}

fn parse_insert_statement(tokens: &lexer::Lexer) -> Result<Statement, String> {
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
        let input = "select";
        let mut l = lexer::Lexer::new(&input);
    }
}
