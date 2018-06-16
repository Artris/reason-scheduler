open Jest;
open Expect;

describe("Heap", () => {  
    test("heap sort", () => {
        let heap = Heap.make(~key= 10, ~value= "10", ~comparator= (a, b) => a > b);
        Heap.insert(heap, 9, "9");
        Heap.insert(heap, 8, "8");
        Heap.insert(heap, 7, "7");
        Heap.insert(heap, 3, "3");
        Heap.insert(heap, 2, "2");
        Heap.insert(heap, 1, "1");
        Heap.insert(heap, 6, "6");
        Heap.insert(heap, 5, "5");
        Heap.insert(heap, 4, "4");

        let e1 = Heap.extract_min(heap);
        let e2 = Heap.extract_min(heap);
        let e3 = Heap.extract_min(heap);
        let e4 = Heap.extract_min(heap);
        let e5 = Heap.extract_min(heap);
        let e6 = Heap.extract_min(heap);
        let e7 = Heap.extract_min(heap);
        let e8 = Heap.extract_min(heap);
        let e9 = Heap.extract_min(heap);

        expect((e1, e2, e3, e4, e5, e6, e7, e8, e9)) |> toEqual((
            Some("1"), Some("2"), Some("3"),
            Some("4"), Some("5"), Some("6"),
            Some("7"), Some("8"), Some("9")
        ));
    });
});