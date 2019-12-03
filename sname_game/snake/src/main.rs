extern crate ncurses;
extern crate rand;

use ncurses::*;
use rand::{
    distributions::{Distribution, Standard},
    Rng,
};

const W: usize = 16 + 2;  // Field width  + Wall
const H: usize = 16 + 2;  // Field height + Wall
const SNAKE_MIN: usize = 3;

/// (x, y)
#[derive(Clone, Copy, Debug)]
enum Direction {
    Up,
    Right,
    Down,
    Left,
}

impl Distribution<Direction> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> Direction {
        match rng.gen_range(0, 4) {
            0 => Direction::Up,
            1 => Direction::Right,
            2 => Direction::Down,
            _ => Direction::Left,
        }
    }
}

#[derive(Clone, Copy, PartialEq)]
enum Kind {
    None,
    Snake,
    Wall,
    Feed,
}

#[derive(Clone, Copy, Debug)]
struct Snake {
    x: usize,
    y: usize,
}

fn draw(field: &[[Kind; H]; W], snake: &Vec<Snake>) {
    let mut field_buf = field.clone();

    for s in snake.iter() {
        field_buf[s.y][s.x] = Kind::Snake;
    }

    clear();
    for y in 0..H {
        for x in 0..W {
            match field_buf[y][x] {
                Kind::None  => printw("  "),
                Kind::Snake => printw("()"),
                Kind::Wall  => printw("[]"),
                Kind::Feed  => printw("<>"),
            };
        }
        printw("\n");
    }
    refresh();
}

fn spawn_feed(field: &mut [[Kind; H]; W], snake: &Vec<Snake>) {
    'outer: loop {
        let y = rand::thread_rng().gen_range(1, H-2);
        let x = rand::thread_rng().gen_range(1, W-2);

        for s in snake.iter() {
            if s.y == y && s.x == x {
                continue 'outer;
            }
        }

        field[y][x] = Kind::Feed;
        return;
    }
}

fn growth_snake(snake: &mut Vec<Snake>) {
    snake.push(Snake { x: 0, y: 0 });
}

fn move_snake(snake: &mut Vec<Snake>, pos_x: usize, pos_y: usize) {
    let mut i = snake.len() - 1;
    while i > 0 {
        snake[i] = snake[i-1];
        i -= 1;
    }
    snake[0].x = pos_x;
    snake[0].y = pos_y;
}

fn collision_snake(snake: &Vec<Snake>, pos_x: usize, pos_y: usize) -> bool {
    for s in snake {
        if s.x == pos_x && s.y == pos_y {
            return true;
        }
    }
    false
}

fn main() {
    initscr();
    noecho();
    curs_set(CURSOR_VISIBILITY::CURSOR_INVISIBLE);  // Not action...

    let mut gameover = false;
    let mut field = [[Kind::None; H]; W];
    let mut snake = Vec::with_capacity((H-2)*(W-2));
    let mut dir: Direction = rand::random();

    for _ in 0..SNAKE_MIN {
        snake.push(Snake { x: W/2, y: H/2 });
    }

    for y in 0..H {
        field[y][0]   = Kind::Wall;
        field[y][W-1] = Kind::Wall;
    }
    for x in 0..W {
        field[0][x]   = Kind::Wall;
        field[H-1][x] = Kind::Wall;
    }

    spawn_feed(&mut field, &snake);
    draw(&field, &snake);

    printw("Press space key");
    loop {
        match getch() as u8 {
            b' ' => break,
            _ => (),
        }
    }
    clear();
    draw(&field, &snake);

    'gameloop: loop {

        // TODO
        // auto move => thread

        let mut pos_x = snake[0].x;
        let mut pos_y = snake[0].y;
        match getch() as u8 {
            b'q' => break,
            b'h' => { pos_x -= 1; dir = Direction::Left  },
            b'j' => { pos_y += 1; dir = Direction::Down  },
            b'k' => { pos_y -= 1; dir = Direction::Up    },
            b'l' => { pos_x += 1; dir = Direction::Right },
            _ => (),
        }

        match field[pos_y][pos_x] {
            Kind::Wall => {
                gameover = true;
                break;
            },
            Kind::Feed => {
                growth_snake(&mut snake);
                move_snake(&mut snake, pos_x, pos_y);
                if snake.len() < (H-2)*(W-2) {
                    field[pos_y][pos_x] = Kind::None;
                    spawn_feed(&mut field, &snake);
                } else {
                    draw(&field, &snake);
                    printw("GAME CLEAR!!\n");
                    printw("Press enter key...");
                    while getch() as u8 != b'\n' {}
                    break;
                }
            },
            _ => {
                if collision_snake(&snake, pos_x, pos_y) {
                    gameover = true;
                    break;
                }
                move_snake(&mut snake, pos_x, pos_y);
            },
        }

        draw(&field, &snake);
    }

    if gameover {
        printw("GAME OVER\n");
        printw("Press enter key...");
        while getch() as u8 != b'\n' {}
    }

    endwin();
}
