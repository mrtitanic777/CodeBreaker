# pyengine -- handoff (2026-11-04)

The porting engine is now a side project (direct ports come first). Where it stands:

## Counts (all of codebreaker/, 563 modules, 8,030 functions)

| policy | translated | compiles | run beside the Python | agrees | disagrees | not judged |
|---|---|---|---|---|---|---|
| strict | 472 (3,109 lines, 2.1%) | 472 | 287 | 287 | 0 | 185 |
| checked (one cycle earlier) | 582 (2.8%) | 581 | 332 | 332 | 0 | 249 |

"Not judged" = compiled but not run: methods (no instance), functions returning objects, parameters
the judge has no generator for, side effects (file writes, rmtree, sleep).

**Gate 1** (the old engine's functions, against `baseline.py`): `--old` strict 274/274 L2
byte-identical, 7,756/7,756 refusal reasons the same. Extended: strict 264/274 identical, 10
refused because the judge found they do not compile or agree (they never compiled before either);
checked 339/356, 17 refused the same way.

## The check loop (state/work/pyengine2: cycle.sh, judge.py, cachegate.sh)

- Engine run with `--cpp`: ~45-60 s.
- Judge: precompiled py.hpp/py_os/py_json/py_unicode (`/Yc` once per runtime hash, `/Yu` + `/FI`),
  module objects cached on their text + transitive generated headers + runtime + flags, verdicts
  cached per function on its own C++ span, the spans of every function it calls (transitively),
  the non-function part of their modules (names' values, records, classes; prototypes and comments
  left out), their Python sources, its cases, the runtime, flags and the judge itself. Cache in
  `state/work/pyengine2/cache/`; no timestamps anywhere. 2 compile workers.
- Gate (s11, same engine output): no-cache 1,356 s, cold 280 s, warm 6 s -- identical counts all
  three (472 / 472 / 287 / 287 / 0 / 185). Warm reuse 285 of 287 verdicts (99%).

## Recorded runtime types as hints (2026-11-05): `--stubs DIR`, hints/stubs/

MonkeyType 23.3.0 traced every suite tests/run_all.py runs (slow ones too, each capped at 4 minutes:
the ones that did not finish lose all their traces, since MonkeyType writes at exit -- test_mcp,
test_service, test_symbols, test_analysis, test_drivers, test_powerpc, test_whole_module, test_invert,
test_readable, test_frozen_slow timed out) plus `info/names/identify/analyze/decompile` over lz32.dll,
attrib.exe, version.dll, fc.exe, into one database; 205 codebreaker modules seen, **191 stubs**
(`hints/stubs/*.pyi`, MonkeyType's output untouched; 13 more failed in MonkeyType's own renderer).
pyengine reads them (`--stubs DIR`, C++ only: `Program::load_stub` / `apply_stub_hints`), applied
after types.json to parameters nothing else types; inference wins, a differing stub is a conflict
(reported in `stubs_report.json` under `--out`); a type the engine cannot model is ignored with its
reason. Return types are not used. Reproduce: `pyengine codebreaker --coverage --stubs
native/pyengine/hints/stubs --out DIR`.

MonkeyType on Python 3.14 needed more local patches in site-packages (typing.py: `rewrite` and
`_rewrite_container` for classes with an `__args__` slot, `get_type` for `Type[Generic]`; stubs.py:
`_is_optional`, `get_imports_for_annotation`); it still writes `Union` as `<class 'Union'>[...]`,
which the reader accepts.

| policy | before: translated (lines) | after, with stubs | gained / lost | conflicts |
|---|---|---|---|---|
| strict | 466 (3,048 lines, 2.2%) | **491 (3,225, 2.3%)** | +26 / -1 | 485 (stub differs, inference wins) |
| checked | 574 (3,957 lines, 2.8%) | **612 (4,198, 3.0%)** | +41 / -3 | same |

Stub parameters: 1,270 applied (675 functions), 713 agree with inference, 485 conflict, 276 ignored
(150 types the engine cannot model, 76 annotated in the source, the rest no single type). The
untyped class barely moves: the traced suites call mostly the same functions the engine already
types, and most untyped-parameter functions are never called by a quick test.
**Gate 1** (HEAD engine rebuilt from git, `--old`): coverage.json (every function's status and L2)
byte-identical with and without this change, in `--old`, strict and checked modes without stubs.
The tree now has 7,664 functions (modules deleted since the 2026-11-04 counts), so the older
totals above (472 / 582) are not comparable: HEAD itself gives 466 strict / 574 checked today.
Not re-run: the judge (state/work/pyengine2 no longer exists), so compiles / agrees / disagrees for
the 25 + 38 newly translated functions are unknown -- rebuild the judge from this file's description
first. Next: trace the timed-out suites in smaller pieces (or with MonkeyType's `--limit`-style
sampling), trace real commands over more corpus samples, then use the recorded *return* types too.

## Open refusal classes, biggest first (strict, first reason per function)

untyped 2,202 (parameters nothing in the package calls: 1,172; callers passing values of no type:
324; mixed-value dicts 143; lists 90), conflict 1,821, dynamic 1,485 (getattr 143, id 93, call 91,
next 91, lambdas as arguments 82, *args 82, first-class lambdas 73, set order 54), stdlib 511
(sqlite3, json, csv, subprocess, threading, collections.*), syntax 469 (with on other context
managers 81, finally, except-as 49), int-range 465, calls-refused 283, unresolved 197, judge 33
(hints/refuse.json), backend 8.

## Next steps

1. Batch the judge's compiles: a few large translation units (unity builds of ~20 modules each)
   instead of one object per module; the cold run is dominated by ~270 separate cl starts.
2. Keys are already narrowed to the function and its callees; the module cache could narrow the
   same way once compiles are batched.
3. Step 2's one-package test: drive one package to 100% with hints. Measured so far: the leaf
   packages (coverage, jobs, store, knowledge) are glue over sqlite3 / threads / contextvars /
   process pools / csv / json; 8 parameter hints on knowledge's facade freed 2 functions. A package
   of computation (a format reader, a decoder) is the right test.
4. The recurring causes behind hints/refuse.json (README "What is left").
