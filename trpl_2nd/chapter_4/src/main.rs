fn main() {
    let s = "hello world!";
    let word = first_word(&s);
    println!("{}", word);
    let a = [1, 2, 3, 4, 5];
    let slice = &a[..3];
    println!("{}", slice.len());
}

fn first_word(s: &str) -> &str {
    let bytes = s.as_bytes();

    for(i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }
    &s[..]
}
