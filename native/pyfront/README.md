# pyfront -- Python 3 parser in C++

The first piece of CodeBreaker's porting engine in C++: Python source in, an abstract syntax
tree out with the same shape and information as CPython's `ast` module (Python 3.14).
Hand-written: a tokenizer and a recursive-descent parser that follows
CPython's `Grammar/python.gram`. Nothing is downloaded and CPython is not embedded. Python
serves only as the oracle that checks the trees.

    cmake -S native/pyfront -B native/pyfront/build -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build native/pyfront/build
    ctest --test-dir native/pyfront/build

(from a VS 18 x64 developer prompt, `vcvars64.bat`). The build produces:

| target | what |
|---|---|
| `pyfront` (static lib) | the C++ API below; native/l2 and the inference stage link this |
| `pyfront.dll` (`pyfront_c`, import lib `pyfront_dll.lib`) | the C ABI, `include/pyfront/pyfront.h` |
| `pyfront_dump.exe` | dumps trees in the canonical text form |
| `pyfront_tests.exe` | the grammar cases (CTest `pyfront_grammar`) |

## The gate: identical trees

For every `.py` file under `codebreaker/`, `tools/` and `tests/`, `pyfront_dump`'s output
is byte for byte what `ast.dump(ast.parse(src), include_attributes=True)` prints:
**727 / 727 files identical** (10.7 MB, 1.25 M nodes). Parsing them takes
0.65-1.2 s against CPython `ast.parse`'s 2.5-5.1 s, about 4x faster (best of 3, the same
machine and load, reading the files not counted). CPython's own `Lib/` is an extra check
outside the gate: 2,435 / 2,436 identical. The one file that differs holds characters new
in Unicode 16 that Windows' ICU does not know yet (see Unicode below).

The gate script lives in the working folder (`state/work/pyfront/gate.py`, `--speed` for the
timing, `--root DIR` for another corpus). It is the only place Python runs.

## C++ API

```cpp
#include "pyfront/pyfront.hpp"     // parse, dump, SyntaxError (includes ast.hpp)

pyfront::ParseResult r = pyfront::parse(source_bytes, "file.py");
if (!r) { auto& e = *r.error; /* e.msg, e.lineno, e.offset (1-based, CPython's) */ }
const pyfront::Module& m = *r.module;
for (const pyfront::Stmt* s : m.body)
    if (auto* f = pyfront::as<pyfront::ClassDef>(s)) { /* f->name, f->bases, f->body ... */ }
std::string text = pyfront::dump(m);   // == ast.dump(..., include_attributes=True)
```

`include/pyfront/ast.hpp` is the tree, one plain struct per CPython node class, with the same
field names in the same order:

- **Four node families** each carry a `kind` enum. `Expr` (`ExprKind`: BoolOp ... Slice),
  `Stmt` (`StmtKind`: FunctionDef ... Continue), `Pattern` (match patterns) and `TypeParam`
  (PEP 695). Switch on `kind`, then `pyfront::cast<T>(node)`, or test and downcast in one step
  with `pyfront::as<T>(node)`, which gives nullptr on a mismatch. A few node classes share one
  struct and differ only by `kind`: `FunctionDef`/`AsyncFunctionDef` (struct `FunctionDef`),
  `For`/`AsyncFor`, `With`/`AsyncWith` and `Try`/`TryStar`. Cast those after checking `kind`,
  because `as<>` needs a single kind. CPython's statement `ast.Expr` is `ExprStmt`.
- **Operators and contexts are enums**: `OpKind`, `UnaryOpKind`, `BoolOpKind`, `CmpOpKind`,
  `ExprCtx`. `name_of()` gives CPython's class name.
- **Helper nodes**: `arguments`, `arg`, `keyword`, `alias`, `withitem`, `comprehension`,
  `ExceptHandler`, `match_case`.
- **None and empty**: a missing child is `nullptr` and a missing list is empty. An optional
  string such as `alias.asname`, `keyword.arg`, `ImportFrom.module` or `ExceptHandler.name`
  comes with a `has_*` flag. `Dict.keys[i]` is nullptr for `**m`, and
  `arguments.kw_defaults[i]` is nullptr where there is no default.
