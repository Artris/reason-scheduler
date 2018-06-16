let job_1: Scheduler.job = {
    period: Second(5),
    invoke: () => {Js.log("Hi from the first Job")}
}

let job_2: Scheduler.job = {
    period: Second(2),
    invoke: () => {Js.log("Hi from second Job")}
}

let scheduler = Scheduler.make(job_1);
Scheduler.create(scheduler, job_2);
