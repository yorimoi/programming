fn main() {
    println!("[AND]");
    for x1 in 0..2 {
        for x2 in 0..2 {
            println!("and({}, {}) => {}", x1, x2, and(x1, x2));
        }
    }

    println!("[OR]");
    for x1 in 0..2 {
        for x2 in 0..2 {
            println!("or({}, {}) => {}", x1, x2, or(x1, x2));
        }
    }

    println!("[NAND]");
    for x1 in 0..2 {
        for x2 in 0..2 {
            println!("nand({}, {}) => {}", x1, x2, nand(x1, x2));
        }
    }

    println!("[XOR]");
    for x1 in 0..2 {
        for x2 in 0..2 {
            println!("xor({}, {}) => {}", x1, x2, xor(x1, x2));
        }
    }
}

fn and(x1: i32, x2: i32) -> i32 {
    let w1 = 0.5;
    let w2 = 0.5;
    let theta = 0.7;

    if ((x1 as f32) * w1 + (x2 as f32) * w2) > theta {
        1
    }
    else {
        0
    }
}

fn or(x1: i32, x2: i32) -> i32 {
    let w1 = 0.5;
    let w2 = 0.5;
    let theta = 0.0;

    if ((x1 as f32) * w1 + (x2 as f32) * w2) > theta {
        1
    }
    else {
        0
    }
}

fn nand(x1: i32, x2: i32) -> i32 {
    let w1 = -0.5;
    let w2 = -0.5;
    let theta = -0.7;

    if ((x1 as f32) * w1 + (x2 as f32) * w2) > theta {
        1
    }
    else {
        0
    }
}

fn xor(x1: i32, x2: i32) -> i32 {
    and(nand(x1, x2), or(x1, x2))
}

