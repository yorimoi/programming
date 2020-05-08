use crate::token;

pub type Expression = token::Token;

#[derive(Debug, Default)]
pub struct Ast {
    statements: Vec<Statement>,
}

impl Ast {
    pub fn new() -> Self {
        Self {
            statements: Vec::new(),
        }
    }

    pub fn push_statement(&mut self, stmt: Statement) {
        self.statements.push(stmt);
    }
}

#[derive(Debug, PartialEq)]
pub enum AstKind {
    Select,
    Create,
    Insert,
}

#[derive(Debug, PartialEq)]
pub struct Statement {
    pub select: Option<SelectStatement>,
    pub create: Option<CreateTableStatement>,
    pub insert: Option<InsertStatement>,
    pub kind: AstKind,
}

impl Statement {
    pub fn new_select(stmt: SelectStatement) -> Self {
        Self {
            select: Some(stmt),
            create: None,
            insert: None,
            kind: AstKind::Select,
        }
    }
}

#[derive(Debug, PartialEq)]
pub struct SelectStatement {
    pub item: Vec<Expression>,
    pub from: Expression,
}

impl SelectStatement {
    pub fn new(item: Vec<Expression>, from: Expression) -> Self {
        Self { item, from }
    }
}

#[derive(Debug, PartialEq)]
pub struct InsertStatement {
    table: token::Token,
    values: Vec<Expression>,
}

#[derive(Debug, PartialEq)]
pub struct ColumnDefinition {
    name: token::Token,
}

#[derive(Debug, PartialEq)]
pub struct CreateTableStatement {
    name: token::Token,
    cols: Vec<ColumnDefinition>,
}
