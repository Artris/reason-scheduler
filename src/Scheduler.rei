type recurrence =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrence,
    invoke: unit => unit
};

exception TimerIsMissing;
type t;

let create: unit => t;
let add: (t, job) => unit