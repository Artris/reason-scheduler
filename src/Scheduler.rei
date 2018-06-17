type recurrance =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrance,
    invoke: unit => unit
};

type scheduler;

exception EmptyQueue;

let create: unit => scheduler;
let add: (scheduler, job) => unit