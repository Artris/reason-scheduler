type heapElement('a, 'b) = {
    key: 'a,
    value: 'b
}

type t('a, 'b) = {
    queue: ref(array(heapElement('a, 'b))),
    compare: ('a, 'a) => bool 
};

exception EmptyQueue;

let create = compare => {
    queue: ref([||]),
    compare: compare
};

let parent = 
    fun
    | 0 => None
    | index => Some(index / 2);

let left = index => 2 * index + 1;
let right = index => 2 * (index + 1);

let swap = (a, b, queue) => {
    let a' = Array.get(queue, a);
    let b' = Array.get(queue, b);
    Array.set(queue, a, b');
    Array.set(queue, b, a');
}

let key = (queue, index) => Array.get(queue, index).key;

let rec min_heapify = (index, compare, queue) => {
    let key = key(queue);
    let heap_size = Array.length(queue);
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
        swap(min_index, index, queue);
        min_heapify(min_index, compare, queue);
    }
}

let rec fix_up = (index, compare, queue) => {
    let key = key(queue);
    let parent_index = parent(index);

    switch parent_index {
    | Some(p_ind) when key(index) < key(p_ind) => {
        swap(index, p_ind, queue);
        fix_up(p_ind, compare, queue);
    }
    | _ => () 
    };
}

let fix_last = (compare, queue) => {
    let heap_size = Array.length(queue);
    fix_up(heap_size - 1, compare, queue);
}

let extract_min = heap => {
    switch heap.queue^ {
    | [||] => raise(EmptyQueue)
    | [|min|] => {
        heap.queue := [||];
        min;
    }
    | q => {
        let heap_size = Array.length(q);
        let head = Array.get(q, 0);
        swap(0, heap_size - 1, q);
        let q = Array.sub(q, 0, heap_size - 1);
        min_heapify(0, heap.compare, q);
        heap.queue := q;
        head;
    }
    };
}

let add = (key, value, heap) => {
    let queue = switch heap.queue^ {
    | [||] => [|{key, value}|]
    | q => Array.append(q, [|{key, value}|])
    };
    fix_last(compare, queue);
    heap.queue := queue;
}

let min = heap => {
    switch heap.queue^ {
    | [||] => raise(EmptyQueue)
    | q => Array.get(q, 0)
    };
}

let update_key = (index, next_key, heap) => {
    let queue = heap.queue^;
    let current_key = key(queue, index);
    let value = Array.get(queue, index).value;
    let is_greater = heap.compare(next_key, current_key);
    Array.set(queue, index, {key: next_key, value: value});
    if(is_greater){ min_heapify(index, heap.compare, queue)}
    else { fix_up(index, heap.compare, queue) }
}

let update_root_key = (next_key, heap) => {
    update_key(0, next_key, heap);
}

let size = heap => Array.length(heap.queue^)

let inspect = heap => Js.Array.toString(heap.queue^)
