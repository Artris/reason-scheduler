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
    | index => Some((index - 1) / 2);

let left = index => 2 * index + 1;
let right = index => 2 * (index + 1);

let swap = (a, b, queue) => {
    let a' = Array.get(queue, a);
    let b' = Array.get(queue, b);
    Array.set(queue, a, b');
    Array.set(queue, b, a');
}

let key = (queue, index) => Array.get(queue, index).key;

let rec heapify = (index, compare, queue) => {
    let key = key(queue);
    let heap_size = Array.length(queue);
    let left_index = left(index);
    let right_index = right(index);

    let max_priority_index = ref(index);
    max_priority_index :=
        if (left_index < heap_size &&
            compare(key(left_index), key(max_priority_index^))){ left_index } 
        else { max_priority_index^ };
    
        max_priority_index :=
        if (right_index < heap_size &&
            compare(key(right_index), key(max_priority_index^))){ right_index } 
        else { max_priority_index^ };
    
    let max_priority_index = max_priority_index^;
    if(max_priority_index != index){
        swap(max_priority_index, index, queue);
        heapify(max_priority_index, compare, queue);
    }
}

let rec fix_up = (index, compare, queue) => {
    let key = key(queue);
    let parent_index = parent(index);

    switch parent_index {
    | Some(p_ind) when compare(key(index), key(p_ind)) => {
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

let extract = heap => {
    switch heap.queue^ {
    | [||] => raise(EmptyQueue)
    | [|head|] => {
        heap.queue := [||];
        head;
    }
    | q => {
        let heap_size = Array.length(q);
        let head = Array.get(q, 0);
        swap(0, heap_size - 1, q);
        let q = Array.sub(q, 0, heap_size - 1);
        heapify(0, heap.compare, q);
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
    fix_last(heap.compare, queue);
    heap.queue := queue;
}

let search = (match, heap) => {
    let q = heap.queue^;
    let heap_size = Array.length(q);

    let rec checkSuffix = ind => {
        switch ind {
        | ind when (ind == heap_size) => None;
        | _ => {
            let value = Array.get(q, ind).value;
            if (match(value)) {
                Some(ind);
            } else {
                checkSuffix(ind + 1);    
            };
        };
        };
    };
    checkSuffix(0);
};

exception RemoveElementNotFound;

let remove = (match, heap) => {
    let index = search(match, heap);
    let q = heap.queue^;
    let heap_size = Array.length(q);
    switch index {
    | None => raise(RemoveElementNotFound);
    | Some(index) => {
        swap(index, heap_size - 1, q);
        let q = Array.sub(q, 0, heap_size - 1);
        heapify(index, heap.compare, q);
        heap.queue := q; 
    }
    };
};

let head = heap => {
    switch heap.queue^ {
    | [||] => raise(EmptyQueue)
    | q => Array.get(q, 0)
    };
}
let update_priority = (index, new_priority, heap) => {
    let queue = heap.queue^;
    let current_priority = key(queue, index);
    let value = Array.get(queue, index).value;
    Array.set(queue, index, {key: new_priority, value: value});

    let has_higher_priority = heap.compare(new_priority, current_priority);
    if(has_higher_priority){
        fix_up(index, heap.compare, queue)
    } else {
        heapify(index, heap.compare, queue)
    }
}

exception HasHigherPriority;

let decrease_root_priority = (new_priority, heap) => {
    let queue = heap.queue^;
    let current_priority = key(queue, 0);
    let has_higher_priority = heap.compare(new_priority, current_priority);
    if(has_higher_priority){
        raise(HasHigherPriority);
    } else {
        update_priority(0, new_priority, heap);
    }
}

let size = heap => Array.length(heap.queue^);

let inspect = heap => Js.Array.toString(heap.queue^);