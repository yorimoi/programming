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
    NUM(f64), // [0-9][0-9]*.[0-9][0-9]*
}

#[derive(Debug, PartialEq)]
pub struct Ast {
    pub kind: AstKind,
    pub lnode: Option<Box<Ast>>,
    pub rnode: Option<Box<Ast>>,
}

#[derive(Debug)]
struct Tokens {
    token: Vec<Token>,
    idx: usize,
}

fn new_node(kind: AstKind, lnode: Option<Box<Ast>>, rnode: Option<Box<Ast>>) -> Option<Box<Ast>> {
    Some(Box::new(Ast { kind, lnode, rnode }))
}

pub fn eval(tok: &Vec<Token>) -> Option<Box<Ast>> {
    let mut tok = Tokens {
        token: tok.clone(),
        idx: 0,
    };

    expr(&mut tok)
}

fn expr(mut tok: &mut Tokens) -> Option<Box<Ast>> {
    let mut node = mul(&mut tok);

    loop {
        tok.check_illegal();
        if consume(TokenKind::PLUS, &mut tok) {
            node = new_node(AstKind::ADD, node, mul(&mut tok));
        } else if consume(TokenKind::MINUS, &mut tok) {
            node = new_node(AstKind::SUB, node, mul(&mut tok));
        } else {
            return node;
        }
    }
}

fn mul(mut tok: &mut Tokens) -> Option<Box<Ast>> {
    let mut node = unary(&mut tok);

    loop {
        tok.check_illegal();
        if consume(TokenKind::ASTERISK, &mut tok) {
            node = new_node(AstKind::MUL, node, unary(&mut tok));
        } else if consume(TokenKind::SLASH, &mut tok) {
            node = new_node(AstKind::DIV, node, unary(&mut tok));
        } else {
            return node;
        }
    }
}

fn unary(mut tok: &mut Tokens) -> Option<Box<Ast>> {
    if consume(TokenKind::PLUS, &mut tok) {
        return primary(&mut tok);
    }
    if consume(TokenKind::MINUS, &mut tok) {
        return new_node(AstKind::SUB, new_node(AstKind::NUM(0.0), None, None), primary(&mut tok));
    }
    return primary(&mut tok);
}

fn primary(mut tok: &mut Tokens) -> Option<Box<Ast>> {
    if consume(TokenKind::LPAREN, &mut tok) {
        let node = expr(&mut tok);
        tok.expect(TokenKind::RPAREN);
        return node;
    }

    match tok.token[tok.idx].kind {
        TokenKind::NUM(_) =>
            new_node(AstKind::NUM(tok.expect_number()), None, None),
        _ => None,
    }
}

fn consume(kind: TokenKind, tok: &mut Tokens) -> bool {
    if tok.token[tok.idx].kind == kind {
        tok.idx += 1;
        true
    } else {
        false
    }
}

impl Tokens {
    fn expect(&mut self, kind: TokenKind) {
        if self.token[self.idx].kind == kind {
            self.idx += 1;
        } else {
            eprintln!("{}^", " ".repeat(self.token[self.idx].cur));
            panic!("expected {:?}, but got {:?}", kind, self.token[self.idx].kind);
        }
    }

    fn expect_number(&mut self) -> f64 {
        match self.token[self.idx].kind {
            TokenKind::NUM(num) => {
                self.idx += 1;
                num as f64
            },
            _ => {
                eprintln!("{}^", " ".repeat(self.token[self.idx].cur));
                panic!("expected NUM(f64). but got {:?}", self.token[self.idx].kind);
            },
        }
    }

    fn check_illegal(&self) {
        match self.token[self.idx].kind {
            TokenKind::ILLEGAL(_) => {
                eprintln!("{}^", " ".repeat(self.token[self.idx].cur));
                panic!("illegal TokenKind {:?}", self.token[self.idx].kind);
            },
            _ => (),
        }
    }
}
