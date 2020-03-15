open Jest;
open Expect;

type options = {cwd: string};
type buffer;

[@bs.module "path"] external resolve: (string, string) => string = "resolve";

[@bs.module "child_process"]
external execSync: (string, options) => buffer = "execSync";
[@bs.module "fs"]
external readdirSync: string => array(string) = "readdirSync";
[@bs.module "os"] external platform: unit => string = "platform";

[@bs.val] external dirname: string = "__dirname";
[@bs.send] external toString: buffer => string = "toString";

let win = platform() == "win32";
let refmt =
  execSync(
    "esy build echo \"#{@opam/reason.bin / }refmt\"",
    {cwd: resolve(dirname, "../..")},
  )
  |> toString
  |> Js.String.trim;

let run_ppx = (path, opts) => {
  execSync(
    (win ? "type " : "cat ")
    ++ path
    ++ " | "
    ++ refmt
    ++ " --parse re --print binary | ../_build/default/src/bucklescript_bin/bin.exe -schema ../graphql_schema.json "
    ++ opts
    ++ (win ? " - -o -" : " /dev/stdin /dev/stdout")
    ++ " | "
    ++ refmt
    ++ " --parse binary --interface false",
    {cwd: resolve(dirname, "..")},
  )
  |> toString;
};

if (win) {
  execSync(
    "del __tests__\\__snapshots__\\snapshots.bs.js.snap",
    {cwd: resolve(dirname, "..")},
  )
  ->ignore;
};

let tests =
  readdirSync("operations")->Belt.Array.keep(Js.String.endsWith(".re"));

describe("Objects (legacy)", () =>
  if (!win) {
    tests
    |> Array.iter(t => {
         test(t, () =>
           expect(run_ppx("operations/" ++ t, "")) |> toMatchSnapshot
         )
       });
  } else {
    test("dummy", () =>
      expect("dummy") |> toMatch("dummy")
    );
  }
);

describe("Records", () =>
  if (!win) {
    tests
    |> Array.iter(t => {
         test(t, () =>
           expect(run_ppx("operations/" ++ t, "-records")) |> toMatchSnapshot
         )
       });
  } else {
    test("dummy", () =>
      expect("dummy") |> toMatch("dummy")
    );
  }
);
