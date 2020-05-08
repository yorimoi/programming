use crate::token;

pub type Expression = Box<token::Token>;

#[derive(Default)]
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

pub enum AstKind {
    Select,
    Create,
    Insert,
}

pub struct Statement {
    select: Option<SelectStatement>,
    create: Option<CreateTableStatement>,
    insert: Option<InsertStatement>,
    kind: AstKind,
}

pub struct InsertStatement {
    table: token::Token,
    values: Vec<Expression>,
}

pub struct ColumnDefinition {
    name: token::Token,
}

pub struct CreateTableStatement {
    name: token::Token,
    cols: Vec<ColumnDefinition>,
}

pub struct SelectStatement {
    item: Vec<Expression>,
    from: token::Token,
}
