extern crate ncurses;
extern crate rand;

use ncurses::*;
use rand::Rng;

#[derive(Clone, Copy, Debug, PartialEq)]
enum Tree {
    NONE,
    AXIS,
    BRANCH,
    PLAYER,
}

#[derive(PartialEq)]
enum Dir {
    LEFT,
    RIGHT,
}

fn draw(tree: &[[Tree; 3]], count: u32) {
    clear();
    printw(format!("{}\n\n", count).as_ref());
    for t in tree {
        for u in t {
            let ch = match u {
                Tree::NONE => ' ',
                Tree::AXIS => '|',
                Tree::BRANCH => '-',
                Tree::PLAYER => '@',
            };
            printw(format!("{}", ch).as_ref());
        }
        printw("\n");
    }
}

fn gen_tree(t: &mut [Tree; 3]) {
    let mut rng = rand::thread_rng();

    match rng.gen::<u8>() % 3 {
        0 => {
            t[0] = Tree::BRANCH;
            t[1] = Tree::AXIS;
            t[2] = Tree::NONE;
        },
        1 => {
            t[0] = Tree::NONE;
            t[1] = Tree::AXIS;
            t[2] = Tree::BRANCH;
        },
        _ => {
            t[0] = Tree::NONE;
            t[1] = Tree::AXIS;
            t[2] = Tree::NONE;
        },
    }
}

fn shift(tree: &mut [[Tree; 3]], pos: &Dir) {
    let mut i: usize = tree.len() - 1;

    while i > 0 {
        tree[i] = tree[i-1];
        i -= 1;
    }
    gen_tree(&mut tree[0]);

    if *pos == Dir::LEFT {
        tree[tree.len()-1][0] = Tree::PLAYER;
    } else {
        tree[tree.len()-1][2] = Tree::PLAYER;
    }
}

fn is_collision(tree: &[[Tree; 3]], pos: &Dir) -> bool {
    match pos {
        Dir::LEFT  => tree[tree.len()-2][0] == Tree::BRANCH,
        Dir::RIGHT => tree[tree.len()-2][2] == Tree::BRANCH,
    }
}

fn main() {
    const KEY_H: i32 = 104;
    const KEY_L: i32 = 108;
    const KEY_Q: i32 = 113;

    const LENGTH: usize = 7;
    let mut tree = [[Tree::NONE; 3]; LENGTH];
    let mut player_pos = Dir::LEFT;
    let mut count: u32 = 0;

    // Initialize
    for t in &mut tree {
        gen_tree(t);
    }
    if tree[LENGTH-1][0] == Tree::NONE {
        tree[LENGTH-1][0] = Tree::PLAYER;
    } else if tree[LENGTH-1][2] == Tree::NONE {
        tree[LENGTH-1][2] = Tree::PLAYER;
    } else {
        panic!("failed initialize\n");
    }

    initscr();
    noecho();
    curs_set(CURSOR_VISIBILITY::CURSOR_INVISIBLE);

    draw(&tree, count);
    loop {
        match getch() {
            KEY_Q => break,
            KEY_H => player_pos = Dir::LEFT,
            KEY_L => player_pos = Dir::RIGHT,
            _ => continue,
        }
        if is_collision(&tree, &player_pos) {
            printw("GAME OVER\n");
            getch();
            break;
        }
        shift(&mut tree, &player_pos);
        draw(&tree, count);
        count += 1;
    }

    endwin();

    println!("SCORE: {}\n", count-1);
}
