use super::token::*;

/// EBNF
/// expr    = mul ('+' mul | '-' mul)*;
/// mul     = unary ('*' unary | '/' unary)*;
/// unary   = ('+' | '-')? primary;
/// primary = num | '(' expr ')';
/// num     = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9';

#[derive(Debug, PartialEq)]
pub enum AstKind {
    ADD,      // +
    SUB,      // -
    MUL,      // *
    DIV,      // /
    NUM(i64), // [0-9][0-9]*
}

#[derive(Debug, PartialEq)]
pub struct Ast {
    pub kind: AstKind,
    pub lnode: Option<Box<Ast>>,
    pub rnode: Option<Box<Ast>>,
}

fn new_node(kind: AstKind, lnode: Option<Box<Ast>>, rnode: Option<Box<Ast>>) -> Option<Box<Ast>> {
    Some(Box::new(Ast { kind, lnode, rnode }))
}

//static mut t: Option<std::slice::Iter<Token>> = None;
//static mut t: Option<Vec<Token>> = None;
static mut T_IDX: usize = 0;

pub fn eval(mut tok: &mut Vec<Token>) -> Option<Box<Ast>> {
    unsafe {
        T_IDX = 0;
    }

    expr(&mut tok)
}

fn expr(mut tok: &mut Vec<Token>) -> Option<Box<Ast>> {
    let mut node = mul(&mut tok);

    loop {
        chaeck_illegal(&tok);
        if consume(TokenKind::PLUS, &tok) {
            node = new_node(AstKind::ADD, node, mul(&mut tok));
        } else if consume(TokenKind::MINUS, &tok) {
            node = new_node(AstKind::SUB, node, mul(&mut tok));
        } else {
            return node;
        }
    }
}

fn mul(mut tok: &mut Vec<Token>) -> Option<Box<Ast>> {
    let mut node = unary(&mut tok);

    loop {
        chaeck_illegal(&tok);
        if consume(TokenKind::ASTERISK, &tok) {
            node = new_node(AstKind::MUL, node, unary(&mut tok));
        } else if consume(TokenKind::SLASH, &tok) {
            node = new_node(AstKind::DIV, node, unary(&mut tok));
        } else {
            return node;
        }
    }
}

fn unary(mut tok: &mut Vec<Token>) -> Option<Box<Ast>> {
    if consume(TokenKind::PLUS, &tok) {
        return primary(&mut tok);
    }
    if consume(TokenKind::MINUS, &tok) {
        return new_node(AstKind::SUB, new_node(AstKind::NUM(0), None, None), primary(&mut tok));
    }
    return primary(&mut tok);
}

fn primary(mut tok: &mut Vec<Token>) -> Option<Box<Ast>> {
    if consume(TokenKind::LPAREN, &tok) {
        let node = expr(&mut tok);
        expect(TokenKind::RPAREN, &tok);
        return node;
    }

    new_node(AstKind::NUM(expect_number(&tok)), None, None)
}

fn consume(kind: TokenKind, tok: &Vec<Token>) -> bool {
    unsafe {
        if tok[T_IDX].kind == kind {
            T_IDX += 1;
            true
        } else {
            false
        }
    }
}

fn expect(kind: TokenKind, tok: &Vec<Token>) {
    unsafe {
        if tok[T_IDX].kind == kind {
            T_IDX += 1;
        } else {
            eprintln!("{}^", " ".repeat(tok[T_IDX].cur));
            panic!("expected {:?}, but got {:?}", kind, tok[T_IDX].kind);
        }
    }
}

fn expect_number(tok: &Vec<Token>) -> i64 {
    unsafe {
        match tok[T_IDX].kind {
            TokenKind::NUM(num) => {
                T_IDX += 1;
                num as i64
            },
            _ => {
                eprintln!("{}^", " ".repeat(tok[T_IDX].cur));
                panic!("expected NUM(u64). but got {:?}", tok[T_IDX].kind);
            }
        }
    }
}

fn chaeck_illegal(tok: &Vec<Token>) {
    unsafe {
        match &tok[T_IDX].kind {
            TokenKind::ILLEGAL(_) => {
                eprintln!("{}^", " ".repeat(tok[T_IDX].cur));
                panic!("illegal TokenKind {:?}", tok[T_IDX].kind);
            },
            _ => (),
        }
    }
}
