type recurrance =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrance,
    invoke: unit => unit
};

type t;

let create: unit => t;
let add: (t, job) => unit