use crate::token::TokenKind;
use crate::table;

pub fn view(table: &table::Table) {

    // header
    let mut len = 0;
    for name in &table.col_names {
        len += name.len() + 3;
        print!("| {} ", name);
    }
    println!("|");
    println!("={:=<width$}", "=", width = len);

    // body
    for row in &table.rows {
        for name in &table.col_names {
            let col = row.get(name);
            match col {
                Some(TokenKind::Number(n)) => print!("| {} ", n),
                Some(TokenKind::String(s)) => print!("| {} ", s),
                _ => print!("| NONE "),
            }
        }
        println!("|");
    }
}

pub fn html(table: &table::Table) -> String {

    let mut ret = String::new();

    // header
    let mut len = 0;
    for name in &table.col_names {
        len += name.len() + 3;
        ret += &format!("| {} ", name);
    }
    ret += &format!("|<br />={:=<width$}<br />", "=", width = len);

    // body
    for row in &table.rows {
        for name in &table.col_names {
            let col = row.get(name);
            match col {
                Some(TokenKind::Number(n)) => ret += &format!("| {} ", n),
                Some(TokenKind::String(s)) => ret += &format!("| {} ", s),
                _ => print!("| NONE "),
            }
        }
        ret += "|<br />";
    }

    ret
}
