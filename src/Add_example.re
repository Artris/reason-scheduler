let job_1: Scheduler.job = {
    period: Second(1),
    invoke: () => Js.log("Hi from the first Job")
}

let job_2: Scheduler.job = {
    period: Second(1),
    invoke: () => Js.log("Hi from second Job")
}
let job_3: Scheduler.job = {
    period: Second(2),
    invoke: () => Js.log("Hi from third Job")
}

let scheduler = Scheduler.create();
Scheduler.add(scheduler, job_1);
let jobId = Scheduler.add(scheduler, job_2); 
Scheduler.reschedule(scheduler, job_3, jobId);