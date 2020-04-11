[@ocaml.ppx.context
  {
    tool_name: "migrate_driver",
    include_dirs: [],
    load_path: [],
    open_modules: [],
    for_package: None,
    debug: false,
    use_threads: false,
    use_vmthreads: false,
    recursive_types: false,
    principal: false,
    transparent_modules: false,
    unboxed_types: false,
    unsafe_string: false,
    cookies: [],
  }
];

module Fragments = {
  module ListFragment = {
    let query = "fragment ListFragment on Lists   {\nnullableOfNullable  \nnullableOfNonNullable  \n}\n";
    type t = {
      .
      "nullableOfNullable": option(array(option(string))),
      "nullableOfNonNullable": option(array(string)),
    };
    type t_Lists = t;

    let parse: Raw.t => t =
      (value: Js.Json.t) => {

        "nullableOfNullable": {
          let value = value##nullableOfNullable;

          switch (Js.toOption(value)) {
          | Some(value) =>
            Some(
              value
              |> Js.Array.map(value =>
                   switch (Js.toOption(value)) {
                   | Some(value) => Some(value)
                   | None => None
                   }
                 ),
            )
          | None => None
          };
        },

        "nullableOfNonNullable": {
          let value = value##nullableOfNonNullable;

          switch (Js.toOption(value)) {
          | Some(value) => Some(value |> Js.Array.map(value => value))
          | None => None
          };
        },
      };
    let name = "ListFragment";
  };
  module Another = {
    let query = "fragment Another on Lists   {\nnullableOfNonNullable  \n}\n";
    type t = {. "nullableOfNonNullable": option(array(string))};
    type t_Lists = t;

    let parse: Raw.t => t =
      (value: Js.Json.t) => {

        "nullableOfNonNullable": {
          let value = value##nullableOfNonNullable;

          switch (Js.toOption(value)) {
          | Some(value) => Some(value |> Js.Array.map(value => value))
          | None => None
          };
        },
      };
    let name = "Another";
  };
};

module MyQuery = {
  module Raw = {
    type t = {
      .
      "l1": Fragments.ListFragment.Raw.t,
      "l2": t_l2,
    }
    and t_l2 = {
      .
      "frag1": Fragments.ListFragment.t_Lists,
      "frag2": Fragments.ListFragment.t_Lists,
    };
  };
  let query =
    (
      (
        (
          (
            (
              ("query   {\nl1: lists  {\n..." ++ Fragments.ListFragment.name)
              ++ "   \n}\n\nl2: lists  {\n..."
            )
            ++ Fragments.ListFragment.name
          )
          ++ "   \n..."
        )
        ++ Fragments.ListFragment.name
      )
      ++ "   \n}\n\n}\n"
    )
    ++ Fragments.ListFragment.query;
  type t = {
    .
    "l1": Fragments.ListFragment.t,
    "l2": t_l2,
  }
  and t_l2 = {
    .
    "frag1": Fragments.ListFragment.t_Lists,
    "frag2": Fragments.ListFragment.t_Lists,
  };
  let parse: Raw.t => t =
    value => {

      "l1": {
        let value = value##l1;

        Fragments.ListFragment.parse(value);
      },

      "l2": {
        let value = value##l2;
        {
          "frag1": Fragments.ListFragment.parse(value),
          "frag2": Fragments.ListFragment.parse(value),
        };
      },
    };
  let makeVar = (~f, ()) => f(Js.Json.null);
  let definition = (parse, query, makeVar);
};
