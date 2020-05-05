
pub trait Node {
    fn new() -> Self;
}

// These are the available root node types. In JSON it will either be an
// object or an array at the base.
#[derive(Debug)]
pub enum RootNodeType {
    ObjectRoot,
    ArrayRoot,
}

// RootNode is what starts every parsed AST. There is a `Type` field so that
// you can ask which root node type starts the tree.
#[derive(Debug)]
pub struct RootNode {
    pub value: Value,
    pub root_node_type: RootNodeType,
}

// Object represents a JSON object. It holds a slice of Property as its children,
// a Type ("Object"), and start & end code points for displaying.
#[derive(Debug, PartialEq)]
pub struct Object {
    pub object_type: &'static str,
    pub children: Vec<Property>,
    pub start: usize,
    pub end: usize,
}

impl Node for Object {
    fn new() -> Self {
        Object {
            object_type: "Object",
            children: Vec::new(),
            start: 0,
            end: 0,
        }
    }
}

// Array represents a JSON array It holds a slice of Value as its children,
// a Type ("Array"), and start & end code points for displaying.
#[derive(Debug, PartialEq)]
pub struct Array {
    pub array_type: &'static str,
    pub children: Vec<Option<Value>>,
    pub start: usize,
    pub end: usize,
}

impl Node for Array {
    fn new() -> Self {
        Array {
            array_type: "Array",
            children: Vec::new(),
            start: 0,
            end: 0,
        }
    }
}

// Literal represents a JSON literal value. It holds a Type ("Literal") and the actual value.
#[derive(Debug, PartialEq)]
pub struct Literal {
    pub literal_type: &'static str,
    pub value: Box<Value>,
}

impl Node for Literal {
    fn new() -> Self {
        Literal {
            literal_type: "Literal",
            value: Box::new(Value::Null),
        }
    }
}

// Property holds a Type ("Property") as well as a `Key` and `Value`. The Key is an Identifier
// and the value is any Value.
#[derive(Debug, PartialEq)]
pub struct Property {
    pub property_type: &'static str,
    pub key: Option<Identifier>,
    pub value: Option<Value>,
}

impl Node for Property {
    fn new() -> Self {
        Property {
            property_type: "Property",
            key: None,
            value: None,
        }
    }
}

// Identifier represents a JSON object property key
#[derive(Debug, PartialEq)]
pub struct Identifier {
    pub identifier_type: &'static str,
    pub value: String,
}

// Value will eventually have some methods that all Values must implement. For now
// it represents any JSON value (object | array | boolean | string | number | null)
#[derive(Debug, PartialEq)]
pub enum Value {
    Object(self::Object),
    Array(self::Array),
    Literal(self::Literal),
    Bool(bool),
    String(String),
    Number(i64),
    Null,
}

// Available states for each type used in parsing
#[derive(Debug)]
pub enum State {
    // Object states
    ObjStart,
    ObjOpen,
    ObjProperty,
    ObjComma,

    // Property states
    PropertyStart,
    PropertyKey,
    PropertyColon,

    // Array states
    ArrayStart,
    ArrayOpen,
    ArrayValue,
    ArrayComma,

    // String states
    StringStart,
    StringQuoteOrChar,
    Escape,

    // Number states
    NumberStart,
    NumberMinus,
    NumberZero,
    NumberDigit,
    NumberPoint,
    NumberDigitFraction,
    NumberExp,
    NumberExpDigitOrSign,
}
