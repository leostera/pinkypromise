# PinkyPromise 🤞

> Disclaimer: some of this might be horribly wrong, please correct me if it is!

A silly little unbreakable promises library for Reason built during a
[ReasonableCoding](https://www.youtube.com/channel/UC8318qJJvq1VjFVXtc75J9Q)
Live-stream.

During the stream, we spend around 4 hours looking into Lwt and it's codebase
and documentation, trying to understand how is it that it works internally.

The key learnings are outlined below:

1. Promises are data.

2. The event-loop waits for some IO to happen before resolving the top-level
   promise.

3. A promise that is resolved, will call all of it's callbacks with it's
   resolution value. Those callbacks may in turn make ask the event-loop to
   perform some IO and wake them up when it's completed.

With this information, we set out to build a tiny promises library that could
more or less be used to the same result.

We found, however, that to provide actual asynchronicity, the underlying 
platform needs to have some async primitives (such as Promises in a Javascript
Engine, or Async I/O at the operating system level).

Thus, PinkyPromise remains a tiny monad to sequence computations that will run
sequentially.

## Stream Notes

```ocaml
Lwt_main.run
  Lwt_io.readdir <- resolves [".", "..", "Lwt.md" ]
    f             <- call f([".", "..", "Lwt.md" ])
    g             <- call g([".", "..", "Lwt.md" ])
                 <- return to Lwt_main.run
```

1. Lwt_main executes the related IO to the `Lwt_io.readdir` promise: spawn a
   process that will run `ls` and it will link the promise to that process
   returning a value.

2. Lwt_main sleeps until the process is finished, and then resolves the
   `Lwt_io.readdir` promise with the returned value.

3. `Lwt_io.readdir` will call then all of it's subscribers, which may go back to
   1.

...

4. Eventually control flow returns to `Lwt_main.run` and the event-loop
   will finish.


#### Promise Type

```ocaml
type +'a t (* type of the promise  *)
type -'b u (* type of the resolver *)

(*
  since the type of a function is

  type -'b +'a f = 'b -> 'a

  u = input
  t = output
*)
```
