extern crate termios;

use std::io;
use std::io::Read;
use std::io::Write;
use termios::{Termios, TCSANOW, ECHO, ICANON, tcsetattr};

const FIELD_WIDTH:      usize = 16;
const FIELD_HEIGHT:     usize = 16;

fn getch() -> [u8; 1] {
  let stdin = 0;
  let termios = Termios::from_fd(stdin).unwrap();
  let mut new_termios = termios.clone();
  new_termios.c_lflag &= !(ICANON|ECHO);
  tcsetattr(stdin, TCSANOW, &mut new_termios).unwrap();
  let stdout = io::stdout();
  let mut reader = io::stdin();
  let mut buffer = [0;1];
  stdout.lock().flush().unwrap();
  reader.read_exact(&mut buffer).unwrap();
  tcsetattr(stdin, TCSANOW, & termios).unwrap();
  buffer
}

fn get_adjacent_lives_count
(_cells: [[u8; FIELD_WIDTH]; FIELD_HEIGHT] ,_x: usize, _y: usize) -> u8
{
  let mut count: u8 = 0;
  for y in 0..3 {
    for x in 0..3 {
      if 1==x && 1==y { continue; }
      let (x2, y2);
      if ((_x as i8 + x as i8 - 1) as i8) < 0 {
        x2 = FIELD_WIDTH  - 1;
      } else {
        x2 = (_x + x - 1) % FIELD_WIDTH;
      }
      if ((_y as i8 + y as i8 - 1) as i8) < 0 {
        y2 = FIELD_HEIGHT - 1;
      } else {
        y2 = (_y + y - 1) % FIELD_HEIGHT;
      }
      //let x2: usize = ((_x + x - 1)) as usize;
      //let y2: usize = ((_y + y - 1)) as usize;
      //if FIELD_WIDTH>=x2 || FIELD_HEIGHT>=y2 { continue; }
      count += _cells[y2 as usize][x2 as usize] as u8;
    }
  }
  count
}

fn main()
{
  let mut cursor_x = 0;
  let mut cursor_y = 0;
  let mut current  = 0;
  let mut cells    = [[[0;  FIELD_WIDTH]; FIELD_HEIGHT]; 2];

  print!("\x1B[H\x1B[2J\x1B[?25l");
  let out = io::stdout();
  let mut out = io::BufWriter::new(out.lock());

  loop {
    for y in 0..FIELD_HEIGHT {
      for x in 0..FIELD_WIDTH {
        if x == cursor_x && y == cursor_y {
          out.write(b" @").unwrap();
        }
        else {
          if 1 == cells[current][y][x] {
            out.write(b" \x1B[38;5;047m#\x1B[0m").unwrap();
          } else {
            out.write(b" .").unwrap();
          }
        }
      }
      out.write(b"\n").unwrap();
    }
    out.flush().unwrap();

    //println!("{}", getch()[0]);
    match getch()[0] {
       32 => { cells[current][cursor_y][cursor_x] ^= 1; }  // space
       99 => { for y in 0..FIELD_HEIGHT {                  // c
                 for x in 0..FIELD_WIDTH {
                   cells[current][y][x] = 0;
                 }
               }
             }
      104 => { if cursor_x>0 {cursor_x-=1;} }              // h
      106 => { if cursor_y<FIELD_HEIGHT-1 {cursor_y+=1;} } // j
      107 => { if cursor_y>0 {cursor_y-=1;} }              // k
      108 => { if cursor_x<FIELD_WIDTH-1 {cursor_x+=1;} }  // l
      113 => { break; }                                    // q
      110 => { for y in 0..FIELD_HEIGHT {                  // n
                 for x in 0..FIELD_WIDTH {
                   let n = get_adjacent_lives_count(cells[current], x, y);
                   let mut next: u8 = cells[current][y][x];
                   if cells[current][y][x] == 1 {
                     if n<=1 || n>=4 {
                       next = 0;
                     }
                   } else {
                     if n==3 {
                       next = 1;
                     }
                   }
                   cells[current^1][y][x] = next;
                 }
               }
               current ^= 1;
             }
       _  => { }
    }
    out.write(b"\x1B[H").unwrap();
  }

  print!("\x1B[?25h");
}
