use std::{thread, time};
use std::sync::{Arc, Mutex};

use rand::seq::SliceRandom;
use getch::Getch;

const FIELD_WIDTH:  usize = 10 + 2 + 2;  // field + wall + sentinel
const FIELD_HEIGHT: usize = 20 + 1 + 2;  // field + wall + sentinel

const MINO_TYPE_LENGTH: usize = 7;  // I O S Z J L T

/*   8421
 * 0 0000
 * 1 0000 */
static MINOS: [[i32; 2]; 7] = [
    [ 0, 15],  // I
    [ 6,  6],  // O
    [ 6, 12],  // S
    [12,  6],  // Z
    [ 8, 14],  // J
    [ 2, 14],  // L
    [ 4, 14],  // T
];

#[derive(Clone, Copy, PartialEq)]
enum MinoType {
    NONE = 0,
    WALL,
    I,
    O,
    S,
    Z,
    J,
    L,
    T,
}

impl From<usize> for MinoType {
    fn from(i: usize) -> Self {
        match i {
            0 => MinoType::NONE,
            1 => MinoType::WALL,
            2 => MinoType::I,
            3 => MinoType::O,
            4 => MinoType::S,
            5 => MinoType::Z,
            6 => MinoType::J,
            7 => MinoType::L,
            8 => MinoType::T,
            _ => unreachable!(),
        }
    }
}

struct Game {
    field: [[MinoType; FIELD_WIDTH]; FIELD_HEIGHT],
    mino_stack_main: [MinoType; MINO_TYPE_LENGTH],
    mino_stack_sub:  [MinoType; MINO_TYPE_LENGTH],
    mino_stack_index: usize,
    shape: [i32; 4],
    mino_type: MinoType,
    hold: Option<MinoType>,
    is_hold: bool,
    x: i32,
    y: i32,
}

impl Game {
    fn new() -> Self {
        let mut game = Game {
            field: [[MinoType::NONE; FIELD_WIDTH]; FIELD_HEIGHT],
            mino_stack_main: [MinoType::NONE; MINO_TYPE_LENGTH],
            mino_stack_sub:  [MinoType::NONE; MINO_TYPE_LENGTH],
            mino_stack_index: 0,
            shape: [0; 4],
            mino_type: MinoType::WALL,
            hold: None,
            is_hold: false,
            x: 0,
            y: 0,
        };

        // Init mino stakcs
        init_miso_stack(&mut game.mino_stack_main);
        init_miso_stack(&mut game.mino_stack_sub);

        game.next_mino();

        // Create wall
        for y in 0..FIELD_HEIGHT {
            game.field[y][1]             = MinoType::WALL;
            game.field[y][FIELD_WIDTH-2] = MinoType::WALL;
        }
        for x in 0..FIELD_WIDTH {
            game.field[FIELD_HEIGHT-2][x] = MinoType::WALL;
        }

        game
    }

    fn next_mino(&mut self) {
        if MINO_TYPE_LENGTH <= self.mino_stack_index {
            self.mino_stack_index = 0;
            self.mino_stack_main = self.mino_stack_sub;
            init_miso_stack(&mut self.mino_stack_sub);
        }

        self.mino_type = self.mino_stack_main[self.mino_stack_index];
        self.mino_stack_index += 1;

        set_mino_shape(&mut self.shape, self.mino_type);

        self.x = 5;
        self.y = 0;

        self.is_hold = false;
    }

