// 摂氏から華氏への換算：F＝1.8×T＋32
// 華氏から摂氏への換算：C＝T/1.8－32/1.8

fn main() {
    let temperature: f64 = 16.0;

    println!("F:{} -> C:{}", temperature, f_to_celsius(temperature));
    println!("C:{} -> F:{}", temperature, c_to_fahrenheit(temperature));
}

fn f_to_celsius(t: f64) -> f64 {
    t/1.8 - 32.0/1.8
}

fn c_to_fahrenheit(t: f64) -> f64 {
    1.8 * t + 32.0
}
