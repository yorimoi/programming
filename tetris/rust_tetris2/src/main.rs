use std::{thread, time};
use std::sync::{Arc, Mutex};

use rand::Rng;
use getch::Getch;

const FIELD_WIDTH:  usize = 10 + 2 + 2;  // field + wall + sentinel
const FIELD_HEIGHT: usize = 20 + 1 + 2;  // field + wall + sentinel

const MINO_TYPE_LENGTH: usize = 7;  // I O S Z J L T

struct Game {
    field: [[i32; FIELD_WIDTH]; FIELD_HEIGHT],
    shape: [i32; 4],
    x: i32,
    y: i32,
}

impl Game {
    fn new() -> Self {
        let mut game = Game {
            field: [[0; FIELD_WIDTH]; FIELD_HEIGHT],
            shape: [0; 4],
            x: 0,
            y: 0,
        };

        game.init_mino();

        for y in 0..FIELD_HEIGHT {
            game.field[y][1]             = 1;
            game.field[y][FIELD_WIDTH-2] = 1;
        }
        for x in 0..FIELD_WIDTH {
            game.field[FIELD_HEIGHT-2][x] = 1;
        }

        game
    }

    fn init_mino(&mut self) {
        /*   8421
         * 0 0000
         * 1 0000 */
        static MINOS: [[i32; 2]; 7] = [
            [15,  0],  // I
            [ 6,  6],  // O
            [ 6, 12],  // S
            [12,  6],  // Z
            [14,  2],  // J
            [ 2, 14],  // L
            [ 4, 14],  // T
        ];

        let rnd = rand::thread_rng().gen_range(0, MINO_TYPE_LENGTH);

        self.shape[0] = 0;
        self.shape[1] = MINOS[rnd][0];
        self.shape[2] = MINOS[rnd][1];
        self.shape[3] = 0;

        self.x = 5;
        self.y = 0;
    }

    fn draw(&self) {
        let mut field_buf = self.field;
        let mut output = String::with_capacity(FIELD_WIDTH * FIELD_HEIGHT * 2);

        for y in 0..4 {
            for x in 0..4 {
                field_buf[y + self.y as usize][x + self.x as usize] |= self.shape[y] >> (3-x) & 1;
            }
        }

        output.push_str("\x1b[H");
        for line in field_buf.iter().take(FIELD_HEIGHT-1).skip(1) {
            for col in line.iter().take(FIELD_WIDTH-1).skip(1) {
                if *col == 0 {
                    output.push_str(" .");
                } else {
                    output.push_str("[]");
                }
            }
            output.push('\n');
        }
        println!("{}", output);
    }

    fn is_collision(&self, dx: i32, dy: i32, shape: &[i32]) -> bool {
        let gx = self.x + dx;
        let gy = self.y + dy;
        for y in 0..4 {
            for x in 0..4 {
                if x+gx < 0 || FIELD_WIDTH as i32 <= x+gx || y+gy < 0 || FIELD_HEIGHT as i32 <= y+gy {
                    continue;
                }
                if self.field[(y+gy) as usize][(x+gx) as usize] & (shape[y as usize] >> (3-x) & 1) == 1 {
                    return true;
                }
            }
        }
        false
    }
}

fn gameloop(game: &mut Arc<Mutex<Game>>) {
    // Fall
    {
        let game = Arc::clone(&game);
        let _ = thread::spawn(move || {
            loop {
                game.lock().unwrap().draw();
                thread::sleep(time::Duration::from_millis(500));

                let mut game = game.lock().unwrap();
                if !game.is_collision(0, 1, &game.shape) {
                    game.y += 1;
                } else {
                    // Fix to field
                    let gy = game.y as usize;
                    let gx = game.x as usize;
                    for y in 0..4 {
                        for x in 0..4 {
                            game.field[y+gy][x+gx] |= game.shape[y] >> (3-x) & 1;
                        }
                    }

                    // Erase line
                    for y in 1..FIELD_HEIGHT-2 {
                        let mut can_erase = true;
                        for x in 2..=FIELD_WIDTH-3 {
                            if game.field[y][x] == 0 {
                                can_erase = false;
                                break;
                            }
                        }
                        if can_erase {
                            for y2 in (1..y+1).rev() {
                                game.field[y2] = game.field[y2-1];
                            }
                        }
                    }

                    // Spawn mino
                    game.init_mino();

                    // Check gameover
                    if game.is_collision(0, 0, &game.shape) {
                        game.draw();
                        println!("        GAME OVER\x1b[?25h");
                        std::process::exit(0);
                    }
                }
            }
        });
    }

    // Key operation
    let g = Getch::new();
    loop {
        match g.getch() {
            // Quit
            Ok(b'q') => return,
            // Left
            Ok(b'h') => {
                let mut game = game.lock().unwrap();
                if !game.is_collision(-1, 0, &game.shape) {
                    game.x -= 1;
                }
            }
            // Down
            Ok(b'j') => {
                let mut game = game.lock().unwrap();
                if !game.is_collision(0, 1, &game.shape) {
                    game.y += 1;
                }
            }
            // Right
            Ok(b'l') => {
                let mut game = game.lock().unwrap();
                if !game.is_collision(1, 0, &game.shape) {
                    game.x += 1;
                }
            }
            // Hard drop
            Ok(b'k') => {
                let mut game = game.lock().unwrap();
                while !game.is_collision(0, 1, &game.shape) {
                    game.y += 1;
                }
            }
            // Right rotation
            Ok(b' ') => {
                let mut game = game.lock().unwrap();
                let mut s: [i32; 4] = [0; 4];
                for y in 0..4 {
                    for x in 0..4 {
                        s[4-1-x] |= (game.shape[y]>>x & 1) << y;
                    }
                }
                if !game.is_collision(0, 0, &s) {
                    game.shape = s;
                }
            }
            _  => (),
        }
        game.lock().unwrap().draw();
    }
}

fn main() {
    let game = Game::new();

    println!("\x1b[2J\x1b[H\x1b[?25l");

    gameloop(&mut Arc::new(Mutex::new(game)));

    println!("\x1b[?25h");
}

