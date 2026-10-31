# pyengine -- the porting engine's core in C++

Python source in (parsed by `native/pyfront`), L2 out (the documents `native/l2` reads and writes),
with every function's type found by whole-package inference and every refusal kept with its reason.
It is piece 2 of the porting engine's rewrite in C++ (docs/NATIVE.md, "The porting engine in C++").
The C++17 back end (L2 -> C++ over `native/ported/include/py.hpp`) is a separate piece and is not
written here.

    (vcvars64)  cmake -S native/pyengine -B native/pyengine/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/pyengine/build
                ctest --test-dir native/pyengine/build

    pyengine <package dir> [--coverage] [--out DIR] [--checked] [--old] [--package NAME] [--module M] [--log]

| target | what |
|---|---|
| `pyengine` (static lib) | the C++ API: `Program`, `Eval`, `translate`, `run_engine` (src/*.hpp) |
| `pyengine_api.dll` | the C ABI, `include/pyengine/pyengine.h` |
| `pyengine.exe` | the command line: L2 per module plus the coverage ledger |
| `pyengine_tests.exe` | the CTest suites |

`--out DIR` writes `DIR/l2/<module>.json` (each module's L2 `Unit`), `DIR/coverage.json` (per
function: translated or refused, the reason, its lines, its L2) and `DIR/coverage.md` (the table by
package and the top refusal reasons). `--old` runs the old engine's rules exactly (no extensions);
`--checked` is the checked int policy (ints past int64 raise OverflowError in C++). The whole of
codebreaker/ takes about 50 s extended, 25 s with `--old` (the Python engine: 115-130 s).

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

## Gate 1: equals or beats the old engine

On all of codebreaker/ (564 modules, 8,158 functions), against the Python engine's own results
(`state/work/pyengine/baseline.py`, compared by `gate1.py`):

| run | the old engine translated | L2 byte-identical | refused now | refusal reasons the same | module units identical |
|---|---|---|---|---|---|
| `--old` strict | 277 | **277** | 0 | 7,881 / 7,881 | 564 / 564 |
| `--old --checked` | 359 | **359** | 0 | 7,799 / 7,799 | -- |
| extended strict | 277 | **277** | 0 | (240 more translated) | -- |
| extended checked | 359 | **359** | 0 | (297 more translated) | -- |

## Gate 2: coverage

| package | functions | lines | old strict | new strict | % lines | old checked | new checked | % lines |
|---|---|---|---|---|---|---|---|---|
| translate | 1756 | 25777 | 29 | 47 | 0.5% -> 0.9% | 33 | 58 | 0.5% -> 1.1% |
| formats | 1351 | 18426 | 100 | 152 | 3.7% -> 6.0% | 160 | 221 | 6.0% -> 9.3% |
| exact | 535 | 13729 | 15 | 24 | 0.4% -> 0.9% | 15 | 28 | 0.4% -> 1.3% |
| java | 572 | 10912 | 34 | 92 | 1.5% -> 7.0% | 36 | 97 | 1.7% -> 7.5% |
| source | 484 | 10534 | 7 | 12 | 0.3% -> 0.6% | 8 | 15 | 0.6% -> 1.2% |
| port | 270 | 9483 | 2 | 13 | 0.2% -> 1.1% | 2 | 17 | 0.2% -> 1.8% |
| build | 385 | 8637 | 25 | 42 | 1.4% -> 2.9% | 28 | 51 | 1.6% -> 3.9% |
| invert | 285 | 7930 | 12 | 15 | 1.1% -> 1.3% | 13 | 19 | 1.5% -> 1.9% |
| decompile | 333 | 7363 | 15 | 36 | 1.2% -> 3.7% | 17 | 41 | 1.3% -> 4.2% |
| verify | 322 | 6553 | 2 | 12 | 0.1% -> 0.9% | 2 | 12 | 0.1% -> 0.9% |
| emulate | 368 | 6439 | 10 | 13 | 0.7% -> 0.8% | 14 | 17 | 0.8% -> 1.0% |
| recompile | 171 | 5604 | 6 | 13 | 2.6% -> 3.1% | 7 | 20 | 2.7% -> 3.7% |
| analyze | 208 | 4419 | 2 | 6 | 0.1% -> 0.6% | 2 | 8 | 0.1% -> 0.8% |
| symbols | 182 | 3176 | 6 | 8 | 1.2% -> 2.2% | 9 | 15 | 1.7% -> 4.0% |
| others (ui, engine, base, xna, compare, store, coverage, knowledge, platforms, jobs, __main__) | 936 | 9730 | 12 | 32 | -- | 13 | 37 | -- |
| **all** | **8158** | **148712** | **277** | **517** | **1.1% -> 2.4%** | **359** | **656** | **1.5% -> 3.2%** |

Top refusal reasons now (strict; first reason per function): untyped parameter with no typed call
site and no demand 1,205 (was 1,575: many are visitor methods over Python's own `ast` nodes, an
outside type), callers passing values of no type 333, calls-refused 287, getattr 164, dicts given
items of no single type 145, int-range of an argument 140, attributes stored values of no type 140,
untyped locals 123, `id` 93, `next` 91, containers other code keeps empty 89 (the contracts at work),
lambdas/generators as arguments 85, `*args` 82, `with` other than open 81. By class: untyped 2,264,
conflict 1,843, dynamic 1,510, stdlib 512 (was 1,435), syntax 476 (was 620), int-range 466,
calls-refused 287. Fixing a root frees its cascade: the extensions moved the first refusal of about
2,700 functions to a later one.

**Honestly**: the engine now carries 2.4% of CodeBreaker's lines strict (3.2% checked), twice the
old engine, and its refusal list is a much more exact to-do list. It does not carry the bulk, for
the reasons docs/NATIVE.md gives: almost nothing is annotated, and the dynamic core (getattr
dispatch, `ast`-node visitors, argparse namespaces, dicts used as objects of several kinds) has no
static type to find.

## Tests

`ctest --test-dir native/pyengine/build` -- 8 suites, 70 checks, each running the whole engine on
Python modules held in `tests/pyengine_tests.cpp`: `scopes` (17: bindings, cells and free names,
class-scope skipping, comprehensions, walrus, global), `inference` (11: caller types, intervals,
records, narrowing, int-range, old = new rules, checked policy, demand typing), `specialization`
(7), `closures` (6), `generators` (7), `variants` (6), `regex` (11: group analysis, optional groups,
patterns carried, compiled patterns, findall), `files` (5).

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
    src/engine.*     the whole run; coverage tables
    src/capi.cc      the C ABI
    tools/           pyengine.exe
    tests/           the CTest suites

The gate scripts are in the working folder (`state/work/pyengine/`: baseline.py, gate1.py,
table.py, why.py); Python runs there only as the oracle.

## What is left

- The C++17 back end over this L2 (another piece), then compiling and running what is translated
  against the Python, as `native/ported/ne` was.
- A generator consumed by `next()` / `zip()`, generator methods, `yield from`; closures that escape
  (they would need captures by value or shared cells); `nonlocal` rebinding.
- Variants of containers and instances (only scalars now); records whose fields are variants
  are carried, dicts whose values are mixed beyond scalars are not.
- The largest remaining roots: getattr dispatch, Python `ast` visitors, `*args`, `id`, `next`,
  `with` on other context managers, `except ... as e`, module-level `global` state.
