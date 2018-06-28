open Jest;
open Expect;

describe("Heap", () => {  
    test("heap sort", () => {
        let heap = Heap.create((a, b) => a < b);
        Heap.add(9, "9", heap);
        Heap.add(8, "8", heap);
        Heap.add(7, "7", heap);
        Heap.add(3, "3", heap);
        Heap.add(2, "2", heap);
        Heap.add(1, "1", heap);
        Heap.add(6, "6", heap);
        Heap.add(5, "5", heap);
        Heap.add(4, "4", heap);

        let {value: e1}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e2}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e3}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e4}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e5}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e6}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e7}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e8}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e9}: Heap.heapElement(int, string) = Heap.extract(heap);

        expect((e1, e2, e3, e4, e5, e6, e7, e8, e9)) |> toEqual(
            ("1", "2", "3", "4", "5", "6", "7", "8", "9"));
    });
    test("heap remove", () => {
        let heap = Heap.create((a, b) => a < b);
        Heap.add(9, "9", heap);
        Heap.add(8, "8", heap);
        Heap.add(7, "7", heap);
        Heap.add(3, "3", heap);
        Heap.add(2, "2", heap);
        Heap.add(1, "1", heap);
        Heap.add(6, "6", heap);
        Heap.add(5, "5", heap);
        Heap.add(4, "4", heap);
        
        /* Heap.print(heap); */
        Heap.remove(0, heap);
        /* Heap.print(heap); */
        Heap.remove(7, heap);
        /* Heap.print(heap); */
        Heap.remove(3, heap);
        /* Heap.print(heap); */

        /* let {value: e1}: Heap.heapElement(int, string) = Heap.extract(heap); */
        let {value: e2}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e3}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e4}: Heap.heapElement(int, string) = Heap.extract(heap);
        /* let {value: e5}: Heap.heapElement(int, string) = Heap.extract(heap); */
        /* let {value: e6}: Heap.heapElement(int, string) = Heap.extract(heap); */
        let {value: e7}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e8}: Heap.heapElement(int, string) = Heap.extract(heap);
        let {value: e9}: Heap.heapElement(int, string) = Heap.extract(heap);

        expect((e2, e3, e4,  e7, e8, e9)) |> toEqual(
            ("2", "3", "4", "7", "8", "9"));
    });
});