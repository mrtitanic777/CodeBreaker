# pyengine -- the porting engine's core in C++

Python source in (parsed by `native/pyfront`), L2 out (the documents `native/l2` reads and writes),
with every function's type found by whole-package inference and every refusal kept with its reason.
It is piece 2 of the porting engine's rewrite in C++ (docs/NATIVE.md, "The porting engine in C++").
The C++17 back end (L2 -> C++ over `native/ported/include/py.hpp`) is native/l2's `l2cpp17`, linked
in: with the extensions on, **a function counts as translated only when the back end writes it**,
and the judge (below) holds every translated function to compiling and behaving as the Python.

    (vcvars64)  cmake -S native/pyengine -B native/pyengine/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/pyengine/build
                ctest --test-dir native/pyengine/build

    pyengine <package dir> [--coverage] [--out DIR] [--cpp DIR] [--checked] [--old] [--package NAME]
             [--module M] [--hints DIR] [--no-backend] [--log]

| target | what |
|---|---|
| `pyengine` (static lib) | the C++ API: `Program`, `Eval`, `translate`, `run_engine` (src/*.hpp) |
| `pyengine_api.dll` | the C ABI, `include/pyengine/pyengine.h` |
| `pyengine.exe` | the command line: L2 per module plus the coverage ledger |
| `pyengine_tests.exe` | the CTest suites |

`--out DIR` writes `DIR/l2/<module>.json` (each module's L2 `Unit`), `DIR/coverage.json` (per
function: translated or refused, the reason, its lines, its L2) and `DIR/coverage.md` (the table by
package and the top refusal reasons). `--old` runs the old engine's rules exactly (no extensions);
`--checked` is the checked int policy (ints past int64 raise OverflowError in C++). `--cpp DIR`
writes every module through the C++17 back end (`DIR/<module path>.hpp/.cpp`, `DIR/spans.json`: each
function's source lines), modules with nothing translated too (others include their headers).
`--hints DIR` (default `native/pyengine/hints`) is where the hints are read from. The whole of
codebreaker/ takes about 50 s extended, 25 s with `--old` (the Python engine: 115-130 s).

## Translated => compiles => agrees

**The back end pass** (engine.cc, extended runs): every unit is written by `cpp17::write_unit`; a
function the back end declines (`cpp17::Decline::where` names it) is refused as `backend: <why>`,
taken out of its unit and the unit written again; a translated function calling a refused one is
refused in turn (`calls-refused`), to a fixpoint. A module name whose value reads a refused name or
calls a refused function is not written either, nor are its readers (lower.cc).

**The judge** (`state/work/pyengine2/judge.py`, the oracle side, not committed): compiles every module
`--cpp` wrote with MSVC (`/std:c++17`, py.hpp and native/l2's pyre on the include path; errors pinned
on functions by spans.json), links one driver over all of them, and runs each module-level function on
generated inputs (its parameters' types; int parameters within the ranges the engine says its callers
give, `ranges` in coverage.json; the string and number literals of its own body) beside the Python
original, comparing results and exception names. A run that hangs or crashes is a case's verdict and
the run restarts after it. Functions that write files, sleep or touch the environment, methods (no
instance to call them on) and functions returning objects are compiled but not run.

**hints/refuse.json, refuse.<policy>.json**: what the judge found does not compile or does not agree,
with the reason; the engine refuses each (`judge: <why>`) with its cascade. A recurring entry is a bug
to fix in the engine, the back end or py.hpp (most of this work was that: see below), the file is
what is left.

**hints/types.json**: `{function: {parameter: annotation text}}`, applied when the classic rules
have settled, to parameters they left untyped, as if the source were annotated (Python files are not
edited). A strict `int` hint is unbounded, so it is refused as int-range unless the code bounds it.

The numbers are in Gate 2 below.

What it took (each found by the judge): the back end had no C++ for any of the extensions' rows
(re, open/files, os/os.path/shutil/time, hashlib, variants, generators, closures with statements) --
now `native/ported/include/py_os.hpp`; class types lost their record/ref/exception flags in the
engine's L2 JSON (`.` for `->`, records built as values where refs were declared); `1 << 32` with a
C++ int literal; `d.get(k)` returning a default-constructed value instead of None; string literals
holding NUL cut short; `os.path.abspath` via str.join (ADL); `environ` a CRT macro; record keys that
are not identifiers; CRT macros as names (`_MAX_DIR`); str.isalpha/isalnum outside Latin-1 (now
Unicode's tables, py_unicode.hpp); max/min of a literal and an int64_t; `int64_t / double`; tuple
iteration; `list(a_tuple)`; `split(sep, maxsplit)`; exceptions' Python names; Windows file-name
rules (`<`, `:`, NUL -> OSError / ValueError as Python's); trailing spaces/dots in abspath.

## Design

**1. Scopes** (`scopes.hpp`): names to bindings by CPython's symtable rules -- module, class,
function, lambda and comprehension scopes; `global` / `nonlocal`; a walrus binds in the enclosing
function; class bodies are skipped by the functions inside them. A name used but not bound is free
(the nearest enclosing function binding it) or global or builtin; a function's names an inner scope
reads are its cells. `astutil.hpp` adds what pyfront leaves to its first consumer: the generic child
walker in CPython's field order, `ast.walk`'s breadth-first order, a parent map, docstrings
(`inspect.cleandoc`), source segments, `unparse` of names.

**2. The program** (`program.hpp`): every module of the package, its functions, classes, module
names and imports resolved across the package (to its own modules, the standard library or outside
libraries), and the facts inference gathers: parameter types, returns, attributes, module names, and
containers -- every list/dict/set display is a site, sites that flow together are one object
(union-find), a dict used only with constant string keys is a record.

**3. Inference** (`infer.hpp`, `methods.cc`): an abstract interpreter over types, rule for rule the
old front end's (codebreaker/translate/frontends/python): branch narrowing (`is None`, truth,
comparisons), loops to a fixpoint with widening over thresholds then narrowing, integer intervals on
unbounded ints (`big.hpp`) proving int64 fit, the counter rule, records, contracts at calls (a value
that may be None or leave int64 is refused at the caller), the whole package run round after round
until no fact changes.

**4. Contracts, then the extensions.** The classic rules run first to their fixpoint. Their facts
are then *frozen* as contracts, and the extensions' second phase runs: code typed in it that would
change a frozen fact (widen a parameter, add an item kind to a container, mutate a record kept as a
value) is refused at the statement that does it -- blame at the caller. This is what keeps every
function the old engine translated byte-identical. Facts the classic rules left with no single type
(read by no translated function) are recomputed. The extensions:

| extension | what it carries |
|---|---|
| demand typing | a parameter no caller types takes the type its uses demand: the typed parameter it is passed to, the one class hierarchy having all the attributes/methods it uses, str-only operations (`open`, `os.path.*`, `p.startswith('x')`, `p == 'x'` ...), `range(p)` |
| per-caller specialization | a function whose callers pass kinds that do not join is copied per argument signature; each copy is its own `MethodDecl` with the original name (C++ overloads) |
| closures | a nested `def` is a `Local` of type `py.fn<R, P...>` initialised by a capturing `Lambda`; its free names take the join of every value the definer binds to them and are declared before it; a closure used as a value (returned, stored) is refused |
| generators | a generator function makes a class deriving `py.generator<T>` (`ClassDecl` mods `generator`): parameters and locals are fields, `next()` returns `py.optional<T>` and resumes after each yield (`Goto` / `Labeled` on `state`); a loop a resume enters becomes an index loop over fields. Its call is `py.iter<T>`, consumed by `for` / `list()` like any iterator. `yield from`, yields in `try`, generator methods: refused |
| tagged variants | where scalars of different kinds meet (int, str, float, bool, bytes), the value is `py.variant<A, B ...>`; its alternatives are its tags. `isinstance` narrows it (`variant.is`, typeargs the tags), a narrowed use reads one (`variant.get`, typeargs `["int"]`), a value enters as `variant.of`; any other use is refused (never an "any" type) |
| Optional | where a value may be None it is `py.optional<T>`, narrowed by `is None` / truth tests, as before; extended to `os.environ.get`, `re` matches and optional regex groups |
| `re` | `re.match/search/fullmatch/sub/subn/split/findall/finditer/escape/compile`, Pattern and Match methods, `m[g]`. The pattern is read for its groups: a group inside `?`, `*`, `{0,}`, an alternative, a negative look-around or a conditional may be unset, so `m.group(g)` is `Optional[str]` there and `str` elsewhere; `findall` gives `list[str]` or `list[tuple]`. Every call carries its pattern (`typeargs`) for the back end's Python-compatible regex engine (native/l2's pyre) |
| files and os | `open()` in every text and binary mode, `read/readline/readlines/write/writelines/seek/tell/close`, iterating a text file's lines, `with open(...)`; `os.makedirs`, `os.path.abspath/realpath/normpath/expanduser/relpath/isabs/getmtime`, `os.getcwd`, `os.remove/unlink/rmdir/rename/replace`, `os.environ.get`, `os.environ[k]`, `os.getenv`, `os.walk`, `shutil.copyfile/copy/copy2/move/rmtree/which`, `time.time/perf_counter/monotonic/sleep`, `hashlib` digests |
| decorators | `native_first`, `lru_cache`, `cache` are transparent (the function's own body is the behaviour) |

**5. Lowering** (`lower.hpp`, `ir.hpp`): each typed function into an L2 `MethodDecl`, each module
into a `Unit` (records, classes with their attributes, the module's functions and names), the same
shapes the Python front end produced, written exactly as `l2.to_json` writes them. A refused
function stays a `Comment` with its reason, and the ledger keeps it.

### What the back end gets beyond the old front end's L2

Calls owned by `py` (`Sym('function', name, 'py')`): `open` (path, mode, `kw` encoding/errors/
newline), `file.read/readline/readlines/write/writelines/seek/tell/close/flush`, `re.<fn>` and
`re.Pattern.<fn>` / `re.Match.group/groups/start/end/span` (typeargs: the pattern), `variant.of/is/get`
(typeargs: the tags), `some` (a generator's next value), `os.*`, `os.path.*`, `shutil.*`, `time.*`,
`hashlib.*`, `hash.hexdigest/digest/update`, and keyword arguments of the mapped rows as
`kw` calls (typeargs: the name). Types: `py.file`, `py.re.Pattern`, `py.re.Match`,
`py.hashlib.Hash`, `py.fn<R, P...>`, `py.variant<A, B ...>`, `py.generator<T>` (a base class with
`virtual py.optional<T> next()`), `py.iter<T>`. Nodes: `Lambda` with a `Block` body (closures, mods
`closure` on the `Local`), `Labeled`, `Goto`, `Empty` (generators); a call of a closure is a `Call`
whose `Sym` is the `local` that holds it.

## Gate 1: the old engine's functions

On all of codebreaker/ (563 modules, 8,030 functions at this commit), against the Python engine's
own results (`state/work/pyengine2/baseline.py`, compared by `gate1.py`). The Python front end's L2
now carries class types' `record` / `ref` / `exception` flags (l2.to_json), and so does this
engine's (it had dropped them: the back end then wrote `.` for `->`).

| run | the old engine translated | L2 byte-identical | refused now | refusal reasons the same |
|---|---|---|---|---|
| `--old` strict | 274 | **274** | 0 | 7,756 / 7,756 |
| extended strict | 274 | **264** | 10: the judge found they do not compile (8) or differ (1), or the back end declines (1) | -- |
| extended checked | 356 | **339** | 17: likewise | -- |

The ten (seventeen) were never compiled before; the old pipeline's own `coverage --compile` had
found 16 of its 274 did not compile. Identical L2 for everything else translated.

## Gate 2: translated, compiled, run

| policy | translated | lines | compile | run beside the Python | agree | compiled, not run |
|---|---|---|---|---|---|---|
| strict | 472 | 3,109 | 472 | 287 | **287** | 185 (methods, objects, no input generator, side effects) |
| checked | 582 | -- | 581 | 332 | **332** | 249 |

(checked: measured one cycle earlier, before the last runtime fixes.) By package (strict):
(5.7%), build 40, decompile 34, translate 31, exact 22 -- 2.1% of CodeBreaker's lines, 1.5% run and
agreeing. The README's earlier 517 / 656 counted functions nothing had compiled: 47 were declined by
the back end, the rest were held to compile and agree here.

Top refusals now (strict, first reason): untyped parameter with no typed call site 1,172 (functions
nothing in the package calls: entry points, getattr-dispatched visitors, test-only helpers), callers
passing values of no type 324, calls-refused 281, getattr 143 (was 164: constant names now carried),
dicts of mixed values 143, int-range of an argument 139, attributes stored values of no type 139,
`id` 93, `next` 91, `with` other than open 81, `*args` 82, lambdas as values 73.

**Honestly**: the leaf packages (coverage, jobs, store, knowledge) are glue -- sqlite3, threads and
locks, contextvars, process pools, csv, json, argparse, `with` / `finally` / `except as`, lambdas as
values. Hints do not move them: of eight parameter hints written for knowledge's facade, two
functions translated (`_bare`, `reads.join`); the rest call functions refused for library reasons.
The engine carries computation (format readers, decoders, text helpers); glue wants writing by hand.

## Tests

`ctest --test-dir native/pyengine/build` -- 10 suites, 87 checks, each running the whole engine on
Python modules held in `tests/pyengine_tests.cpp`: `scopes` (17: bindings, cells and free names,
class-scope skipping, comprehensions, walrus, global), `inference` (11: caller types, intervals,
records, narrowing, int-range, old = new rules, checked policy, demand typing), `specialization`
(7), `closures` (6), `generators` (7), `variants` (6), `regex` (11: group analysis, optional groups,
patterns carried, compiled patterns, findall), `files` (5), `dynamic` (10: getattr with a constant
name -- the attribute, the default, refused when it may or may not be there; a type hint; a module
name whose value calls a refused function), `backend` (7: re, os.path, hashlib, open with keywords in
their slots, text reads, written by l2cpp17).

## C ABI

`include/pyengine/pyengine.h`: `pyengine_run_package(root, package, policy, extended, out_dir)`,
`pyengine_ok`, `pyengine_error`, `pyengine_ledger_json`, `pyengine_unit_json(r, module)`,
`pyengine_coverage_md`, `pyengine_counts`, `pyengine_free`.

## Layout

    src/big.*        Python's unbounded ints (interval bounds)
    src/pytypes.*    the inference's types, intervals, widening
    src/astutil.*    child walker, ast.walk, parent map, docstrings, segments, unparse
    src/scopes.*     the scope pass
    src/program.*    modules, symbols, imports, containers, the join, contracts, nested functions
    src/infer.*      the abstract interpreter and the solver (two phases)
    src/methods.cc   builtins, builtin types' methods, struct formats, stdlib rows
    src/demand.cc    demand typing
    src/ext.cc       re (with the group analysis), files, os/shutil/time/hashlib
    src/ir.*         L2 nodes and their JSON
    src/lower.*      lowering, closures, generators, variants, units, the ledger
    src/engine.*     the whole run; the back end pass; hints; coverage tables
    src/capi.cc      the C ABI
    tools/           pyengine.exe
    tests/           the CTest suites
    hints/           types.json (parameter hints), refuse.json / refuse.<policy>.json (the judge's refusals)

The gate scripts are in the working folder (`state/work/pyengine2/`: baseline.py, gate1.py,
judge.py, why.py, roots.py, merge_refuse.py, gen_unicode.py); Python runs there only as the oracle.

## What is left

- Methods are compiled but not run by the judge (139 strict): it has no instance to call them on.
  Recorded inputs (arguments captured while the Python tests run) would judge them and the rest.
- The recurring compile failures behind hints/refuse.json: a class of another module named in a
  header before its include; `py::list<bool>` (std::vector<bool>'s reference); a caller passing a
  bool where a str is declared; a module name rebound by a later import; module state filled at
  import by registration calls.
- Library rows the glue packages need before the engine carries them at all: sqlite3, threading,
  contextvars, subprocess, csv, json; `with` on other context managers, `finally`, `except ... as e`.
- A generator consumed by `next()` / `zip()`, generator methods, `yield from`; closures that escape
  (they would need captures by value or shared cells); `nonlocal` rebinding.
- Variants of containers and instances (only scalars now); records whose fields are variants
  are carried, dicts whose values are mixed beyond scalars are not.
- The largest remaining roots: getattr dispatch, Python `ast` visitors, `*args`, `id`, `next`,
  `with` on other context managers, `except ... as e`, module-level `global` state.
