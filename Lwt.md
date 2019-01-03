# Lwt: Concurrent Programming in Reason

Promises are data.

There's a mapping between a promise and some underlying I/O that will be
executed.

The main I/O loop will yield our program and once a certain I/O operation
finished, it will look into the mapping and resolve the appropriate promise.

> Note: the mapping may not be a table, it may just be a function?

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


## Promise Type

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
