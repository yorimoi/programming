use super::ast::*;

pub fn gen(node: &Option<Box<Ast>>, mut vec: &mut Vec<i64>) {
    match node.as_ref().unwrap().kind {
        AstKind::NUM(num) => {
            vec.push(num);
            return;
        },
        _ => (),
    }

    gen(&node.as_ref().unwrap().lnode, &mut vec);
    gen(&node.as_ref().unwrap().rnode, &mut vec);

    let i = vec.pop().unwrap();
    let j = vec.pop().unwrap();

    match node.as_ref().unwrap().kind {
        AstKind::ADD => vec.push(j+i),
        AstKind::SUB => vec.push(j-i),
        AstKind::MUL => vec.push(j*i),
        AstKind::DIV => {
            if i == 0 {
                panic!("Divide by Zero");
            }
            vec.push(j/i);
        },
        _ => (),
    }
}
