type recurrence =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrence,
    invoke: unit => unit
};

type jobId;
type t;

let create: unit => t;
let add: (t, job) => jobId;
let remove: (t, jobId) => unit;