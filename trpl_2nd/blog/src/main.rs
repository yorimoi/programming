extern crate blog;
use blog::Post;

fn main() {
    let mut post = Post::new();  // Draft

    post.add_text("I ate a salad for lunch today");  // Draft
    assert_eq!("", post.content());

    post.request_review();  // PendingReview
    assert_eq!("", post.content());

    post.approve();  // Published
    assert_eq!("I ate a salad for lunch today", post.content());

    post.reject();  // PendingReview
    assert_eq!("", post.content());

    post.reject();  // Draft
    assert_eq!("", post.content());

    post.reject();  // Draft
    assert_eq!("", post.content());

    post.request_review();  // PendingReview
    assert_eq!("", post.content());

    post.approve();  // Published
    assert_eq!("I ate a salad for lunch today", post.content());
}

