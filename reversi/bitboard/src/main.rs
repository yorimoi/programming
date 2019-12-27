extern crate rand;

use std::mem;
use std::ops::Not;
use rand::Rng;

type BitBoard = u64;

#[derive(Clone, Copy, PartialEq)]
enum Turn {
    Black,
    White,
}

struct Board {
    player_board: BitBoard,
    opponent_board: BitBoard,
    now_turn: Turn,
}

impl Not for Turn {
    type Output = Turn;
    fn not(self) -> Turn {
        if self == Turn::Black {
            Turn::White
        } else {
            Turn::Black
        }
    }
}

impl Board {
    fn new() -> Self {
        Board {
            player_board:   0x0000_0008_1000_0000,
            opponent_board: 0x0000_0010_0800_0000,
            now_turn: Turn::Black,
        }
    }

    fn can_put(&self, put: BitBoard) -> bool {
        let legal_board = self.make_legal_board();
        (put & legal_board) == put
    }

    fn make_legal_board(&self) -> BitBoard {
        let horizontal_watch_board: BitBoard = self.opponent_board & 0x7e7e_7e7e_7e7e_7e7e;
        let vertical_watch_board: BitBoard   = self.opponent_board & 0x00FF_FFFF_FFFF_FF00;
        let all_side_watch_board: BitBoard   = self.opponent_board & 0x007e_7e7e_7e7e_7e00;
        let blank_board: BitBoard = !(self.player_board | self.opponent_board);
        let mut tmp: BitBoard;
        let mut legal_board: BitBoard;

        // Left
        tmp = horizontal_watch_board & (self.player_board << 1);
        for _ in 0..5 {
            tmp |= horizontal_watch_board & (tmp << 1);
        }
        legal_board = blank_board & (tmp << 1);

        // Right
        tmp = horizontal_watch_board & (self.player_board >> 1);
        for _ in 0..5 {
            tmp |= horizontal_watch_board & (tmp >> 1);
        }
        legal_board |= blank_board & (tmp >> 1);

        // Up
        tmp = vertical_watch_board & (self.player_board << 8);
        for _ in 0..5 {
            tmp |= vertical_watch_board & (tmp << 8);
        }
        legal_board |= blank_board & (tmp << 8);

        // Down
        tmp = vertical_watch_board & (self.player_board >> 8);
        for _ in 0..5 {
            tmp |= vertical_watch_board & (tmp >> 8);
        }
        legal_board |= blank_board & (tmp >> 8);

        // Up right
        tmp = all_side_watch_board & (self.player_board << 7);
        for _ in 0..5 {
            tmp |= all_side_watch_board & (tmp << 7);
        }
        legal_board |= blank_board & (tmp << 7);

        // Up left
        tmp = all_side_watch_board & (self.player_board << 9);
        for _ in 0..5 {
            tmp |= all_side_watch_board & (tmp << 9);
        }
        legal_board |= blank_board & (tmp << 9);

        // Down right
        tmp = all_side_watch_board & (self.player_board >> 9);
        for _ in 0..5 {
            tmp |= all_side_watch_board & (tmp >> 9);
        }
        legal_board |= blank_board & (tmp >> 9);

        // Down left
        tmp = all_side_watch_board & (self.player_board >> 7);
        for _ in 0..5 {
            tmp |= all_side_watch_board & (tmp >> 7);
        }
        legal_board |= blank_board & (tmp >> 7);

        legal_board
    }

    fn reverse(&mut self, put: BitBoard) {
        let mut rev: BitBoard = 0;
        for k in 0..8 {
            let mut rev_: BitBoard = 0;
            let mut mask: BitBoard = transfer(put, k);
            while mask != 0 && mask & self.opponent_board != 0 {
                rev_ |= mask;
                mask = transfer(mask, k);
            }
            if mask & self.player_board != 0 {
                rev |= rev_;
            }
        }
        self.player_board   ^= put | rev;
        self.opponent_board ^= rev;
    }

    fn swap_board(&mut self) {
        mem::swap(&mut self.player_board, &mut self.opponent_board);
        self.now_turn = !self.now_turn;
    }

