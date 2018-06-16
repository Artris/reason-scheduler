type heap('a, 'b);
let make: (~key: 'a, ~value: 'b, ~comparator: ('a, 'a) => bool) => heap('a, 'b);
let insert: (heap('a, 'b), 'a, 'b) => unit;
let extract_min: heap('a, 'b) => option('b);
let min: heap('a, 'b) => option('b);
let inspect: heap('a, 'b) => unit;