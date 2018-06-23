type timerId;
[@bs.val] external setTimeout : (unit => unit, int) => timerId = "setTimeout";
[@bs.val] external clearTimeout : timerId => unit = "clearTimeout";

type long;
let time_now: unit => long = [%raw "() => { return new Date().getTime(); }"];
let sum: (long, int) => long = [%raw "(a, b) => a + b"];
let subtract: (long, long) => int = [%raw "(a, b) => a - b"];
let has_higher_priority: (long, long) => bool = [%raw "(a, b) => a < b"];

type recurrence =
  | Second(int)
  | Minute(int)
  | Hour(int);

type job = {
    period: recurrence,
    invoke: unit => unit
};

type t = {
  queue: Heap.t(long, job),
  timer_id: ref(option(timerId))
};

let wait = period => {
  switch period {
    | Second(s) => s * 1000
    | Minute(m) => m * 60 * 1000
    | Hour(h) => h * 60 * 60 * 1000
  };
};

let next_invocation = job => {
  let wait = wait(job.period);
  sum(time_now(), wait);
}

let rec execute = scheduler => () => {
  let job = Heap.head(scheduler.queue).value; 
  let new_key = next_invocation(job);
  Heap.decrease_root_priority(new_key, scheduler.queue);

  let key = Heap.head(scheduler.queue).key;
  let timeout = subtract(key, time_now());
  let timer_id = setTimeout(execute(scheduler), timeout);
  scheduler.timer_id := Some(timer_id);
  job.invoke(); 
};

exception TimerIsMissing;

let add = (scheduler, job) => {
  let queue = scheduler.queue;
  let queue_size = Heap.size(queue);
  let next_invocation = next_invocation(job);
  let () = switch queue_size {
  | 0 => {
    Heap.add(next_invocation, job, queue);
    let wait = wait(job.period);
    scheduler.timer_id := Some(setTimeout(execute(scheduler), wait));
  }
  | _ => {
    let key = Heap.head(scheduler.queue).key;
    Heap.add(next_invocation, job, queue);
    if(has_higher_priority(next_invocation, key)){
      let timer_id = switch scheduler.timer_id^ {
      | None => raise(TimerIsMissing)
      | Some(id) => id
      };
      clearTimeout(timer_id);
      let wait = wait(job.period);
      let timer_id = setTimeout(execute(scheduler), wait);
      scheduler.timer_id := Some(timer_id);
    }
  }
  }

};

let create = () => {
  queue: Heap.create(has_higher_priority),
  timer_id: ref(None)
};