open Jest;
open Expect;

describe("Heap", () => {  
    test("heap sort", () => {
        let heap = Heap.make(10, "10");
        let heap = Heap.insert(heap, 9, "9");
        let heap = Heap.insert(heap, 8, "8");
        let heap = Heap.insert(heap, 7, "7");
        let heap = Heap.insert(heap, 3, "3");
        let heap = Heap.insert(heap, 2, "2");
        let heap = Heap.insert(heap, 1, "1");
        let heap = Heap.insert(heap, 6, "6");
        let heap = Heap.insert(heap, 5, "5");
        let heap = Heap.insert(heap, 4, "4");

        let (e1, heap) = Heap.extract_min(heap);
        let (e2, heap) = Heap.extract_min(heap);
        let (e3, heap) = Heap.extract_min(heap);
        let (e4, heap) = Heap.extract_min(heap);
        let (e5, heap) = Heap.extract_min(heap);
        let (e6, heap) = Heap.extract_min(heap);
        let (e7, heap) = Heap.extract_min(heap);
        let (e8, heap) = Heap.extract_min(heap);
        let (e9, _) = Heap.extract_min(heap);

        expect((e1, e2, e3, e4, e5, e6, e7, e8, e9)) |> toEqual((
            Some("1"), Some("2"), Some("3"),
            Some("4"), Some("5"), Some("6"),
            Some("7"), Some("8"), Some("9")
        ));
    });
});