open Jest;
open Expect;

type timerId;
[@bs.val] external setTimeout : (unit => unit, int) => timerId = "setTimeout";

describe("Scheduler", () => {  
    testPromise("add", () => {
        let scheduler = Scheduler.create();
        let recurrence = Scheduler.Millisecond(500);
        let counter = ref(0);
        let job: Scheduler.job = {
            period: recurrence,
            invoke: () => { counter := counter^ + 1 }
        };
        Scheduler.add(scheduler, job) |> ignore;

        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. counter^), 2100);
        });

        promise
        |> Js.Promise.then_(count => {
              let assertion = expect(count) |> toEqual(4);
              Js.Promise.resolve(assertion);
            });
    });

    /* Removing the head job of the queue should result in a queue without the head element
    and remaining jobs should function properly */
    testPromise("removeHead", () => {
        let counter_1 = ref(0);
        let counter_2 = ref(0);
        let job_1: Scheduler.job = {
            period: Millisecond(500),
            invoke: () => { counter_1 := counter_1^ + 1 }
        }
        let job_2: Scheduler.job = {
            period: Millisecond(1500),
            invoke: () => { counter_2 := counter_2^ + 1 }
        }
        let scheduler = Scheduler.create();
        let job_1_id = Scheduler.add(scheduler, job_1);
        Scheduler.add(scheduler, job_2) |> ignore;

        setTimeout(() => Scheduler.remove(scheduler, job_1_id), 750) |> ignore;
        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. (counter_1^, counter_2^)), 2500);
        });

        promise
        |> Js.Promise.then_(counters => {
              let assertion = expect(counters) |> toEqual((1,1));
              Js.Promise.resolve(assertion);
            });
    });

    /* Removing a job in the middle of the queue should result in a queue without said job 
    and with all other jobs functioning properly*/
    testPromise("removeMiddle", () => {
        let counter_1 = ref(0);
        let counter_2 = ref(0);
        let counter_3 = ref(0);
        let job_1: Scheduler.job = {
            period: Millisecond(500),
            invoke: () => { counter_1 := counter_1^ + 1 }
        };
        let job_2: Scheduler.job = {
            period: Second(1),
            invoke: () => { counter_2 := counter_2^ + 1 }
        };
        let job_3: Scheduler.job = {
            period: Millisecond(2500),
            invoke: () => { counter_3 := counter_3^ + 1 }
        };
        
        let scheduler = Scheduler.create();
        Scheduler.add(scheduler, job_1) |> ignore;
        Scheduler.add(scheduler, job_3) |> ignore;
        let job_2_id = Scheduler.add(scheduler, job_2);
          
        setTimeout(() => Scheduler.remove(scheduler, job_2_id), 1100) |> ignore;
        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. (counter_1^, counter_2^, counter_3^)), 3100);
        });

        promise
        |> Js.Promise.then_(counters => {
              let assertion = expect(counters) |> toEqual((6,1,1));
              Js.Promise.resolve(assertion);
            });
    });
    
    /* Removing the only job in the queue should result in an empty queue */
    testPromise("removeSingle", () => {
        let counter = ref(0);
        let job_1: Scheduler.job = {
            period: Millisecond(500),
            invoke: () => { counter := counter^ + 1 }
        }
        let scheduler = Scheduler.create();
        let job_1_id = Scheduler.add(scheduler, job_1);

        setTimeout(() => Scheduler.remove(scheduler, job_1_id), 600) |> ignore;
        let promise = Js.Promise.make((~resolve, ~reject) => {
            let _ = setTimeout(() => resolve(. counter^), 1000);
        });

        promise
        |> Js.Promise.then_(count => {
              let assertion = expect(count) |> toEqual(1);
              Js.Promise.resolve(assertion);
            });
    });
});