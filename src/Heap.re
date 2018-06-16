type heapElement('a) = {
    key: int,
    value: 'a
};

type heap('a) = array(heapElement('a));

let make = (key, value) => {
    let element = {key: key, value: value};
    Array.make(1, element);
}

let parent = 
    fun
    | 0 => None
    | index => Some(index / 2);

let left = index => 2 * index + 1;
let right = index => 2 * (index + 1);

let key = (heap, index) => Array.get(heap, index).key;

let swap = (heap, a, b) => {
    let a' = Array.get(heap, a);
    let b' = Array.get(heap, b);
    Array.set(heap, a, b');
    Array.set(heap, b, a');
}

let rec min_heapify = (heap, index) => {
    let key = key(heap);
    let heap_size = Array.length(heap);
    let left_index = left(index);
    let right_index = right(index);

    let min_index = ref(index);
    min_index :=
        if (left_index < heap_size && key(min_index^) > key(left_index)){ left_index } 
        else { min_index^ };
    
    min_index :=
        if (right_index < heap_size && key(min_index^) > key(right_index)){ right_index } 
        else { min_index^ };
    
    let min_index = min_index^;
    if(min_index != index){
        swap(heap, min_index, index);
        min_heapify(heap, min_index);
    }
}

let rec fix_up = (heap, index) => {
    let key = key(heap);
    let parent_index = parent(index);

    switch parent_index {
    | Some(parent_index) when key(index) < key(parent_index) => {
        swap(heap, index, parent_index);
        fix_up(heap, parent_index);
    }
    | _ => () 
    };
}

let fix_last = heap => {
    let heap_size = Array.length(heap);
    if(heap_size > 0){
        fix_up(heap, heap_size - 1);
    }
}

let extract_min = (heap) => {
    let heap_size = Array.length(heap);
    switch heap_size {
    | 0 => (None, heap)
    | 1 => {
        let value = Array.get(heap, 0).value;
        (Some(value), [||]);
    }
    | _ => {
        let value = Array.get(heap, 0).value;
        let heap = Array.copy(heap);
        swap(heap, 0, heap_size - 1);
        let heap = Array.sub(heap, 0, heap_size - 1);
        min_heapify(heap, 0);
        (Some(value), heap);
    }
    };
}

let insert = (heap, key, value) => {
    let element = {key: key, value: value};
    let heap = Array.append(heap, [|element|]);
    fix_last(heap);
    heap;
}

let min = heap => {
    let heap_size = Array.length(heap);
    switch (heap_size) {
    | 0 => None
    | _ => Some(Array.get(heap, 0).value)
    };
}

let inspect = heap => {
    Js.log(Array.map(e => e.value, heap));
}