    fn is_pass(&self) -> bool {
        let player_legal_board   = self.make_legal_board();
        let mut board_tmp        = Board::new();
        board_tmp.now_turn       = self.now_turn;
        board_tmp.player_board   = self.opponent_board;
        board_tmp.opponent_board = self.player_board;
        let opponent_legal_board = board_tmp.make_legal_board();

        player_legal_board == 0 && opponent_legal_board != 0
    }

    fn is_gameover(&self) -> bool {
        let player_legal_board   = self.make_legal_board();
        let mut board_tmp        = Board::new();
        board_tmp.now_turn       = self.now_turn;
        board_tmp.player_board   = self.player_board;
        board_tmp.opponent_board = self.opponent_board;
        let opponent_legal_board = board_tmp.make_legal_board();

        player_legal_board == 0 && opponent_legal_board == 0
    }

    fn get_random_coordinate(&self) -> BitBoard {
        let shift = rand::thread_rng().gen_range(0, 64);
        1 << shift
    }

    fn print_result(&self) {
        let mut score_black = bit_count(self.player_board);
        let mut score_white = bit_count(self.opponent_board);
        if self.now_turn == Turn::White {
            mem::swap(&mut score_black, &mut score_white);
        }

        let winner = {
            if score_black < score_white {
                "WHITE WIN!"
            } else if score_black > score_white {
                "BLACK WIN!"
            } else {
                "DRAW"
            }
        };

        println!("BLACK[x]: {} vs {} :WHITE[o]", score_black, score_white);
        println!("{}", winner);
    }

    fn display(&self) {
        print!("\x1b[H");
        let (p, o) = if self.now_turn == Turn::Black {
            ('x', 'o')
        } else {
            ('o', 'x')
        };

        for i in (0..64).rev() {
            if self.player_board >> i & 1 == 1 {
                print!("{}", p);
            } else if self.opponent_board >> i & 1 == 1 {
                print!("{}", o);
            } else {
                print!(".");
            }
            if i % 8 == 0 {
                print!("\n");
            }
        }
        println!();
    }
}

fn coordinate2bit(x: char, y: u64) -> BitBoard {
    let mut mask: BitBoard = 0x8000_0000_0000_0000;
    match x {
        'A'..='H' => mask >>= x as u64 - 0x41,
        _ => panic!("out of range 'A'-'H': '{}'", x),
    }
    mask >> ((y - 1) * 8)
}

fn transfer(put: BitBoard, k: BitBoard) -> BitBoard {
    match k {
        0 => put << 8 & 0xffff_ffff_ffff_ff00, // Up
        1 => put << 7 & 0x7f7f_7f7f_7f7f_7f00, // Up right
        2 => put >> 1 & 0x7f7f_7f7f_7f7f_7f7f, // Right
        3 => put >> 9 & 0x007f_7f7f_7f7f_7f7f, // Down right
        4 => put >> 8 & 0x00ff_ffff_ffff_ffff, // Down
        5 => put >> 7 & 0x00fe_fefe_fefe_fefe, // Down left
        6 => put << 1 & 0xfefe_fefe_fefe_fefe, // Left
        7 => put << 9 & 0xfefe_fefe_fefe_fe00, // Up left
        _ => 0,
    }
}

fn bit_count(num: BitBoard) -> usize {
    let mut mask: BitBoard = 0x8000_0000_0000_0000;
    let mut count = 0;

    for _ in 0..64 {
        if mask & num != 0 {
            count += 1;
        }
        mask >>= 1;
    }

    count
}

fn main() {
    println!("\x1b[2J\x1b[H");
    let mut board = Board::new();

    loop {
        board.display();
        let put = board.get_random_coordinate();

        if board.can_put(put) {
            board.reverse(put);
            board.swap_board();
        } else {
            if board.is_pass() {
                board.swap_board();
            } else {
                continue;
            }
        }
        if board.is_pass() {
            board.swap_board();
        }
        if board.is_gameover() {
            board.display();
            board.print_result();
            break;
        }

        std::thread::sleep(std::time::Duration::from_millis(100));
    }
}

