open Jest;
open Expect;

describe("Heap", () => {
    open Heap;

    let num_keys = 30;

    let gen_keys = n => {
        let rec gen =
            fun
            | -1 => []
            | n => [n, ...gen(n - 1)];
        
        gen(n - 1);
    };
    
    let keys = gen_keys(num_keys);
    
    let setup = () => {
        let heap = create((a, b) => a < b);
        List.iter(key => add(key, string_of_int(key), heap), keys);
        heap;
    };

    test("size", () => {
        let heap = setup();

        let heap_size = size(heap);

        expect(heap_size) |> toBe(num_keys);
    });

    test("head", () => {
        let heap = setup();

        let head = head(heap);

        expect(head.value) |> toBe("0");
    });

    test("decrease_root_priority", () => {
        let heap = setup();

        decrease_root_priority(500, heap);
        decrease_root_priority(501, heap);
        decrease_root_priority(502, heap);
        let head = head(heap);

        expect(head.value) |> toBe("3");
    });

    test("heap sort", () => {
        let heap = setup();

        let expected = List.map(key => string_of_int(key), List.rev(keys));
        let result = List.map(_key => extract(heap).value, keys);

        expect(result) |> toEqual(expected);
    });
});