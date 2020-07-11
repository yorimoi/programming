use getch::Getch;
use std::time::SystemTime;

const ROW: usize = 9;
const COL: usize = 9;

struct Board {
    cells: [[usize; COL]; ROW],
    pos:   (usize, usize),  // col, row
}

impl Board {
    fn new() -> Self {
        // Answer example
        // +========+========+========+
        // | 5, 3, 4| 6, 7, 8| 9, 1, 2|
        // |--------+--------+--------|
        // | 6, 7, 2| 1, 9, 5| 3, 4, 8|
        // |--------+--------+--------|
        // | 1, 9, 8| 3, 4, 2| 5, 6, 7|
        // +========+========+========+
        // | 8, 5, 9| 7, 6, 1| 4, 2, 3|
        // |--------+--------+--------|
        // | 4, 2, 6| 8, 5, 3| 7, 9, 1|
        // |--------+--------+--------|
        // | 7, 1, 3| 9, 2, 4| 8, 5, 6|
        // +========+========+========+
        // | 9, 6, 1| 5, 3, 7| 2, 8, 4|
        // |--------+--------+--------|
        // | 2, 8, 7| 4, 1, 9| 6, 3, 5|
        // |--------+--------+--------|
        // | 3, 4, 5| 2, 8, 6| 1, 7, 9|
        // +========+========+========+
        let cells = [
            [5,3,0,0,7,0,0,0,0],
            [6,0,0,1,9,5,0,0,0],
            [0,9,8,0,0,0,0,6,0],
            [8,0,0,0,6,0,0,0,3],
            [4,0,0,8,0,3,0,0,1],
            [7,0,0,0,2,0,0,0,6],
            [0,6,0,0,0,0,2,8,0],
            [0,0,0,4,1,9,0,0,5],
            [0,0,0,0,8,0,0,7,9],
        ];
        Board {
            cells,
            pos:   (0, 0),
        }
    }

    fn display(&self) {
        print!("\x1b[H");
        for row in 0..ROW {
            print!("{}", if row%3 == 0 {"+========+========+========+\n"} else {"|--------+--------+--------|\n"});
            for col in 0..COL {
                if col == self.pos.0 && row == self.pos.1 {
                    print!("{}\x1b[41m ", if col%3 == 0 {"|"} else {","});
                    if self.cells[row][col] == 0 {
                        print!(" ");
                    } else {
                        print!("{}", self.cells[row][col]);
                    }
                    print!("\x1b[0m");
                } else {
                    print!("{} ", if col%3 == 0 {"|"} else {","});
                    if self.cells[row][col] == 0 {
                        print!(" ");
                    } else {
                        print!("{}", self.cells[row][col]);
                    }
                };
            }
            print!("|\n");
        }
        println!("+========+========+========+");
    }

    fn is_clear(&self) -> bool {
        // Check row
        for row in 0..ROW {
            if self.cells[row].iter().sum::<usize>() != 45 {
                return false;
            }
        }
        // Check col
        for col in 0..COL {
            let mut sum = 0;
            for row in 0..ROW {
                sum += self.cells[row][col];
            }
            if sum != 45 {
                return false;
            }
        }
        // Check box
        for col in 0..3 {
            let mut boxes = [0; 3];
            for row in 0..3 {
                boxes[0] += self.cells[col*3+row][0] + self.cells[col*3+row][1] + self.cells[col*3+row][2];
                boxes[1] += self.cells[col*3+row][0] + self.cells[col*3+row][1] + self.cells[col*3+row][2];
                boxes[2] += self.cells[col*3+row][0] + self.cells[col*3+row][1] + self.cells[col*3+row][2];
            }
            if boxes[0] != 45 || boxes[1] != 45 || boxes[2] != 45 {
                return false;
            }
        }

        true
    }
}

fn main() {
    let mut board = Board::new();
    let g = Getch::new();

    print!("\x1b[H\x1b[2J\x1b[?25l");

    // Time start
    let start = SystemTime::now();

    loop {
        board.display();

        match g.getch() {
            Ok(b'q') => break,
            Ok(b'h') => if 0 < board.pos.0     { board.pos.0 -= 1 },
            Ok(b'j') => if board.pos.1 < ROW-1 { board.pos.1 += 1 },
            Ok(b'k') => if 0 < board.pos.1     { board.pos.1 -= 1 },
            Ok(b'l') => if board.pos.0 < COL-1 { board.pos.0 += 1 },
            Ok(b' ') | Ok(b'0') => board.cells[board.pos.1][board.pos.0] = 0,
            Ok(c) if b'1' <= c && c <= b'9' => {
                board.cells[board.pos.1][board.pos.0] = (c - b'0') as usize;
                if board.is_clear() {
                    let secs = start.elapsed().unwrap().as_secs();
                    board.display();
                    println!("         GAME CLEAR");
                    println!("{} hours {} mins {} secs", secs / 60 / 60, secs / 60, secs % 60);
                    break;
                }
            },
            _ => (), // Do nothing
        }
    }

    println!("\x1b[?25h");
}
