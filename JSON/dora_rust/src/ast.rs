use std::collections::HashMap;

// These are the available root node types. In JSON it will either be an
// object or an array at the base.
pub enum RootNodeType {
    ObjectRoot,
    ArrayRoot,
}

// RootNode is what starts every parsed AST. There is a `Type` field so that
// you can ask which root node type starts the tree.
pub struct RootNode {
    pub value: Value,
    pub root_node_type: RootNodeType,
}

// Object represents a JSON object. It holds a slice of Property as its children,
// a Type ("Object"), and start & end code points for displaying.
pub struct Object {
    object_type: &'static str,
    children: Vec<Property>,
    start: usize,
    end: usize,
}

// Array represents a JSON array It holds a slice of Value as its children,
// a Type ("Array"), and start & end code points for displaying.
pub struct Array {
    array_type: &'static str,
    children: Vec<Value>,
    start: usize,
    end: usize,
}

// Literal represents a JSON literal value. It holds a Type ("Literal") and the actual value.
pub struct Literal {
    literal_type: &'static str,
    value: Value,
}

// Property holds a Type ("Property") as well as a `Key` and `Value`. The Key is an Identifier
// and the value is any Value.
pub struct Property {
    property_type: &'static str,
    key: Identifier,
    value: Value,
}

// Identifier represents a JSON object property key
pub struct Identifier {
    identifier_type: String,
    value: String,
}

// Value will eventually have some methods that all Values must implement. For now
// it represents any JSON value (object | array | boolean | string | number | null)
#[derive(PartialEq)]
pub enum Value {
    Object(HashMap<String, Value>),
    Array(Vec<Value>),
    Bool(bool),
    String(String),
    Number(i64),
    Null,
}

// Available states for each type used in parsing
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
