type recurrence =
  | Millisecond(int)
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrence,
    invoke: unit => unit
};

type jobId;

exception TimerIsMissing;
exception NoActiveJobWithId(jobId);

type t;

let create: unit => t;
let add: (t, job) => jobId;
let remove: (t, jobId) => unit;