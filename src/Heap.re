type heapElement('a, 'b) = {
    key: 'a,
    value: 'b
}

type t('a, 'b) = {
    memo: ref(array(heapElement('a, 'b))),
    compare: ('a, 'a) => bool 
};

exception EmptyQueue;

let create = compare => {
    memo: ref([||]),
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

let rec fix_up = (index, queue) => {
    let key = key(queue);
    let parent_index = parent(index);

    switch parent_index {
    | Some(p_ind) when key(index) < key(p_ind) => {
        swap(index, p_ind, queue);
        fix_up(p_ind, queue);
    }
    | _ => () 
    };
}

let fix_last = queue => {
    let heap_size = Array.length(queue);
    fix_up(heap_size - 1, queue);
}

let extract_min = heap => {
    switch heap.memo^ {
    | [||] => raise(EmptyQueue)
    | [|min|] => {
        heap.memo := [||];
        min;
    }
    | q => {
        let heap_size = Array.length(q);
        let head = Array.get(q, 0);
        swap(0, heap_size - 1, q);
        let q = Array.sub(q, 0, heap_size - 1);
        min_heapify(0, heap.compare, q);
        heap.memo := q;
        head;
    }
    };
}

let add = (key, value, heap) => {
    let queue = switch heap.memo^ {
    | [||] => [|{key, value}|]
    | q => Array.append(q, [|{key, value}|])
    };
    fix_last(queue);
    heap.memo := queue;
}

let min = heap => {
    switch heap.memo^ {
    | [||] => raise(EmptyQueue)
    | q => Array.get(q, 0)
    };
}

let size = heap => Array.length(heap.memo^)

let inspect = heap => Js.Array.toString(heap.memo^)