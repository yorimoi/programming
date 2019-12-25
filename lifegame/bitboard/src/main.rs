use std::fs::File;
use std::io::{stdout, Write, BufWriter, Read, BufReader};

fn display(field: &[u32]) {
    let out = stdout();
    let mut out = BufWriter::new(out.lock());
    let _ = out.write(b"\x1b[H");
    for f in field.iter().skip(1).take(field.len()-2) {
        for j in 1..=field.len()-2 {
            if f&(1<<j) == 1<<j {
                let _ = out.write(b" *");
            } else {
                let _ = out.write(b" .");
            }
        }
        let _ = out.write(b"\n");
    }
    //println!();
}

fn eval_normal(field: &mut [u32; 32]) {
    let mut buffer: [u32; 32] = [0; 32];

    for i in 1..=field.len()-2 {
        for j in 1..=field.len()-2 {
            // 012
            // 345
            // 678
            let cnt = {
                ((field[i-1]>>j+1)&1) +  // 0
                ((field[i-1]>>j  )&1) +  // 1
                ((field[i-1]>>j-1)&1) +  // 2
                ((field[i  ]>>j+1)&1) +  // 3
                /*  focus bit  */        // 4
                ((field[i  ]>>j-1)&1) +  // 5
                ((field[i+1]>>j+1)&1) +  // 6
                ((field[i+1]>>j  )&1) +  // 7
                ((field[i+1]>>j-1)&1)    // 8
            };
            if cnt==3 || cnt==2 && field[i]>>j&1==1 {
                buffer[i] |= 1<<j;
            }
        }
    }

    *field = buffer;
}

fn main() {
    println!("\x1b[2J");
    let mut field: [u32; 32] = [0; 32];  // 32x32 (30x30 sentinel)

    { // Initialize field
        let mut i = 0;
        let f = File::open("board.txt").expect("file not found");
        for bit in BufReader::new(f).bytes() {
            let bit = bit.unwrap();
            if bit != 49 {
                if bit == 48 { i += 1; }
                continue;
            }
            field[i/32] |= 1<<i%32;
            i += 1;
        }
    }

    //loop {
    for _ in 0..1000 {
        display(&field);
        eval_normal(&mut field);
    }
}

