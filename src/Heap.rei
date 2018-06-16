type heap('a);
let make: (int, 'a) => heap('a);
let insert: (heap('a), int, 'a) => heap('a);
let extract_min: heap('a) => (option('a), heap('a));
let min: heap('a) => option('a);
let inspect: heap('a) => unit;