open Jest;
open Expect;

type timerId;
[@bs.val] external setTimeout : (unit => unit, int) => timerId = "setTimeout";

describe("Scheduler", () => {  
    testPromise("add", () => {
        let scheduler = Scheduler.create();
        let recurrence = Scheduler.Second(1);
        let counter = ref(0);
        let job: Scheduler.job = {
            period: recurrence,
            invoke: () => { counter := counter^ + 1 }
        };
        Scheduler.add(scheduler, job) |> ignore;

        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. counter^), 4100);
        });

        promise
        |> Js.Promise.then_(count => {
              let assertion = expect(count) |> toEqual(4);
              Js.Promise.resolve(assertion);
            });
    });

    testPromise("remove", () => {
        let scheduler = Scheduler.create();
        let recurrence = Scheduler.Second(1);
        let counter = ref(0);
        let job: Scheduler.job = {
            period: recurrence,
            invoke: () => { counter := counter^ + 1 }
        };
        let job_id = Scheduler.add(scheduler, job);

        setTimeout(() => Scheduler.remove(scheduler, job_id), 2100) |> ignore;

        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. counter^), 4100);
        });

        promise
        |> Js.Promise.then_(count => {
              let assertion = expect(count) |> toEqual(2);
              Js.Promise.resolve(assertion);
            });
    });
});