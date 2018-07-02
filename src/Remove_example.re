type timerId;
[@bs.val] external setTimeout : (unit => unit, int) => timerId = "setTimeout";

let job_1: Scheduler.job = {
    period: Second(4),
    invoke: () => Js.log("Hi from the first Job")
}

let scheduler = Scheduler.create();
let jobId = Scheduler.add(scheduler, job_1);



let remove = () => {
    Scheduler.remove(scheduler, jobId);
};
setTimeout(remove, 8200);
