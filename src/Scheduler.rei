type recurrance =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrance,
    invoke: unit => unit
};

type scheduler;

let make: job => scheduler;
let create: (scheduler, job) => unit