extern crate ncurses;
extern crate rand;

use ncurses::*;
use rand::{
    distributions::{Distribution, Standard},
    Rng,
};

use std::{thread, time};
use self::Direction::*;
use std::slice::Iter;


const AUTO: bool = true;     // Auto play
const UPS: u64 = 60;         // Update per seconds

const W: usize = 16 + 2;     // Field width  + Wall
const H: usize = 16 + 2;     // Field height + Wall
const SNAKE_MIN: usize = 3;  // Minimum snake length

/// (x, y)
#[derive(Clone, Copy, Debug, PartialEq)]
enum Direction {
    Up,
    Right,
    Down,
    Left,
}

impl Direction {
    fn iter() -> Iter<'static, Direction> {
        static DIRECTIONS: [Direction; 4] = [Up, Right, Down, Left];
        DIRECTIONS.into_iter()
    }
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
    Head,
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

    let mut snake_iter = snake.iter();
    if let Some(s) = snake_iter.next() {
        field_buf[s.y][s.x] = Kind::Head;
    }
    for s in snake_iter {
        field_buf[s.y][s.x] = Kind::Snake;
    }

    clear();
    for y in 0..H {
        for x in 0..W {
            match field_buf[y][x] {
                Kind::None  => printw("  "),
                Kind::Snake => printw("()"),
                Kind::Head  => printw("^^"),
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

fn dead_end_snake(field: &[[Kind; H]; W], snake: &Vec<Snake>, pos: &(usize, usize)) -> bool {
    //let mut field_buf = field.clone();
    //let mut snake_buf = snake.clone();

    //let mut i = snake_buf.len() - 1;
    //while i > 0 {
    //    snake_buf[i] = snake_buf[i-1];
    //    i -= 1;
    //}
    //snake_buf[0].x = pos.0;
    //snake_buf[0].y = pos.1;

    //for s in snake_buf.iter() {
    //    field_buf[s.y][s.x] = Kind::Snake;
    //}

    false
}

fn get_feed_pos(field: &[[Kind; H]; W]) -> Option<(usize, usize)> {
    for y in 1..H-1 {
        for x in 1..W-1 {
            if field[y][x] == Kind::Feed {
                return Some((x, y));
            }
        }
    }
    None
}

fn get_to_feed_distance(field: &[[Kind; H]; W], s_pos: &(usize, usize))
-> i32 {
    if let Some(f_pos) = get_feed_pos(&field) {
        (s_pos.0 as i32 - f_pos.0 as i32).abs()
            + (s_pos.1 as i32 - f_pos.1 as i32).abs()
    } else {
        eprintln!("error: get_feed_pos(): can't get pos");
        0
    }
}

fn eval(field: &[[Kind; H]; W], snake: &Vec<Snake>, dir: &Direction)
-> Direction {
    let mut ret = Direction::Right;
    let mut max_feed_distance = H as i32 * W as i32;

    for d in Direction::iter() {
        match d {
            Direction::Up    if *dir == Direction::Down  => continue,
            Direction::Right if *dir == Direction::Left  => continue,
            Direction::Down  if *dir == Direction::Up    => continue,
            Direction::Left  if *dir == Direction::Right => continue,
            _ => (),
        }

        let s_pos = match d {
            Direction::Up    => (snake[0].x,     snake[0].y - 1),
            Direction::Right => (snake[0].x + 1, snake[0].y    ),
            Direction::Down  => (snake[0].x,     snake[0].y + 1),
            Direction::Left  => (snake[0].x - 1, snake[0].y    ),
        };

        let feed_distance = get_to_feed_distance(&field, &s_pos);
        let is_collision  = collision_snake(&snake, s_pos.0, s_pos.1);
        let is_dead_end   = dead_end_snake(&field, &snake, &s_pos);

        if is_collision || is_dead_end {
            continue;
        }

        if feed_distance < max_feed_distance {
            max_feed_distance = feed_distance;
            ret = *d;
        }
    }
    ret
}

fn main() {
    initscr();
    noecho();
    curs_set(CURSOR_VISIBILITY::CURSOR_INVISIBLE);  // Not action...

    let mut gameover = false;
    let mut field = [[Kind::None; H]; W];
    let mut snake = Vec::with_capacity((H-2)*(W-2));
    let mut dir: Direction = rand::random();

    let wait_msec: u64 = 1000 / UPS;

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

        let mut pos_x = snake[0].x;
        let mut pos_y = snake[0].y;

        if AUTO {
            dir = eval(&field, &snake, &dir);
            match dir {
                Direction::Right => pos_x += 1,
                Direction::Up    => pos_y -= 1,
                Direction::Down  => pos_y += 1,
                Direction::Left  => pos_x -= 1,
            }
        } else {
            match getch() as u8 {
                b'q' => break,
                b'h' if dir != Direction::Right
                    => { pos_x -= 1; dir = Direction::Left  },
                b'j' if dir != Direction::Up
                    => { pos_y += 1; dir = Direction::Down  },
                b'k' if dir != Direction::Down
                    => { pos_y -= 1; dir = Direction::Up    },
                b'l' if dir != Direction::Left
                    => { pos_x += 1; dir = Direction::Right },
                _ => continue,
            }
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

        if AUTO {
            thread::sleep(time::Duration::from_millis(wait_msec));
        }
    }

    if gameover {
        printw("GAME OVER\n");
        printw("Press enter key...");
        while getch() as u8 != b'\n' {}
    }

    endwin();
}