- **Constants**: `ConstValue` holds None/True/False/Ellipsis, Int (unbounded: the decimal
  digits, plus `i64` when `fits_i64`), Float, Complex (imaginary part), Str (UTF-8) and Bytes.
  `Constant.kind` is `"u"` for a u-prefixed string.
- **Positions**: every node with positions in CPython has `pos` (`lineno`, `col_offset`,
  `end_lineno`, `end_col_offset`). Columns count UTF-8 bytes, as CPython counts them.
- **Ownership**: all nodes live in the `Module`'s arena (`Module::arena`) and die with it.
  Links are raw pointers, so a tree is cheap to walk and to annotate from the outside, for
  example with a side table keyed on `const Expr*`.

The parser is exec mode (`ast.parse(src)`). `type_comments=True`, `feature_version` and the
`eval`/`single`/`func_type` modes are not provided, and the `type_comment` fields stay empty.

## C ABI

`include/pyfront/pyfront.h`: `pyfront_parse(src, len, filename)` -> `pyfront_tree*`;
`pyfront_ok`, `pyfront_dump(t, include_attributes, &len)`, `pyfront_error_msg`,
`pyfront_error_lineno/offset/end_lineno/end_offset`, `pyfront_module(t)` (the
`pyfront::Module*`) and `pyfront_free`.

## Command line

    pyfront_dump FILE...              one dump per file
    pyfront_dump --batch LIST OUT     per file: "== PATH" then the dump or "ERROR msg (line, offset)"
    pyfront_dump --time N LIST        parse every listed file N times, print the best time
    --no-attributes                   leave positions out

## Errors

A file with a syntax error is refused with CPython's line and offset. The 33 error cases in
`tests/cases/errors.cases` all match, for example unclosed brackets, unterminated strings,
missing `:`, bad indentation, invalid targets, f-string errors and bad escapes, and so do
the `badsyntax_*` files in CPython's `Lib/test`, message text included. pyfront reproduces
CPython's messages for the common errors only. It does not reproduce every hint that
CPython's `invalid_*` grammar rules add, so a rarer error can carry a plainer
"invalid syntax" at the same place.

## Unicode and encodings

On Windows the operating system's own ICU (`System32\icu.dll`, present since Windows 10
1903) supplies four things, loaded on first use: character names for `\N{...}`, general
categories for `repr()`'s printable test, XID_Start/XID_Continue for identifiers, and NFKC
for identifier normalization. Legacy `# coding:` sources (Latin-1 and the Windows code
pages: cp125x, koi8-r, Shift-JIS, ...) are converted to UTF-8 first. Without ICU, `\N{...}`
is refused, a built-in printable table is used and identifiers are kept as written.
Windows' ICU predates Unicode 16, so a character assigned in 16.0 counts as unassigned
there.

## Tests

`tests/cases/*.cases`: 301 grammar cases (Python source; not `.py`, so no Python tool trips on the deliberate syntax errors) separated by `#----` lines, covering every expression
and statement form, f-strings and t-strings, match statements, type parameters, numbers,
escapes and errors. `tests/cases/*.expected` holds CPython's dump of each case, or
`ERROR line offset` for a case CPython refuses. The expected files were made once by running
CPython over the cases (`state/work/pyfront/make_expected.py`). After editing a cases file,
regenerate them with that script and review the diff.

## Layout

    include/pyfront/ast.hpp      the tree
    include/pyfront/pyfront.hpp  parse / dump (C++)
    include/pyfront/pyfront.h    the C ABI
    src/lexer.*                  tokenizer: INDENT/DEDENT, implicit joining, PEP 701 f-string modes
    src/parser.cpp               recursive descent, one method per grammar rule
    src/literals.*               string escapes, numbers (big ints), Python repr()
    src/unicode.*                ICU and code pages
    src/dump.cpp                 the canonical text form
    src/capi.cpp                 the C ABI
    tools/pyfront_dump.cpp       the command-line tool
    tests/                       grammar cases and their runner