    fn draw(&self) {
        static COLORS: &[&str] = &[
            "\x1b[48;2;000;000;000m  ",  // NONE
            "\x1b[48;2;127;127;127m__",  // WALL
            "\x1b[48;2;000;255;255m__",  // I
            "\x1b[48;2;255;255;000m__",  // O
            "\x1b[48;2;000;255;000m__",  // S
            "\x1b[48;2;255;000;000m__",  // Z
            "\x1b[48;2;000;000;255m__",  // J
            "\x1b[48;2;255;127;000m__",  // L
            "\x1b[48;2;127;000;255m__",  // T
        ];
        let mut field_buf = self.field;
        let mut output = String::new();

        for y in 0..4 {
            for x in 0..4 {
                if self.shape[y] >> (3-x) & 1 == 1 {
                    field_buf[y + self.y as usize][x + self.x as usize] = self.mino_type;
                }
            }
        }

        output.push_str("\x1b[H\n\n\n");
        for line in field_buf.iter().take(FIELD_HEIGHT-1).skip(1) {
            output.push_str("\x1b[0m  ");
            for col in line.iter().take(FIELD_WIDTH-1).skip(1) {
                output.push_str(COLORS[*col as usize]);
            }
            output.push('\n');
        }
        output.push_str("\x1b[0m");

        // Display hold
        if let Some(hold) = self.hold {
            output.push_str(&format!("\x1b[{};{}HHOLD", 1, FIELD_WIDTH * 2));
            let mino = MINOS[hold as usize - MinoType::I as usize];
            for y in 0..2 {
                output.push_str(&format!("\x1b[{};{}H", 3 + y, FIELD_WIDTH * 2));
                for x in (0..4).rev() {
                    if mino[y] >> x & 1 == 1 {
                        output.push_str(&format!("{}\x1b[0m", COLORS[hold as usize]));
                    } else {
                        output.push_str("  ");
                    }
                }
            }
        }

        // Display nexts
        output.push_str(&format!("\x1b[{};{}HNEXT", 6, FIELD_WIDTH * 2));
        let mut next_mino_count = 0;
        for i in self.mino_stack_index..MINO_TYPE_LENGTH {
            let mino = MINOS[self.mino_stack_main[i] as usize - MinoType::I as usize];
            for y in 0..2 {
                output.push_str(&format!("\x1b[{};{}H", 8 + y + next_mino_count * 3, FIELD_WIDTH * 2));
                for x in (0..4).rev() {
                    if mino[y] >> x & 1 == 1 {
                        output.push_str(&format!("{}\x1b[0m", COLORS[self.mino_stack_main[i] as usize]));
                    } else {
                        output.push_str("  ");
                    }
                }
            }
            next_mino_count += 1;
        }
        for i in 0..self.mino_stack_index-1 {
            let mino = MINOS[self.mino_stack_sub[i] as usize - MinoType::I as usize];
            for y in 0..2 {
                output.push_str(&format!("\x1b[{};{}H", 8 + y + next_mino_count * 3, FIELD_WIDTH * 2));
                for x in (0..4).rev() {
                    if mino[y] >> x & 1 == 1 {
                        output.push_str(&format!("{}\x1b[0m", COLORS[self.mino_stack_sub[i] as usize]));
                    } else {
                        output.push_str("  ");
                    }
                }
            }
            next_mino_count += 1;
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
                if self.field[(y+gy) as usize][(x+gx) as usize] != MinoType::NONE && shape[y as usize] >> (3-x) & 1 == 1 {
                    return true;
                }
            }
        }
        false
    }

    fn fix2field(&mut self) {
        let gy = self.y as usize;
        let gx = self.x as usize;
        for y in 0..4 {
            for x in 0..4 {
                if self.shape[y] >> (3-x) & 1 == 1 {
                    self.field[y+gy][x+gx] = self.mino_type;
                }
            }
        }
    }

    fn erase_line(&mut self) {
        for y in 1..FIELD_HEIGHT-2 {
            let mut can_erase = true;
            for x in 2..=FIELD_WIDTH-3 {
                if self.field[y][x] == MinoType::NONE {
                    can_erase = false;
                    break;
                }
            }
            if can_erase {
                for y2 in (1..y+1).rev() {
                    self.field[y2] = self.field[y2-1];
                }
            }
        }
    }

    fn check_gameover(&self) {
        if self.is_collision(0, 0, &self.shape) {
            self.draw();
            println!("\x1b[0m         GAME OVER\x1b[?25h");
            std::process::exit(0);
        }
    }

    fn hold(&mut self) {
        if self.is_hold {
            // Do nothing
            return;
        }

        if let Some(mut hold) = self.hold {
            std::mem::swap(&mut hold, &mut self.mino_type);
            set_mino_shape(&mut self.shape, self.mino_type);
            self.hold = Some(hold);
            self.x = 5;
            self.y = 0;
        } else {
            self.hold = Some(self.mino_type);
            self.next_mino();
        }
        self.is_hold = true;
    }
}

fn set_mino_shape(shape: &mut [i32; 4], mino_type: MinoType) {
    shape[0] = 0;
    shape[1] = MINOS[mino_type as usize - MinoType::I as usize][0];
    shape[2] = MINOS[mino_type as usize - MinoType::I as usize][1];
    shape[3] = 0;
}

fn init_miso_stack(stack: &mut [MinoType; MINO_TYPE_LENGTH]) {
    let mut new_stack = [
        MinoType::I,
        MinoType::O,
        MinoType::S,
        MinoType::Z,
        MinoType::J,
        MinoType::L,
        MinoType::T,
    ];
    new_stack.shuffle(&mut rand::thread_rng());
    *stack = new_stack;
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
                    game.fix2field();

                    // Erase line
                    game.erase_line();

                    // Spawn next mino
                    game.next_mino();

                    // Check gameover
                    game.check_gameover();
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
            // Left rotation
            Ok(b'z') => {
                let mut game = game.lock().unwrap();
                let mut s: [i32; 4] = [0; 4];
                for y in 0..4 {
                    for x in 0..4 {
                        s[x] |= (game.shape[4-1-y]>>x & 1) << y;
                    }
                }
                if !game.is_collision(0, 0, &s) {
                    game.shape = s;
                }
            }
            // Right rotation
            Ok(b'x') => {
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
            // Hold
            Ok(b' ') => {
                game.lock().unwrap().hold();
            }
            _  => (),
        }
        game.lock().unwrap().draw();
    }
}

fn main() {
    // Clear screen, hide cursor
    println!("\x1b[2J\x1b[H\x1b[?25l");

    gameloop(&mut Arc::new(Mutex::new(Game::new())));

    // Reset escape, display cursor
    println!("\x1b[0m\x1b[?25h");
}

