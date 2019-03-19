//$ rustc --version
//rustc 1.32.0 (9fda7c223 2019-01-16)
//もう少しいけそう
fn main(){for i in 1..101{if i%3>0{if i%5>0{print!{"{}",i}}}else{print!{"Fizz"}}
println!{"{}",if i%5<1{"Buzz"}else{""}}}}
