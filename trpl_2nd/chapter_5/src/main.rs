
struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}

fn main() {

    let mut user1 = User {
        email: String::from("someoneexample.com"),
        username: String::from("someusername123"),
        active: true,
        sign_in_count: 1,
    };

    let user2 = User {
        email: String::from("hogehoge@example.com"),
        username: String::from("user2"),
        ..user1
    };
    user1.email = String::from("anotheremail@example.com");

    println!("{}", user1.email);
}

fn build_user(email: String, username: String) -> User {
    User {
        email,
        username,
        active: true,
        sign_in_count: 1,
    }
}
