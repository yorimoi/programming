//! [オブジェクト指向デザインパターンを実装する](https://doc.rust-jp.rs/book/second-edition/ch17-03-oo-design-patterns.html)

#[derive(PartialEq)]
enum States {
    Draft,
    PendingReview,
    Published,
}

pub struct Post {
    state: Option<Box<dyn State>>,
    content: String,
    approvable: bool,
}

impl Post {
    pub fn new() -> Post {
        Post {
            state: Some(Box::new(Draft {})),
            content: String::new(),
            approvable: false,
        }
    }

    pub fn add_text(&mut self, text: &str) -> Result<(), &str> {
        if self.state() == &States::Draft {
            self.content.push_str(text);
            Ok(())
        } else {
            Err("can't add text to the post when it is in a state other than draft")
        }
    }

    pub fn content(&self) -> &str {
        self.state.as_ref().unwrap().content(&self)
    }

    pub fn request_review(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.request_review())
        }
    }

    pub fn approve(&mut self) {
        if self.approvable {
            if let Some(s) = self.state.take() {
                self.state = Some(s.approve())
            }
        } else {
            self.approvable = true;
        }
    }

    pub fn reject(&mut self) {
        self.approvable = false;
        if let Some(s) = self.state.take() {
            self.state = Some(s.reject())
        }
    }

    fn state(&self) -> &'static States {
        self.state.as_ref().unwrap().state()
    }
}

trait State {
    fn request_review(self: Box<Self>) -> Box<dyn State>;
    fn approve(self: Box<Self>) -> Box<dyn State>;
    fn content<'a>(&self, _post: &'a Post) -> &'a str {
        ""
    }
    fn reject(self: Box<Self>) -> Box<dyn State>;
    fn state(&self) -> &'static States;
}

struct Draft {}

impl State for Draft {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        Box::new(PendingReview {})
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn reject(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn state(&self) -> &'static States {
        &States::Draft
    }
}

struct PendingReview {}

impl State for PendingReview {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        Box::new(Published {})
    }

    fn reject(self: Box<Self>) -> Box<dyn State> {
        Box::new(Draft {})
    }

    fn state(&self) -> &'static States {
        &States::PendingReview
    }
}

struct Published {}

impl State for Published {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn content<'a>(&self, post: &'a Post) -> &'a str {
        &post.content
    }

    fn reject(self: Box<Self>) -> Box<dyn State> {
        Box::new(PendingReview {})
    }

    fn state(&self) -> &'static States {
        &States::Published
    }
}

