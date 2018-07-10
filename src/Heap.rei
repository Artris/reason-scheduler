type heapElement('a, 'b) = {
    key: 'a,
    value: 'b
}
type t('a, 'b);

exception RemoveElementNotFound;

let create: (('a, 'a) => bool) => t('a, 'b);
let add: ('a, 'b, t('a, 'b)) => unit;
let remove: (('b) => bool, t('a, 'b)) => unit;
let extract: t('a, 'b) => heapElement('a, 'b);
let head: t('a, 'b) => heapElement('a, 'b);
let size: t('a, 'b) => int;
let decrease_root_priority: ('a, t('a, 'b)) => unit
let inspect: t('a, 'b) => string;