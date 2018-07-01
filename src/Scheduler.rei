type recurrence =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrence,
    mutable id: int,
    invoke: unit => unit
};

exception TimerIsMissing;
type t;

let create: unit => t;
let add: (t, job) => unit;
let remove: (t, int) => unit;