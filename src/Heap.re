type heapElement('a, 'b) = {
    key: 'a,
    value: 'b
};

type heap('a, 'b) = {
    memo: ref(array(heapElement('a, 'b))),
    compare: ('a, 'a) => bool 
};

let make = (~key, ~value, ~comparator) => {
    {
        memo: ref(Array.make(1, {key: key, value: value})),
        compare: comparator
    }
}

let parent = 
    fun
    | 0 => None
    | index => Some(index / 2);

let left = index => 2 * index + 1;
let right = index => 2 * (index + 1);

let key = (memo, index) => Array.get(memo, index).key;

let swap = (memo, a, b) => {
    let a' = Array.get(memo, a);
    let b' = Array.get(memo, b);
    Array.set(memo, a, b');
    Array.set(memo, b, a');
}

let rec min_heapify = (memo, index, compare) => {
    let key = key(memo);
    let heap_size = Array.length(memo);
    let left_index = left(index);
    let right_index = right(index);

    let min_index = ref(index);
    min_index :=
        if (left_index < heap_size && compare(key(min_index^), key(left_index))){ left_index } 
        else { min_index^ };
    
    min_index :=
        if (right_index < heap_size && compare(key(min_index^), key(right_index))){ right_index } 
        else { min_index^ };
    
    let min_index = min_index^;
    if(min_index != index){
        swap(memo, min_index, index);
        min_heapify(memo, min_index, compare);
    }
}

let rec fix_up = (memo, index) => {
    let key = key(memo);
    let parent_index = parent(index);

    switch parent_index {
    | Some(parent_index) when key(index) < key(parent_index) => {
        swap(memo, index, parent_index);
        fix_up(memo, parent_index);
    }
    | _ => () 
    };
}

let fix_last = memo => {
    let heap_size = Array.length(memo);
    if(heap_size > 0){
        fix_up(memo, heap_size - 1);
    }
}

let extract_min = heap => {
    let memo = heap.memo^;
    let heap_size = Array.length(memo);
    switch heap_size {
    | 0 => None
    | 1 => {
        let value = Array.get(memo, 0).value;
        heap.memo := [||];
        Some(value);
    }
    | _ => {
        let value = Array.get(memo, 0).value;
        let memo = Array.copy(memo);
        swap(memo, 0, heap_size - 1);
        let memo = Array.sub(memo, 0, heap_size - 1);
        min_heapify(memo, 0, heap.compare);
        heap.memo := memo;
        Some(value);
    }
    };
}

let insert = (heap, key, value) => {
    let element = {key: key, value: value};
    let memo = Array.append(heap.memo^, [|element|]);
    fix_last(memo);
    heap.memo := memo;
}

let min = heap => {
    let heap_size = Array.length(heap.memo^);
    switch (heap_size) {
    | 0 => None
    | _ => Some(Array.get(heap.memo^, 0).value)
    };
}

let inspect = heap => Js.log(heap.memo^);