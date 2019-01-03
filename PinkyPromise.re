type promise('a) =
  | Pending(list('a => unit))
  | Resolved('a);

type t('a) = {state: ref(promise('a))};

let return = x => {state: ref(Resolved(x))};

let make = () => {
  let promise = {state: ref(Pending([]))};
  let resolve = x => {
    switch (promise.state^) {
    | Pending(cbs) =>
      promise.state := Resolved(x);
      cbs |> List.iter(cb => cb(x));
    | Resolved(_) => ()
    };
  };
  (promise, resolve);
};

let bind: ('a => t('b), t('a)) => t('b) =
  (f, p) => {
    switch (p.state^) {
    | Resolved(res) => f(res)
    | Pending(cbs) =>
      let (p', res) = make();
      let f' = a => {
        let fp = f(a);
        switch (fp.state^) {
        | Resolved(x) => res(x)
        | Pending(cbs) => fp.state := Pending([res, ...cbs])
        };
      };
      p.state := Pending([f', ...cbs]);
      p';
    };
  };

let (>>=) = (p, f) => bind(f, p);

let wait = (t, a) => {
  let (p, res) = make();
  Unix.sleep(t);
  res(a);
  p;
};

let (p, res) = make();
let (q, res2) = make();

p
>>= (
  x => {
    Printf.printf("%d %!", x);
    q;
  }
)
>>= wait(2)
>>= (
  y => {
    Printf.printf("%d %!", y);
    return();
  }
);

res(1);
res2(20);
