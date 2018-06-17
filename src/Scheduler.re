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
  queue: Heap.t(long, job),
  timer: ref(option(timerId))
};

exception EmptyQueue;

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
  let job = Heap.extract_min(scheduler.queue).value; 
  let new_key = next_invokation(job);
  Heap.add(new_key, job, scheduler.queue);

  let key = Heap.min(scheduler.queue).key;
  let timeout = subtract(key, time_now());
  let timer_id = setTimeout(execute(scheduler), timeout);
  scheduler.timer := Some(timer_id);
  job.invoke(); 
};

exception TimerIsMissing;

let add = (scheduler, job) => {
  let queue = scheduler.queue;
  let queue_size = Heap.size(queue);
  let next_invokation = next_invokation(job);
  let () = switch queue_size {
  | 0 => {
    Heap.add(next_invokation, job, queue);
    let wait = wait(job.period);
    scheduler.timer := Some(setTimeout(execute(scheduler), wait));
  }
  | _ => {
    let key = Heap.min(scheduler.queue).key;
    Heap.add(next_invokation, job, queue);
    if(is_greater(key, next_invokation)){
      let timer_id = switch scheduler.timer^ {
      | None => raise(TimerIsMissing)
      | Some(id) => id
      };
      clearTimeout(timer_id);
      let wait = wait(job.period);
      let timer_id = setTimeout(execute(scheduler), wait);
      scheduler.timer := Some(timer_id);
    }
  }
  }

};

let create = () => {
  queue: Heap.create(is_greater),
  timer: ref(None)
};