type timerId;
[@bs.val] external setTimeout : (unit => unit, int) => timerId = "setTimeout";
[@bs.val] external clearTimeout : timerId => unit = "clearTimeout";

type long;
let time_now: unit => long = [%raw "() => { return new Date().getTime(); }"];
let sum: (long, int) => long = [%raw "(a, b) => a + b"];
let subtract: (long, long) => int = [%raw "(a, b) => a - b"];
let is_greater: (long, long) => bool = [%raw "(a, b) => a > b"];

type recurrance =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrance,
    invoke: unit => unit
};

type scheduler = {
  queue: Heap.heap(long, job),
  timer: ref(timerId)
};

exception EmptyHeap;

let wait = period => {
  switch period {
    | Second(s) => s * 1000
    | Minute(m) => m * 60 * 1000
    | Hour(h) => h * 60 * 60 * 1000
  };
};

let next_invokation = job => {
  let wait = wait(job.period);
  sum(time_now(), wait);
}

let rec execute = scheduler => () => {
  let head = Heap.extract_min(scheduler.queue);
  let job = switch head {
  | Some((_, job)) => job
  | _ => raise(EmptyHeap)
  };
  let new_key = next_invokation(job);
  Heap.insert(scheduler.queue, new_key, job);

  let head = Heap.min(scheduler.queue);
  let (key, job) = switch head {
  | Some(element) => element
  | _ => raise(EmptyHeap)
  };

  let timeout = subtract(key, time_now());
  let timer_id = setTimeout(execute(scheduler), timeout);
  scheduler.timer := timer_id;
  job.invoke(); 
};

let create = (scheduler, job) => {
  let next_invokation = next_invokation(job);
  let head = Heap.min(scheduler.queue);
  let (key, _) = switch head {
  | Some(element) => element
  | _ => raise(EmptyHeap)
  };

  Heap.insert(scheduler.queue, next_invokation, job);
  if(is_greater(key, next_invokation)){
    clearTimeout(scheduler.timer^);
    scheduler.timer := setTimeout(execute(scheduler), wait(job.period));
  }
}

let make = job => {
  let next_invokation = next_invokation(job);
  let queue = Heap.make(~key= next_invokation, ~value=job, ~comparator=is_greater);
  let scheduler = {
    queue: queue,
    timer: ref(setTimeout(()=>(), 1))
  };
  scheduler.timer := setTimeout(execute(scheduler), wait(job.period));
  scheduler;
}