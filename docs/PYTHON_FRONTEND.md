# The Python front end: Python into L2, and from there C++

`codebreaker/translate/frontends/python/` turns Python modules into L2 (the typed tree every
language shares, `codebreaker/translate/ir.py`), and `codebreaker/translate/backends/cpp17.py`
writes that tree as C++17 over one support header, `native/ported/include/py.hpp`. It exists to
carry the mechanical part of moving CodeBreaker's own code to C++ (docs/NATIVE.md), and it is held
to the same rule as everything CodeBreaker makes: readable source that behaves as the original,
proven by running both.

    python tools/py2cpp.py port codebreaker.formats.ne native/ported/ne/src     one module
    python tools/py2cpp.py coverage [--compile] [--checked]                     all of codebreaker/

## How types are found

Almost none of CodeBreaker's functions are annotated (499 of 7,438), so the types come from
inference over the whole package at once (`program.py`, `infer.py`):

- **An abstract interpreter.** Each function is run on types instead of values, flow by flow. A
  branch's test narrows what it tests (`x is None`, `isinstance`, `p < len(data)`); a loop is run
  to a fixpoint. What one function learns about others -- the arguments it passes, what it stores
  in attributes and containers, what it returns -- is a program fact, and the program is run round
  after round (only the functions that read a changed fact) until no fact changes. The whole
  package settles in about 18 rounds, 1.5-3.5 minutes.
- **Parameters** come from annotations where there are any, else from every call site in the
  package, joined, plus their defaults. A parameter nothing calls with a typed value is untyped.
- **Attributes** are the join of every store to them, in any method or from outside; a class-level
  value is the attribute's default.
- **Containers are objects.** Every list, dict or set display is a *site*; sites that flow
  together are one object (union-find), whose element types grow as items are added anywhere. A
  dict site used only with constant string keys is a **record** -- a C++ struct with a field per
  key, required where every creation sets it, `std::optional` where not. Any other use (a computed
  key, iteration, `len`, `.items()`) makes it a `py::dict`.
- **Integers have intervals.** C++'s int is `int64_t`, so every Python int must be shown to fit.
  Intervals come from literals, `struct` formats, byte indexing, masks, `len` (no object holds more
  than 2**47 items: the Windows x64 address space), and are carried through arithmetic. A loop is
  widened over thresholds and narrowed back; a successful index bounds what indexed it (`data[p]`
  returned, so `p` is inside `data`); a counter a loop only increases by bounded steps, in a loop
  of bounded passes, is bounded by its start plus steps times passes.
- **Contracts at calls.** A value with no single type is the caller's problem, not the callee's: it
  is not passed on. An argument that may leave int64, or may be None where the callee never tests
  for None, is refused *at the call*, where it comes from.

## What is refused, and why

A function the front end cannot type stays Python, with its reason. Never "any" or a variant: a
function either becomes C++ that reads as written by hand, or it is left alone. The classes:

| class | meaning |
|---|---|
| untyped | a parameter no annotation or call site gives a type, or an empty container nothing fills |
| conflict | two kinds of value meet in one place (`x = 1 if c else "one"`, a list of mixed items) |
| int-range | an integer not shown to stay inside int64 (`sector << shift` with `shift` read from a file) |
| dynamic | getattr/setattr, `*args`/`**kwargs`, generators, decorators, set iteration order, first-class functions, `isinstance` on a run-time class |
| stdlib | a standard library call not mapped yet (`re.*`, `json.*`, `subprocess.*`, `os.walk` ...) |
| extern | a library outside the package and the standard library (unicorn, capstone ...) |
| syntax | a construct not lowered yet (`with` other than `open`, `try/finally`, nested functions, `del`, walrus) |
| calls-refused | it calls a function that is refused, or reads a module name that is |

**int policy.** `strict` (the default) refuses an int it cannot bound. `checked` writes the same
code with overflow-checked operations (`py::checked_add`, `py::shl`) where a result is not shown to
fit: it behaves as Python wherever the value fits int64 and raises `OverflowError` where Python
would have gone on with a big integer. The coverage report gives both.

## What the C++ looks like

Python's model is kept where a program would notice it: lists, dicts, sets, class instances and
records changed after they are built are references (`py::list`, `py::dict`, `py::ref<T>` --
Python's aliasing); str, bytes, tuples, numbers and records never changed after they are built are
values. Integer operations are C++'s own where the intervals prove they agree with Python's
(`p += 2`, `a / b` of non-negatives) and the header's where not (`py::floordiv`, `py::mod`,
`py::shr` for negative operands). A local is declared where it is first assigned when that point
holds all its uses. A dict built key by key (`h = {}; h["a"] = ...`) becomes the struct, declared,
then filled field by field -- the same statements.

The support header (`py.hpp`) holds the semantics that bite: arbitrary-precision ints are not
emulated (the front end proves the range instead); `py::str` is code points (len and slicing count
characters); `py::bytes` indexes to ints, slices clamp, negatives count from the end, `decode`
knows latin1/ascii/utf-8/cp437/cp1252 with strict/replace/ignore; `py::list` iterates by index over
the live size (appending while iterating is Python's behaviour, not undefined); `py::dict` keeps
insertion order and `at` raises `KeyError`; `struct` formats are read at run time with standard
sizes; floor division and modulo take the divisor's sign; `repr` and `format` follow Python's rules.
Library calls are rows of the API map, `codebreaker/java/api_maps/python.json` (target `cpp`, a
template over the header).

## Proof

`native/ported/ne/` is `codebreaker/formats/ne.py` translated, built with MSVC, and gated:
`python native/ported/ne/gate.py` runs every MZ file of the corpus through both and compares
every fact (docs/NATIVE.md has the table). `tests/translate/test_python_frontend.py` translates a
module of the semantics above, compiles it, runs it and compares each result with Python's, and
checks each refusal class.
