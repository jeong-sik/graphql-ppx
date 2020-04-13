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
    module Raw = {
      type t = {
        .
        "nullableOfNullable": Js.Nullable.t(array(Js.Nullable.t(string))),
        "nullableOfNonNullable": Js.Nullable.t(array(string)),
      };
      type t_Lists = t;
    };
    type t = {
      .
      "nullableOfNullable": option(array(option(string))),
      "nullableOfNonNullable": option(array(string)),
    };
    type t_Lists = t;

    let parse = (value: Raw.t) => {

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
    module Raw = {
      type t = {. "nullableOfNonNullable": Js.Nullable.t(array(string))};
      type t_Lists = t;
    };
    type t = {. "nullableOfNonNullable": option(array(string))};
    type t_Lists = t;

    let parse = (value: Raw.t) => {

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
    and t_l2 = Js.t({.});
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

          "frag1": {
            let value: Fragments.ListFragment.Raw.t = Obj.magic(value);

            Fragments.ListFragment.parse(value);
          },

          "frag2": {
            let value: Fragments.ListFragment.Raw.t = Obj.magic(value);

            Fragments.ListFragment.parse(value);
          },
        };
      },
    };
  let makeVar = (~f, ()) => f(Js.Json.null);
  let make =
    makeVar(~f=variables =>
      {"query": query, "variables": variables, "parse": parse}
    );
  let makeWithVariables = variables => {
    "query": query,
    "variables": serializeVariables(variables),
    "parse": parse,
  };
  let definition = (parse, query, makeVar);
};