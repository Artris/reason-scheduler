let job_1: Scheduler.job = {
    period: Second(4),
    id: 1,
    invoke: () => Js.log("Hi from the first Job")
}

let job_2: Scheduler.job = {
    period: Second(1),
    id: 2,
    invoke: () => Js.log("Hi from second Job")
}

let scheduler = Scheduler.create();
Scheduler.add(scheduler, job_1);
Scheduler.add(scheduler, job_2);
