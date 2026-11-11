# CodeBreaker in C++: the design every refactor agent follows

Design note, 2026-11-11. Logan's rule: CodeBreaker is a complete C++ rewrite, "as if the Python was never
written" -- idiomatic C++, no code that mimics Python. About 365 modules (742 files, 168k lines) were
machine-assisted-ported to typed C++ under `native/typed/cpp/codebreaker/` on top of an imitation of Python
(`native/ported/include/py*.hpp`). Logan chose to **refactor those ports into idiomatic C++**: keep the
algorithms, drop the imitation layer, and judge correctness by what CodeBreaker does -- the decompile, build and
judge results, the files it writes -- not by matching the Python's incidental output.

This document is the contract. Where it says MUST, a reviewer rejects the module without it. Decisions that need
Logan are collected in section 9; everything else is decided here, one choice per question.

## 1. What the survey found

**The imitation layer** (`py.hpp` and friends, 5.4k lines) gives: `py::str` (UTF-32, length and indexing in code
points), `py::bytes`/`bytearray` (negative indices, clamped slices, an item is an int), `py::list`/`dict`/`set`
as *shared handles* (Python aliasing; iteration by live index; `dict` keeps insertion order), Python integer
semantics in `int64_t` (`floordiv`, `mod`, `shl`, `checked_*`), `%`-formatting and `format()`, `repr`, Python's
exception classes named as Python names them (`py::ValueError`, `KeyError`, `StructError` whose `type()` says
`"error"`), `py::re` (a CPython-exact backtracking engine), `py::os`/`shutil`/`tempfile`/`hashlib`/`json`/
`subprocess`/`settings`, an `argparse` look-alike, and the translator's `py::ref<T>` (= `shared_ptr`).

**What the 742 ported files use** (counted over `native/typed/cpp`):

| Python-ism | Uses | What it really was |
|---|---|---|
| `py::str` | 38,245 | text that is ASCII in practice (assembly, C, identifiers, paths); code-point indexing never needed |
| `py::list<>` / `py::dict<>` / `py::set<>` | 8,495 / 3,024 / 1,478 | vectors and lookup tables; aliasing almost never relied on |
| `py::bytes` / `py::bytearray` | 4,459 / 39 | the binary under analysis; slicing with silent clamping |
| `py::re::*` | 1,941 (157 distinct literal patterns) | `\W`->`_`, identifier tests, trims, number scans, 12 lazy patterns, 7 lookarounds, 1 backreference |
| `py::os::*` | 1,927 | path joins, existence tests, makedirs |
| `py::subprocess::run` | 120 | running OpenWatcom, MASM, DOSBox, mklink |
| `throw py::ValueError/TypeError/AttributeError` | 291 / 276 / 175 | TypeError and AttributeError are Python's dynamic typing leaking in: **dead in C++**, delete them |
| `std::optional` / `std::variant` | 5,698 / 200 | already idiomatic; keep |
| `std::string` | 2,178 | a few modules (describe.cpp, parts of the recompiler) were already written over UTF-8 strings and hand-written scanners -- the target style |

**The code that never went through Python** is the style to match, and it is already most of the performance
story: `native/formats` (`cbf::pe::Image`, plain classes over a byte buffer), `native/engine` (SQLite-backed fact
store, step scheduler, worker pool), `native/l2` (the L2 node model, C back end), `native/tracer`,
`native/dosexec`, `native/x360`, `native/xenonrecomp`. They are DLLs with C interfaces called from Python through
`ctypes`, with a pickle-subset value format (`core/pyvalue.cc`) across the boundary, and a few Python-isms inside
(`PyError`, a `Bytes` that slices as Python does). Many typed ports are thin shells over them: `formats/pe.cpp`
turns `cbf::pe::Image` results into `py::str`/`py::list`/`py::dict` and `cbf::PyError` into `py::ValueError`.
Refactoring such a port means **deleting the shell and moving the real code into the new tree**.

**How codebreaker.exe is built today:** `tools/build_codebreaker_exe.cmd` runs `pyengine --dynamic` over every
Python module (a dynamic `py::var` fallback for what is not typed), links it with `pyrt` (a Python object model
and library in C++: `lib_re`, `lib_rich`, `lib_tui`, `lib_pil`, `lib_net`, `lib_ctypes` ...), and swaps in the
typed ports through `--dyn-native-typed`. It emits CMake (C++17, `/permissive- /utf-8 /EHsc /bigobj`, Ninja, 2 jobs,
unity batches of about 8,000 lines over a precompiled header). Third-party Python packages the program uses are translated
too (`native/pyrt/site.txt`: unicorn, capstone, pycdlib, z3, fontTools) or served by a pyrt library (PIL, rich,
jsonschema, mcp). **Ending the Python means replacing each of those with native code**, not only the 553 modules
of `codebreaker/`.

**Not ported yet** (no typed C++ in this checkout): `__main__`, `ui/` (mcp_server, service, serve, terminal,
progress), `exact/project` and a few dozen more. They are written directly in the target style, from the Python
for intent, never through the imitation layer.

## 2. The test of a refactored module

A module is done when a C++ programmer who never saw Python reads it and finds nothing odd, **and** the product
behaves the same. Concretely, the reviewer checks:

1. No `#include "py*.hpp"`, no `py::`, no `u32string`, no exception named after Python.
2. No function exists only because Python had it (`truthy`, `or_`, `none_attr`, `to_str`, `percent`, `len`).
3. Every container, integer and string has a type chosen for its domain (section 3), not for Python's.
4. Dynamic-typing residue is gone: `TypeError`/`AttributeError` throws, `variant<str,int64_t>` that only existed
   because a dict held mixed values, defensive `isinstance`-shaped branches.
5. The module's tests pass (section 7) and the product gates it feeds still give the same verdicts and files.

Behaviour that is **kept**: the algorithm, the decisions it makes, every output a user, a file or another tool
reads (files written, verdict tables, command output that scripts parse, L2 JSON, the names in generated C).
Behaviour that is **free to change**: error-message text, the incidental order of things where order carried no
meaning, Python's `repr` of values in messages, negative-index and clamped-slice leniency (bounds are now
checked explicitly), unbounded-int arithmetic past 64 bits, the `str` code-point model.

## 3. Conventions

### 3.1 Standard, compiler, flags

- **C++20**, MSVC (Visual Studio 2026, `C:\Program Files\Microsoft Visual Studio\18\Community`), CMake + Ninja.
  Reason: `std::span`, `std::format`, `starts_with`/`ends_with`/`contains`, designated initialisers, `<bit>`,
  `std::jthread`, `<ranges>` where it reads better; MSVC has all of it. C++23 `std::expected` is **not** used;
  `cb::Result<T>` (3.6) is ours so a compiler move never matters.
- Flags for `native/cb/`: `/std:c++20 /permissive- /utf-8 /EHsc /W4 /WX /Zc:__cplusplus /Zc:char8_t- /bigobj`,
  defines `NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE`, static CRT (`/MT`, as the other libraries), `/O2` release.
  `/Zc:char8_t-` keeps `u8` literals as `char` (we store UTF-8 in `std::string`). `/WX` is on from the first
  migrated module: warnings are fixed, not suppressed; a justified `#pragma warning` carries a comment.
- A debug configuration with `/fsanitize=address` (MSVC) exists for the test target; every module's tests run
  under it once before merge.
- No RTTI-dependent design, no `dynamic_cast` chains: use `std::variant` + `std::visit` for closed sets of node
  kinds, virtual functions only for real open extension points (a `Lane`, an `Emulator`, a `Platform`).

### 3.2 Text

- **`std::string` holding UTF-8, everywhere.** Function parameters are `std::string_view` (or `const std::string&`
  only when a NUL-terminated string is needed). Never `std::wstring`, `char16_t`, `u32string` in an interface.
- Text in CodeBreaker is overwhelmingly ASCII (instructions, C, identifiers, paths). **Text algorithms work on
  bytes**; they never index "characters". Where a code-point count is genuinely needed (terminal column widths,
  truncating a display string) call `cb::text::display_width` / `cb::text::utf8_prefix`. These are the only two.
- **A wide string exists in one place:** `cb/core/win.hpp` (`to_wide`, `from_wide`) used at the Win32 W-API
  boundary (`CreateProcessW`, `CreateFileW`, console). The exe carries a manifest with
  `<activeCodePage>UTF-8</activeCodePage>` and sets the console to code page 65001 at startup, so narrow Win32
  calls and `std::filesystem::u8path` agree.
- Text from a binary (a PE export name, an NE resident name, a DOS string) is **decoded once, at the reader**, by
  `cb::text::from_latin1 / from_cp437 / from_cp1252 / from_utf16le` into UTF-8, and re-encoded by the matching
  `to_*` only where the bytes are written back to a binary. A name that must round-trip byte-exactly (resource
  names, linker input) stays `Bytes` and is converted at display.
- Case: `cb::text::lower`/`upper` are **ASCII-only** (documented; Unicode case folding has no use here).
  `trim`, `split`, `join`, `replace_all`, `starts_with`, `lines` (splits on `\n` and `\r\n`) live in
  `cb/core/text.hpp`. Number to text: `std::format` (`{:08x}`, `{:#x}`); text to number:
  `cb::text::parse_int<T>(sv, base)` returning `std::optional<T>` (built on `std::from_chars`, no exceptions,
  rejects trailing junk). Floats in generated C use `std::to_chars` (shortest round-trip), never `%g`.
- Output formatting is **explicit**: there is no `repr`. A value in a message is formatted by the code that knows
  what it is (`std::format("0x{:08x}", addr)`).

### 3.3 Bytes

- `using Bytes = std::vector<std::uint8_t>;` owning. `using ByteView = std::span<const std::uint8_t>;`
  everything that reads. `std::span<std::uint8_t>` for in-place edits. Never `std::string` for binary data, never
  `std::vector<char>`.
- `cb::io::ByteReader` (a cursor over a `ByteView`: `u8 u16 u32 u64 i8..i64`, `bytes(n)`, `cstr()`, `skip`, `seek`,
  `remaining`, little-endian by default, `_be` variants) and `ByteWriter` replace `struct.pack/unpack`. A read past
  the end throws `cb::TruncatedError` with the offset and the size wanted. A format reader that must give a
  *shorter answer* on damaged input (the inspector readers: PE Explorer views) uses the `try_` forms that return
  `std::optional` -- the choice is per reader and stated in its header, as `native/formats/pe/pe.h` states it today.
- Binaries are untrusted input. Offsets and sizes are validated with overflow-safe 64-bit arithmetic
  (`cb::io::checked_range(view, off, len)`) before use. Python's silent clamping is **not** reproduced; a place
  that relied on a clamp says so in a comment and does the clamp explicitly.
- Memory-mapped files: `cb::fs::MappedFile` (a `ByteView`) for big inputs (ISOs, packages); small files are read
  into `Bytes`.

### 3.4 Integers

Fixed widths, chosen for the domain, written as `std::uint32_t` etc. (`<cstdint>`):

| Domain | Type |
|---|---|
| 16-bit/32-bit guest addresses and RVAs, instruction operands | `std::uint32_t` (`std::uint16_t` for 16-bit offsets/selectors) |
| PE32+/PowerPC/64-bit addresses, image bases | `std::uint64_t` |
| A real-mode far pointer | `struct Far16 { std::uint16_t seg, off; }` (`linear()` gives `seg*16+off`) |
| A file offset or size, a count of bytes | `std::uint64_t` for file/image offsets, `std::size_t` for in-memory sizes and indices |
| A signed displacement, a delta, anything that can be negative | `std::int64_t` |
| Counts of items (functions, segments) | `std::size_t` (or `int` for a handful of tiny bounded counters) |
| Hash/digest words | the exact width the algorithm names |

- Mixing signed and unsigned is a `/W4` error; cast **once**, at the boundary, with `static_cast`, and where the
  value comes from untrusted input check first.
- Python floor division and modulo (`py::floordiv`, `py::mod`) do not exist. Almost every use was on
  non-negative values (plain `/` and `%`); the few real ones (negative displacements into table indices) call
  `cb::floor_div`/`cb::floor_mod` (`cb/core/intmath.hpp`) by name.
- Shifts: `x << n` only with `n < width` (checked or constant). Rotates via `std::rotl`.
- `py::checked_*` becomes `cb::checked_add/mul` (`std::optional` result) **only** at trust boundaries; elsewhere
  an overflow of a 64-bit domain value is a bug, caught by the sanitizer build, not a runtime concept.
- **Big integers: not in the core.** The only arbitrary-size arithmetic in the program is RSA/DSA-sized
  modular exponentiation in `formats/xbox/crypto` (fixed 1024-4096 bits). It gets a small fixed-capacity
  `U4096` with Montgomery multiplication inside that module (`cb::crypto::bignum`), or Windows CNG (`BCrypt`,
  already linked by `native/engine`) where the operation exists there. No general bigint library.

### 3.5 Containers, optional, variant, ownership

- **Sequence:** `std::vector<T>`. Parameters `std::span<const T>` (or `const std::vector<T>&` when a vector is
  needed). Returned by value. `std::array<T,N>` for fixed tables. No `py::list` aliasing: where Python code
  appended to a list it was handed, the C++ takes `std::vector<T>&` and says so in the name (`append_to`) or
  returns the new elements.
- **Mapping, decided by what the map is used for** (this is the one rule reviewers check hardest):
  1. *Lookup only, never iterated into an output or a decision:* `std::unordered_map`.
  2. *Iterated, and a deterministic sorted order is right* (reports, symbol tables, anything diffed between runs):
     `std::map` (or a sorted `std::vector<std::pair>`).
  3. *Iterated, and the **insertion** order is the meaning* (a function's locals in declaration order, JSON written
     in the order the producer chose, a build's steps): `cb::OrderedMap<K,V>` (`cb/core/ordered_map.hpp`: a
     `std::vector<std::pair<K,V>>` plus a hash index; O(1) lookup, stable iteration, erase is O(n) and rare).
  Iterating an `unordered_map` to produce output is a defect. Python 3.7+ dicts were always ordered, so a port
  that silently depended on it is found by this rule: ask "does order show up in a file or a verdict?".
- **Set:** `std::set` when iterated, `std::unordered_set` otherwise, sorted `std::vector` for small immutable sets.
- `dict.get(k, d)` is `cb::get_or(map, k, d)`; "absent is normal" uses `find`/`std::optional<const V&>` wrappers,
  not exceptions. `map.at(k)` throwing `std::out_of_range` means "absent is a bug".
- **Optional:** `std::optional<T>` for "or None". Never a sentinel `-1`/`""` unless the original format itself
  uses it (a PE field). **Variant:** `std::variant` only for a real closed union in the domain (an address that is a
  linear `uint32_t` or a `Far16`; an L2 expression kind), visited with `std::visit`; never to hold "whatever the
  dict had".
- **Records:** a `struct` with named, typed members and in-class defaults (already how the ports render fixed-key
  dicts). Designated initialisers at construction. No `std::tuple` in an interface where a struct with names
  would read better; `std::pair` only for map entries.
- **Ownership:** values by default; `std::unique_ptr` for a tree or polymorphic object; `std::shared_ptr` only for
  a graph with genuinely shared ownership (a function referenced by the call graph and the module). `py::ref<T>`
  everywhere is a smell: each one is decided, not translated. Large trees that are built once and freed together
  (L2 nodes, IR) use an arena (`cb::Arena`, nodes hold `NodeId`/raw pointers into it).
- **L2** is the typed node model of `native/l2/src/l2.hpp`, not a `Dict`/`Val` tree. The ports' `translate::ir::Dict`
  and `Val` exist because Python held JSON documents as dicts; the refactor reads and writes the JSON at the edge
  (`l2::from_json`, `to_json`) and works on typed nodes in between.
- **Strings as keys:** `std::string` (UTF-8). For case-insensitive Windows names keep a `cb::text::ilower`-keyed
  map (`cb::IMap<V>`), not a custom comparator scattered about.
- No mutable globals (3.11). Module-level constant tables are `inline constexpr`/`static const` built by a
  function-local static.

### 3.6 Errors

One hierarchy, in `cb/core/error.hpp`; every exception derives from `cb::Error`, which derives from
`std::runtime_error` (message only; the **type** is the contract, the message is for humans).

```
cb::Error
  IoError          path + OS error code; NotFoundError, AccessError are subtypes
  FormatError      a binary or data file is malformed: reader name, offset, what
    TruncatedError   a read ran past the end
    UnsupportedError this variant of the format is valid but not handled
  ParseError       source/text input did not parse: file, line, column
  ToolError        an external program failed: tool, exit code, captured output; TimeoutError is a subtype
  EmulationError   the guest faulted in a way the harness cannot continue from (unmapped, bad opcode, cap exceeded)
  UsageError       the command line or API was misused (exit code 2)
  InternalError    an invariant of CodeBreaker's own was violated (a bug); never caught except at top level
```

- **Python exception names do not appear**: no `ValueError`, `KeyError`, `RuntimeError`, `StructError`,
  `AssertionError`, `SystemExit`. Each Python `raise X` is mapped by *meaning*: bad input data -> `FormatError`/
  `ParseError`; failed tool -> `ToolError`; missing file -> `NotFoundError`; "can't happen" -> `CB_ASSERT`
  (below). The 276 `TypeError` and 175 `AttributeError` throws are deleted with the dynamic typing that needed them.
- **Exceptions** for faults that cross more than one frame and that the caller usually cannot handle locally.
  **`cb::Result<T>`** (`std::variant<T, Error>` with `.ok()`, `.value()`, `.error()`, `CB_TRY(expr)` that returns
  early) for failure that is **routine and expected on the hot path**: probing ("is this a PE?"), trial decoding,
  a compiler that rejects a candidate, parse of optional syntax. **`std::optional<T>`** when absence needs no
  reason. A Python `try: ... except ValueError: return None` becomes a `try_`-prefixed function returning
  `optional`/`Result`, never a C++ `try/catch` used as control flow.
- `catch (...)` and `catch (const std::exception&)` are only allowed at *isolation boundaries*: one function of a
  batch (a failure becomes that function's verdict `no-compile`/`error` and the run goes on), a worker's job loop,
  the command dispatcher. They log the error and convert; they never swallow silently.
- `CB_ASSERT(cond, "why")` (always on) throws `InternalError` with file:line. Used for invariants, not input checks.
- **Exit codes** of codebreaker.exe: `0` success, `1` the command ran and reports failure (a verdict gate failed,
  a build failed), `2` usage error, `3` a requested thing was not found (the exit code the xbox-disc commands
  already use), `70` internal error. `cb::app::main` is the only place that maps `Error` types to codes.

### 3.7 Regular expressions

`std::regex` is slow and wrong for this; the Python's `re` semantics are not the goal either. Decision, in order of
preference for every pattern met in a port:

1. **Replace it with code.** Most of the 157 distinct patterns are idioms: `\W`->`_` (a loop),
   `[A-Za-z_]\w*` as a whole-string test (`cb::text::is_identifier`), `^\s*` (`ltrim`), `\s+` splits (`split_ws`),
   `-?(0x[0-9a-f]+|\d+)` (`parse_int`), `(?<![\w.])0x...` scans (a tokenizer step). These become named functions in
   `cb/core/text.hpp` or the module itself. This is the *default*; it is faster and reads better.
2. **A real lexer/parser where the regex was a tokenizer.** The comment/string/char-literal alternations over C and
   assembly source are a lexer (`cb/text/lex.hpp`: `CLexer`, `AsmLexer`), written once and shared by every pass that
   walked C text with a regex.
3. **`cb::Regex`** for what is left (a few dozen genuinely structural patterns: the NASM-listing and Watcom-map
   formats, the MASM `DESCRIPTION '...'` form, the demangler's lazy matches). `cb/core/regex.hpp`: the engine of
   `native/l2/src/pyre.cc` (backtracking, with prefilters) **moved in-tree and re-specified**: ECMAScript-like
   syntax with groups, `(?:)`, `(?<name>)`, lazy quantifiers, `\b`, lookahead, fixed-width lookbehind, one
   backreference form; **`\w \d \s` are ASCII**; matching is over bytes, offsets are byte offsets; flags
   `icase`, `multiline`, `dotall`; a **step budget** (default 10M) that returns a `Result` error instead of hanging
   on a pathological pattern/input. API: `search`, `match` (anchored at start), `full_match`, `replace(all|first)`,
   `for_each_match`, `split`; `Match` exposes `group(i)` as `std::string_view` into the subject.
   Patterns are written as **raw string literals** (`R"(...)"`, never double-escaped) and compiled **once**:
   `static const cb::Regex re(R"(...)");` -- a `Regex` constructed inside a loop is a review failure.

No download: the engine is already in the repository. RE2 (no lookaround/backreference, would force rewrites of
~10 patterns) and PCRE2 (full, JIT, one more dependency) were considered; see section 9 for the one case in which
PCRE2 would be proposed (if profiling shows a regex on a hot path the in-tree engine cannot meet).

### 3.8 JSON and configuration

- **One JSON library: `nlohmann::json` (3.10.5, MIT), already vendored** at
  `native/x360/system/live/third_party/nlohmann/json.hpp`. It moves to `native/third_party/nlohmann/` (a `git mv`,
  nothing downloaded). Alias `cb::Json = nlohmann::ordered_json` -- insertion order preserved, so written files are
  stable. The `$t/$s/$d/$f` tagged forms the Python shim invented to carry tuples/sets/non-string keys are **not
  carried over**: typed structs carry them.
- JSON is an **edge** format: it is parsed into, and written from, typed structs by `from_json`/`to_json` in the
  owning module's `.cpp` (nlohmann's `NLOHMANN_DEFINE_TYPE_*` macros). `Json` values do not travel between
  modules. Headers never include `json.hpp` (its compile cost is large); they declare `load(path)`/`save(path)`.
- Writing: `Json::dump(2)` for files people read or diff (L2, reports, manifests); compact for caches.
  `ensure_ascii` off (UTF-8). Files that the **remaining Python** still reads during the transition keep the layout
  the Python wrote (`", "` and `": "` separators, `ensure_ascii`) -- one switch in `cb::json::save(path, j,
  Style::PythonCompatible)`, removed with the Python.
- Settings (`settings.json`) are read through `cb::config` (typed: `workers`, `low_load`, `min_free_gb` ...), loaded
  once into a `Config` struct held by the `App` object, replacing `py::settings::get_str` and its TypeError
  workarounds.
- Binary caches and pickles: a C++ cache is a versioned binary format of our own (`cb::io::ByteWriter`, a
  4-byte magic + schema version, key = hash of inputs as the cache rules in `base/cache.py` define). A pickle written
  by Python is never read by C++; the cache just misses and is rebuilt.

### 3.9 Filesystem

- `std::filesystem` throughout. Paths are `std::filesystem::path` internally, joined with `/`, never as strings
  concatenated with `\\`. `py::os::path::{join,basename,dirname,splitext,normpath,abspath}` map to
  `operator/`, `filename()`, `parent_path()`, `stem()/extension()`, `lexically_normal()`, `absolute()`.
  Conversion to text is `path.generic_string()`/`u8string` in `cb::fs::to_text`; from text `cb::fs::from_text`.
- `cb/core/fs.hpp` adds what `std::filesystem` lacks: `read_file(path) -> Bytes`, `read_text`, `write_file`,
  `write_file_atomic` (temp + rename), `MappedFile`, `ScopedTempDir` (RAII, deletes on scope exit **including on
  exception**, so harnesses stop leaking multi-GB temp folders), `ScopedEnv`, `copy_tree`, `remove_tree`, and a
  `long_path()` that adds the `\\?\` prefix. Windows path comparison is `cb::fs::same_path` (case-insensitive).
- All file errors are `IoError`. No raw `FILE*`/`std::ifstream` outside `cb/core/fs.cpp` and the process module.
- Project data stays inside the project folders (`CodeBreaker/state/...`), as the standing rule says; the layout is
  owned by `cb::paths` (one place that knows `state/`, `Converted/`, the corpus root, `CB_CHECKOUT_ROOT`).

### 3.10 Processes

One API, `cb/core/process.hpp`, over `CreateProcessW` (UTF-8 in, converted at the edge):

```cpp
struct ProcessSpec {
    std::vector<std::string> argv;            // argv[0] resolved through PATH + the env below
    std::optional<std::filesystem::path> cwd;
    std::map<std::string, std::string> env;   // overrides on top of the inherited environment
    std::vector<std::string> env_remove;
    Bytes stdin_data;                         // empty: stdin is NUL
    Capture out = Capture::Collect;           // Collect | Inherit | Discard | ToFile(path)
    Capture err = Capture::Collect;           // or Merge (stderr into stdout)
    std::chrono::milliseconds timeout{0};     // 0 = none
    Priority priority = Priority::Normal;     // Normal | BelowNormal (background sweeps)
};
struct ProcessResult { std::uint32_t exit_code; Bytes out, err; bool timed_out; std::chrono::milliseconds elapsed; };
ProcessResult run(const ProcessSpec&);                 // throws ToolError only if it cannot start
Process      start(const ProcessSpec&);                // streaming: read(), write(), wait(), kill()
```

- Output is **bytes**; the caller decodes with the codec the tool speaks (`from_cp437`, `from_latin1`), once.
  Exit codes are unsigned 32-bit as Windows gives them.
- Every child is put in a **Job Object** created by us (kill-on-close). A timeout, or our own exit, terminates
  exactly the processes in that job. **No code walks process trees or kills by name/PID it did not start**
  (the standing safety rule); the Job Object is how children-of-children are cleaned up without it.
- `cmd /c`, `mklink` and batch files are called as `cmd` with an argv; shell strings are never built with `+`.
  Quoting follows the Windows argv rules in one function (`cb::quote_arg`).
- `cb::Process` replaces `py::subprocess`, `os.system`, `subprocess.Popen` and the DOSBox/OpenWatcom wrappers'
  private variants (`build/driver.cpp::_run`, `flat_thunk.cpp`, `msc6.cpp`).

### 3.11 Threads, state, worker processes

- **No mutable globals.** Python modules held state in module variables (the Java profile registry with its
  `on_change` observers, the tracer's `cache_lock`-guarded `_INSN`/`_DECODED`, `emulator` caps). In C++ that state is
  an object -- `Session`, `ProfileSet`, `Emulator`, `TraceCache` -- created by the caller and passed down
  (`const&` for read-only context). Immutable tables are function-local statics (thread-safe initialisation).
- **Threads are the default** for parallel work: `cb::ThreadPool` (`cb/core/parallel.hpp`; `parallel_for`,
  `parallel_map`), sized by `cb::parallel::workers()` (settings `workers`, `CB_JOBS`, `low_load`; the rule today in
  `base/parallel.py`), plus the machine-slot sharing across CodeBreaker processes already built.
- Each emulator instance (a Unicorn engine, a DOS executive) belongs to **one thread**; instances share nothing
  mutable except through a `cb::SharedCache` (sharded mutexes, keyed by content hash).
- **Worker processes** only where isolation is the point: running arbitrary guest code that can crash the host,
  a memory ceiling, or a legacy module that still holds process-global state and has not yet been de-globalised.
  The mechanism is the same exe: `codebreaker.exe worker` reads length-prefixed binary job messages on stdin and
  answers on stdout (`cb/app/worker.hpp`), launched through `cb::Process` inside a Job Object.
  `build_many`'s "each module in its own Python" becomes a thread pool over self-contained `Build` objects; it
  falls back to workers only for a module whose lane is not yet de-globalised.

### 3.12 Command line, logging, output

- **CLI:** one small in-tree parser, `cb/core/cli.hpp` (~400 lines, no dependency). A command is declared, not
  parsed by hand:

  ```cpp
  cli::Command{"decompile", "decompile one function or a whole module",
      {cli::Positional{"module", "corpus name or path"},
       cli::Option{"--function", "-f", "name or hex address", cli::Arg::Value},
       cli::Option{"--judge",    "",   "run the equivalence judge", cli::Arg::Flag}},
      [](const cli::Args& a) { ... return exit_code; }}
  ```
  Grammar: `--opt value`, `--opt=value`, `-o value`, `-abc` clusters of flags, `--` ends options, repeated options
  collect into lists, subcommand tree (`codebreaker <command> [<sub>] ...`). Help is **ours**: a command's
  `--help` prints `usage`, a one-line summary, then aligned option rows; `codebreaker help [command]` lists
  commands grouped by area. Errors: `UsageError` -> message + usage on stderr, exit 2. The command table
  (`cb/app/commands.cpp`) is the single registry; MCP `commands`/`command_help` read the same table.
  argparse's abbreviation matching, `%(default)s` help expansion and its error wording are not reproduced.
- **stdout is for results, stderr for everything else.** Machine-read output (the `PROGRESS n m` lines, TSV the
  Dashboard's tools parse) is a documented contract per command and stays byte-stable.
- **Logging:** `cb::log` (`cb/core/log.hpp`): `log::error/warn/info/debug(std::format(...))`, level from
  `--verbose/--quiet` or `CB_LOG`, to stderr, optional file; thread-safe, no timestamps unless `CB_LOG_TIME`.
  Progress for long jobs goes through `cb::Progress` (the job system's reporter), not `printf`.
  `std::cout <<` chains are not used; `cb::out(fmt, args...)` writes UTF-8 to stdout.

### 3.13 Naming and file layout

- **Layout:** the new tree is **`native/cb/`**; it mirrors the package areas:
  `native/cb/core/` (3.2-3.12), `cb/formats/`, `cb/analyze/`, `cb/decompile/`, `cb/translate/`, `cb/emulate/`,
  `cb/verify/`, `cb/build/`, `cb/exact/`, `cb/java/`, `cb/xna/`, `cb/recompile/`, `cb/engine/`, `cb/knowledge/`,
  `cb/store/`, `cb/jobs/`, `cb/platforms/`, `cb/app/` (commands, service, mcp, terminal, worker, main). Include
  paths are rooted at `native/`: `#include "cb/formats/pe.hpp"`.
- **One module = one `.hpp` + one `.cpp`** (`pe.hpp`, `pe.cpp`) **+ its test** `pe_test.cpp` next to it. A module
  over ~3,000 lines (describe, c_text, java/transpile, shapes) is split by concern into several `.cpp` behind one
  `.hpp` (`describe_ne.cpp`, `describe_dos.cpp` ...); the Python's one-file-per-package sprawl is not a reason to
  keep a 3,000-line file.
- **Namespaces:** `cb::<area>::<module>` for a module's API (`cb::formats::pe`), `cb::<area>` when a module exports
  one main type. Internals in an unnamed namespace in the `.cpp` (no `_leading_underscore` names, no `detail_`
  hunts across headers; a helper two modules need is promoted to a real, named function).
- **Style:** types `PascalCase`, functions and variables `snake_case`, constants `kPascalCase`, enumerators of an
  `enum class` `PascalCase`, template parameters `T`/`Key`. Members `snake_case` without trailing underscore
  for structs; private members of classes `snake_case_`. Headers `#pragma once`, include what you use, no
  `using namespace` in headers. 120-column lines, 4-space indents (the style of the existing native code).
  `.clang-format` at `native/cb/` pins it.
- **Comments:** keep the Python docstring's *prose* as `//` comments above the function (the explanation of why is
  the most valuable thing the port has), drop the typing notes that exist only to explain how a Python value was
  rendered. Header-top comment says what the module is, in two paragraphs at most.
- A header includes only what its *declarations* need; heavy ones (`json.hpp`, `<windows.h>`, Unicorn, SQLite)
  never appear in a public header.

## 4. Before and after

### 4.1 formats/pe -- delete the shell, move the real code in

Before (`typed/cpp/codebreaker/formats/pe.cpp`): every method re-opens the DLL's reader, converts, and renames
exceptions.

```cpp
py::dict<py::str, int64_t> PEFile::export_addrs() const {
    return guard([&] {
        py::dict<py::str, int64_t> out;
        for (const cbf::pe::Export& e : opened(*this).exports()) {
            if (!e.name || e.name->empty()) continue;
            out.set(L(*e.name), static_cast<int64_t>(e.rva));   // a name met again keeps its place, takes the last rva
        }
        return out;
    });
}
...
if (k == "struct.error") throw py::StructError(m);   if (k == "ValueError") throw py::ValueError(m);
```

After (`cb/formats/pe.hpp/.cpp`: `cbf::pe::Image` *is* the module now, with its `PyError`/`Bytes` imitation removed):

```cpp
namespace cb::formats::pe {
class Image {
public:
    static Image parse(ByteView file);                 // throws FormatError / TruncatedError
    static std::optional<Image> try_parse(ByteView);   // for probing: "is this a PE?"
    std::uint64_t image_base() const { return image_base_; }
    // Names the export directory gives, in directory order; a name met twice keeps the later rva.
    const OrderedMap<std::string, std::uint32_t>& export_addrs() const;
    std::span<const Export> exports() const;
    ...
};
struct Export { std::uint16_t ordinal; std::optional<std::string> name; std::uint32_t rva; };
}
```
`PEFile` (a handle that re-parses on every call) is gone; callers hold an `Image`. The `L()`/`T()` latin1-to-`py::str`
helpers move into the reader (`from_latin1` once, when the table is read). The ctypes/pickle C ABI (`api.cc`,
`core/pyvalue.cc`, `questions.cc`) is deleted when its last Python caller is.

### 4.2 decompile/describe -- regexes and string helpers become named functions

Before:
```cpp
// re.sub(r"\W", "_", s)
py::str ident_py(const py::str& s) { return py::re::sub(py::str("\\W"), py::str("_"), s); }
// re.fullmatch(r"[A-Za-z_]\w*", s)
bool c_name(const py::str& s) { return py::re::fullmatch(py::str("[A-Za-z_]\\w*"), s).has_value(); }
std::optional<int64_t> try_int(const string& s, int64_t base) {
    try { return py::int_parse(py::str(s), base); } catch (const py::ValueError&) { return std::nullopt; }
}
```
After (`cb/core/text.hpp`, shared by every module that had these):
```cpp
std::string sanitize_identifier(std::string_view s);   // every byte that is not [A-Za-z0-9_] becomes '_'
bool is_identifier(std::string_view s);                // [A-Za-z_][A-Za-z0-9_]*
template <class T> std::optional<T> parse_int(std::string_view s, int base = 10);   // from_chars, no exception
```
and the module's three hand-rolled `split_on`/`split_strip_commas`/`before_comma` become `cb::text::split`,
`split_trim`, and `s.substr(0, s.find(','))`. Python's 3,052-line `describe` splits into `describe_ne.cpp`,
`describe_dos.cpp`, `describe_pe.cpp`, `describe_le.cpp` behind one `describe.hpp`; `Description` (one class for all
kinds, members "empty for other kinds") becomes `std::variant<NeDescription, PeDescription, LeDescription,
DosDescription>` plus the shared `Description` base facts, because the C++ type system can say what Python could
not.

### 4.3 emulate/windows16 -- bytes, spans, records, errors

Before:
```cpp
py::dict<int64_t, int64_t> sels, seg_bases;
...
py::bytes body = nef.data.slice(s.file_off, s.file_off + s.length);
if (!body.empty()) uc.mem_write(base, body);
harness_list.emplace_back(base, 0xFFFF, s.kind == py::str("CODE"));
...
if (static_cast<int64_t>(harness_list.size()) > _N_GDT - 24)
    throw py::ValueError(py::percent(py::str("%d segments: more than the GDT holds"), segs.size()));
```
After:
```cpp
struct Loaded16 { std::vector<std::uint16_t> sels;  // indexed by NE segment number - 1
                  std::vector<std::uint32_t> seg_bases;  std::uint16_t stack_sel, sent_sel; };

for (const ne::Segment& s : ne.segments()) {
    ByteView body = io::checked_range(ne.data(), s.file_off, s.length);   // FormatError if the table lies
    uc.mem_map(seg_base(s.index), kSegWindow);
    if (!body.empty()) uc.mem_write(seg_base(s.index), body);
    gdt.push_back({seg_base(s.index), 0xFFFF, s.kind == ne::SegKind::Code});
}
if (gdt.size() > kGdtSlots - kHeapSlots)
    throw UnsupportedError(std::format("{} segments: more than the GDT holds", segs.size()));
```
Dense integer-keyed `dict` -> `vector` (the keys are 1..n). `s.kind == "CODE"` string compare -> `enum class`.
`_`-prefixed globals (`_N_GDT`, `_FIXED_SLOT`) -> `constexpr` in the unnamed namespace. The Unicorn wrapper takes
`ByteView`, not `py::bytes`.

### 4.4 java/transpile -- module state becomes an object

Before: a profile *registry* in module-level statics, `use_profile()` switching it and "reseeding" four other
modules' globals through `on_change_`, `convert()` taking a lock so only one conversion runs.
```cpp
void on_change_(const Profile& p) {
    typemap::Profile t;  t.nullable = py::set<str>::from(p.nullable);  ...
    typemap::_reseed(t);  rules::_reseed(p.heavy_types, p.heavy_types_high);  console::vcxproj::_reseed(...);
}
```
After: a `Conversion` takes a `const Profile&` and hands it to the stages that need it; nothing is global, two
conversions run in parallel, and the one-at-a-time lock disappears:
```cpp
struct ConversionOptions { const Profile& profile; CppStandard std; ProgressFn progress; Ledger* ledger = nullptr; };
Summary convert(const fs::path& project, const fs::path& out, const ConversionOptions& opt);
// typemap::map_type(const Profile&, ...), rules::check(const Profile&, ...) read what they need from opt.profile
```
The Minecraft profiles are registered by the project object that owns them (`MinecraftProject::profiles()`), found by
name from a `ProfileSet` the `App` builds at start.

### 4.5 build/driver -- one process API

Before: `_run(py::list<py::str> cmd, py::str cwd)` building `RunOptions`, concatenating stdout+stderr bytes, decoding
latin1/replace, catching `py::OSError`. After:
```cpp
ProcessResult r = process::run({.argv = {wasm, "-zq", "-dIS_32=1", "-i=" + kit, "-fo=" + obj.string(), src.string()},
                                .cwd = work, .env = watcom_env(Target::Pe), .err = Capture::Merge,
                                .timeout = 5min});
std::string log = text::trim(text::from_latin1(r.out));
return (r.exit_code == 0 && fs::is_regular_file(obj)) ? Assembled{obj, log} : Assembled{std::nullopt, log};
```
(`timeout` is new: the Python-shaped runtime could not give one, a standing hole.)

## 5. Idiom table (mechanical part)

| Python-shaped | Idiomatic |
|---|---|
| `py::str` / `py::len(x)` / `x[i]` on text | `std::string` / `x.size()` / byte index (ASCII) |
| `s.startswith(p)`, `.endswith`, `p in s` | `s.starts_with(p)`, `.ends_with`, `s.contains(p)` |
| `s.strip()`, `.split(sep)`, `.join`, `.replace`, `.lower()` | `text::trim`, `text::split`, `text::join`, `text::replace_all`, `text::lower` (ASCII) |
| `py::percent("%08x", v)`, `py::format` | `std::format("{:08x}", v)` |
| `py::list<T>` with `.append`, `for x in xs` | `std::vector<T>` with `push_back`, range-for |
| `xs[-1]`, `xs[a:b]`, `xs[::2]` | `xs.back()`, `std::span(xs).subspan(a, b-a)`, an explicit loop |
| `py::dict<K,V>` | by use: `unordered_map` / `map` / `OrderedMap` (3.5) |
| `d.get(k)` / `k in d` / `d[k]` raising KeyError | `find` / `contains` / `at` (bug if absent) |
| `py::bytes` slicing, `b[i]` int, `bytes + bytes` | `ByteView::subspan`, `view[i]`, `append(out, view)` |
| `struct.unpack("<HHI", ...)` | `ByteReader r(v); auto a = r.u16(); ...` |
| `py::floordiv`, `py::mod`, `py::shl/shr` | `/`, `%` (non-negative) or `cb::floor_div`; plain shifts with checked width |
| `py::or_(a, b)`, `py::truthy(x)`, `and_` | `a.empty() ? b : a`, `!x.empty()`, ternaries written out |
| `py::none`, `optional` + `none_attr` | `std::nullopt`, `if (x) ...` |
| `py::ref<T>` | `unique_ptr`/value/arena by ownership; `shared_ptr` only if shared |
| `py::os::path::*`, `py::os::makedirs/remove/walk` | `std::filesystem` + `cb::fs` |
| `py::subprocess::run`, `RunOptions` | `cb::process::run(ProcessSpec)` |
| `py::hashlib::{md5,sha1,sha256}` | `cb::crypto::{md5,sha1,sha256}(ByteView)` (CNG/BCrypt where it is the same, in-tree for the rest) |
| `py::tempfile::*` | `cb::fs::ScopedTempDir` |
| `py::settings::get_str` | `cb::Config` fields |
| `py::json::*` / `py_json.hpp` | `cb::Json` (nlohmann) at the edge only |
| `py::re::*` | 3.7 (code first, `cb::Regex` last) |
| `py::SystemExit` | `cb::app::Exit{code}` thrown only to `main` (or just return the code) |
| `repr`, `to_str`, `float_repr_` in output | explicit format; `std::to_chars` for floats |
| "catch py::Exception" | catch `cb::Error` at an isolation boundary only |
| module-global mutable state, `_reseed` hooks | an object passed down (3.11) |

## 6. Migration plan

### 6.1 Two worlds, one rule

During the transition both exist: **old** = `native/typed/cpp/codebreaker/...` in namespace `codebreaker::`, over
`py.hpp`; **new** = `native/cb/...` in namespace `cb::`. They never collide (different namespaces, different
include roots), and the old directory shrinks to nothing: **the migration is finished when
`native/typed/cpp/codebreaker/` and `native/ported/` are empty.** `git mv`-ing is not used for the module body (it is
rewritten), so history shows the rewrite as new files and a deletion; the commit message names the module.

### 6.2 Order: bottom-up by include dependency

A refactor agent picks a module only when **every module it includes is already refactored** (or is one with no
dependency). `tools/cb_order.py` (a one-off, written first; deleted with the Python) computes the include graph of
`native/typed/cpp`, condenses strongly connected components (cyclic modules are refactored **together**, one agent,
one PR) and writes `docs/cpp_order.txt`: waves 0..N, each a list of modules. Wave summary:

0. **`cb/core`** (hand-written by one agent before any module): `error`, `text`, `io` (ByteReader/Writer/checked_range),
   `intmath`, `ordered_map`, `fs`, `process`, `json` glue, `regex` (moved from `native/l2/src/pyre.cc`), `cli`, `log`,
   `parallel`, `testing`, `win`, `crypto` (hashes), `Arena`. Plus `cb/compat/py_bridge.hpp` (6.3). **This is the
   critical path**; nothing else starts before wave 0's headers are reviewed, because they fix the vocabulary.
1. **Leaf data and readers:** `formats/*` (moving `native/formats` in: pe, ne, le, dos, omf, xex, cab, lzx, detect,
   resources, the console cartridge formats), `symbols`, `base` (cache, runs, commands, corpus, parallel), `store`,
   `platforms`, `compare`, `knowledge`, `engine` (moving `native/engine` in, deleting its Python callbacks).
2. **Analysis and the decompiler spine:** `analyze/*`, `decompile/*` (describe, frame16, ctree, asm_blocks, inline_asm,
   c_text, jar), `translate` (ir -> typed L2 model from `native/l2`, `l2`, `rewrite`, `desugar`, then `backends/*`
   and `frontends/*`).
3. **Emulation and judging:** `emulate/*` (with `native/tracer` and `native/dosexec` moving in: they are already C++),
   `verify/*` (`compare`, `concolic`, `guided`, `verdicts`, `lanes/*`, `gate16`).
4. **Builders and the rest:** `build/*`, `exact/*`, `invert`, `java/*` (transpile, runtime, console), `xna`,
   `recompile`, `port`, `jobs`, the VxD lane, the Xbox crypto and ZAR.
5. **Applications:** `app/` -- the command table, `__main__`, `ui/service`, `ui/mcp_server` (JSON-RPC over stdio on
   `cb::Json`), `ui/terminal` (decision in section 9), `ui/serve`, `worker`.

Within a wave, **speed first**: the modules the exe's hot paths use (judge tracer, describe, c_text, compare) are
taken before cold ones, since Logan's standing rule is that slow is unacceptable.

### 6.3 Interoperating during the transition: compat shims

The old tree keeps compiling while new modules appear, so each refactored module leaves behind a **shim at the old
header path**:

```cpp
// native/typed/cpp/codebreaker/formats/pe.hpp   (shim; deleted when its last py:: consumer is refactored)
#pragma once
#include "cb/compat/py_bridge.hpp"
#include "cb/formats/pe.hpp"
namespace codebreaker::formats::pe {
struct Section { py::str name; int64_t vsize = 0, vaddr = 0; ... };           // the OLD record, as callers use it
class PEFile { /* same methods and signatures as before, bodies call cb::formats::pe::Image */ };
}
```
`cb/compat/py_bridge.hpp` (small, one agent writes it in wave 0): `to_py(std::string_view) -> py::str`,
`from_py(const py::str&) -> std::string`, and generic converters for `vector<->py::list`, `map/OrderedMap<->py::dict`,
`set<->py::set`, `Bytes<->py::bytes`, `optional`, `tuple`, plus `from_py_error` (the reverse of 3.6, so a `cb::Error`
crossing into old code becomes the matching `py::` exception). Rules:

- A shim has **no logic**: conversion and forwarding only. If it needs logic, the new API is wrong.
- Bottom-up order means **new code never calls old code**. If an agent must (a cycle, a not-yet-refactored
  dependency), it uses `cb/compat/from_py.hpp` at the call site and marks the line
  `// COMPAT(py): remove when <module> is refactored`; `tools/cb_compat_count` (grep) reports these, and a merge
  may not raise the count of a module that is not part of its own cycle.
- Python-side callers (the dynamic exe's translated Python, until it is gone) reach the new code through the same
  shim, via `--dyn-native-typed`, exactly as the ports are reached today. A module's *interface to Python* is
  therefore never the new API; **Python never sees `cb::` types.** That is what keeps the exe working at every merge.
- Persisted formats stay compatible with the remaining Python until it is retired (JSON layout, 3.8); C++-only
  caches use the new binary format and a new cache `kind` so the two never read each other's files.

### 6.4 The refactor recipe (what each agent does, in order)

For `codebreaker/<area>/<name>` (rules of `state/work/typed-port/BRIEF.md` still apply: edit only your own
worktree, never kill a process you did not start, one build at a time on the shared machine, scratch under
`state/work/cpp-refactor/<module>/`, never the Temp scratchpad):

1. **Read** the typed port, the original `.py` (for the *intent* and docstrings), the callers (`grep` the include
   graph), the module's old harness (`state/work/typed-port/<module>/`: `expected.py`, `expected.txt`).
2. **Decide the API** on a page: the public functions, the records, who owns what, which errors it raises
   (by section 3.6). The header is written first and is the review target; it contains no `py::`.
3. **Write the new `.hpp/.cpp`** under `native/cb/<area>/`, keeping the algorithm line for line where it is
   algorithmic, replacing Python-isms per section 5, deleting dead dynamic-typing branches, naming magic numbers.
   Constants are copied from the port, not retyped.
4. **Regexes** are classified (3.7) and each pattern gets a test case.
5. **Container pass:** for every `dict`/`set`, write the one-line reason for the container chosen in a comment
   where it is not obvious (3.5); check aliasing hazards (a by-value parameter that used to be a shared `py::list`
   and is mutated; iteration while appending).
6. **Tests:** `<name>_test.cpp` (section 7.1): cases from the old harness, normalised, plus bounds/damage cases
   for readers.
7. **Shim** at the old header path (6.3); the old `.cpp` is deleted in the same change (the shim is header-only).
8. **Build** `cb_tests` (+ASan) and the exe; run the module's tests, then the product checks section 7.2 lists
   for its area; time it against the old port (`docs/TYPED_CPP_SPEED.md`): not slower beyond noise.
9. **Report** in the commit message: what changed in behaviour on purpose (if anything), which old harness cases
   were dropped as incidental (with the reason), COMPAT lines left, timings.
10. **Review** (Claude, per the standing rule): header read first; the idiom checklist (appendix); rerun the
    tests and the area's product check before merging. A merge that is green on its own tests but fails a product
    check is reverted, not patched forward.

### 6.5 When the `py::` layer can be deleted

In three steps, each gated by a grep, not a judgment:

1. **Shims go** when `grep -r "py_bridge\|COMPAT(py)" native/cb` is empty and no old header is included: the files in
   `native/typed/cpp/codebreaker/` are deleted module by module as their last consumer is refactored.
2. **The layer goes** when `grep -rl 'py\(_[a-z]*\)\?\.hpp' native` finds nothing outside `native/pyrt`/`native/pyengine`:
   delete `native/ported/`.
3. **The runtime goes** with the Python (section 8): `native/pyrt`, `native/pyengine`, `native/pyfront`.

## 7. Verification without Python-mimicry

What proves a module correct is **what CodeBreaker does with it**. Three layers, cheapest first; each answers a
different question.

### 7.1 Module tests: C++, in the tree, forever

- `native/cb/<area>/<name>_test.cpp`, all built into one `cb_tests.exe` (a registry of `CB_TEST("formats.pe.exports", [] {...})`
  from `cb/core/testing.hpp`: `CB_CHECK`, `CB_CHECK_EQ`, `CB_CHECK_THROWS_AS(Type, expr)`, a data-file helper, a filter
  `cb_tests --filter formats.pe`, `ctest` integration, XML/JSON result file for the job system). In-tree, ~150
  lines, no gtest download.
- **Cases come from two places.** (a) *Harvested* from the old harness: `expected.py`'s inputs (from the Python
  suites' own tests, so they are the real assertions) and the outputs it recorded, **reviewed and frozen as golden
  data** `native/cb/<area>/testdata/<name>.golden.json` (a human decides each case is meaningful; not a blind
  copy). (b) *Written*: boundary and damage cases for any reader (truncated at every structure, offsets past the
  end, counts of 0xFFFFFFFF), the empty input, and every error type the header promises.
- **Compare normalised**, never textually: `cb/core/semdiff.hpp` (also the `codebreaker semdiff A B` command used by
  agents) compares two JSON documents/line files under these rules: object key order ignored; arrays ordered unless the
  test marks them a *set*; integers equal by value (`10 == 0xa`); floats within 1 ulp unless exact is declared; strings
  exact, except **error messages are not compared** -- only the error *category*, mapped through a fixed table
  (Python `ValueError`/`struct.error`/`IndexError` from a reader -> `FormatError`/`TruncatedError`; `KeyError` ->
  `NotFound`; `OSError` -> `IoError`); paths compared case-insensitively with normalised separators;
  trailing whitespace and CRLF/LF ignored.
- **Intentional divergences are written down, not hidden**: a case that the new module answers differently from the
  Python (a clamped slice that now throws `TruncatedError`; an arithmetic that exceeded int64) is listed in the
  module's test file in a `// DIVERGES:` block naming the old behaviour and why the new one is right. The
  reviewer reads that block first.

### 7.2 Product checks: the real judge

These run `codebreaker.exe` and compare **artifacts**, and they are the acceptance gate for a module (the area's
check, below, must be no worse than before):

| Area | Check | Passes when |
|---|---|---|
| formats, analyze, symbols | `describe` + `inspect` over the **smoke corpus** (about 40 files: lz32.dll, 4 NE, 4 PE, 3 LE/VxD, 3 DOS, a NES/GB/Genesis ROM each, an XEX, a CAB, an ISO) | the facts equal (function starts, names, imports, segments), compared with `semdiff` against the stored baseline |
| decompile, translate | `decompile` of every function of lz32.dll and 3 NE modules; L2 JSON canonicalised | L2 documents equal (same algorithm => same trees); if not, the **verdicts** of the changed functions are equal or better in the judge |
| emulate, verify | `reach --judge` and the judge on the smoke modules | verdict tables equal row for row (`verified`/`weak`/`unverified`/`no-compile` per function), run counts equal |
| build, exact | module build of the 15 MINI.CAB modules (`owbuild`/`buildall`) and the exact-recovery set | linked images **identical bytes** where they were byte-exact before (it is a strong check where it applies, and only a check); the boot gate at milestones |
| java, recompile | the Java conversion of the Minecraft Alpha PC build and the recompile of the Tetris sample | output trees compare equal modulo whitespace and ordering of independent files; the converted project still compiles; recompile `--check` passes |
| app (commands, service, MCP) | the command's documented output on fixed inputs; MCP tool calls through a stdio client | output equal where it is a documented contract; the Dashboard's and Windows95's callers still work |

The smoke corpus and its baselines live in `tests/smoke/` (inputs referenced by corpus name, baselines checked in;
produced once by the current exe, i.e. by the Python-derived behaviour, then maintained by the C++ product).
`tools/cb_smoke` runs an area's check and prints PASS/FAIL per row. **The full regression** (all-15 boot, slow
suites, sweeps) runs at milestones only (a finished wave), not per module -- the standing rule.

### 7.3 What replaces the per-module harnesses

- The `state/work/typed-port/<module>/` harnesses (`expected.py` + `test.cpp` + compile + diff) **stop being
  written**. Existing ones are an *input* (their `expected.txt` is harvested once, 7.1), then archived.
- `tests/*.py` (the Python suites, including `tests/run_all.py`) are ported to C++ by the same fleet in two kinds:
  unit-level asserts join `cb_tests`; command-level ones become `tests/e2e/*.cb-test` (a small file: command
  line, expected stdout/exit code/file hashes, run by `cb_tests --e2e`). Until a suite is ported it keeps running
  as the exe gate (`codebreaker.exe tests\run_all.py --quick`) -- it is the regression net for the transition and
  retires with the Python.
- `docs/TYPED_CPP_SPEED.md` continues as the speed record: each refactor appends its before/after.
- Sanitizer and warnings: the `cb_tests` ASan run and `/W4 /WX` replace what the dynamic runtime's checks (IndexError,
  KeyError, overflow) caught: those were the only memory-safety net, and the explicit bounds checks plus ASan are
  the replacement.

## 8. One program: codebreaker.exe from one CMake target, and the Python's retirement

**Target layout** (`native/CMakeLists.txt`, replacing the pyengine-generated project): static libraries per area
(`cb_core`, `cb_formats`, `cb_analyze`, `cb_decompile`, `cb_translate`, `cb_emulate`, `cb_verify`, `cb_build`,
`cb_exact`, `cb_java`, `cb_app` ...) so incremental builds touch only what changed, linked into **one executable
target `codebreaker`** (`native/cb/app/main.cpp`) and one test target `cb_tests` that links the same libraries.
Vendored third-party code lives in `native/third_party/` (nlohmann, sqlite3, and what section 9 adds) as static libs.
Unity batches stay (they cut the cold build) but are applied per library, off for the test target.
`tools/build_codebreaker_exe.cmd` shrinks to `cmake --build` of this target.

**During the transition** the same executable target also links `pydyn` (the pyengine-generated Python world) and
`pyrt`, behind `-DCB_WITH_PYTHON=ON`: `main()` is C++, parses the command line through `cb::cli`, runs commands
that exist natively, and hands anything else to the translated Python `__main__` exactly as today. Every merge
leaves `codebreaker.exe <command>` working; a command moves from "Python" to "native" in the command table by one
line, which is also how the exe gate stays meaningful.

**Retiring the Python**, in order, each step gated by the product checks:

1. *Third-party packages replaced by native code* (each is a ticket, section 9): `unicorn`, `capstone`, `z3`
   (these are DLLs the bindings call by ctypes today: link them statically/vendored instead of by the Python
   package's folder), `pycdlib` (ISO9660 read/write: small, written in C++), `fontTools` (`font_to_ttf`: a minimal
   TrueType writer), PIL (PNG/BMP via the in-tree zlib), `jsonschema` (a validator for `l2.schema.json` only),
   `rich`/`textual` (the terminal UI), the `mcp` package (JSON-RPC 2.0 over stdio on `cb::Json`).
2. *Every command native* -- the command table has no "Python" entries; `-DCB_WITH_PYTHON=OFF` builds and the exe
   gate (ported suites + smoke corpus) is green.
3. *The tools around it* (`tools/*.py`, `tests/*.py`, the Dashboard's and Windows95's `prepare_game.py`/`place_map.py`
   callers) call `codebreaker.exe` commands or are rewritten as C++ tools/e2e tests.
4. *Delete*: `codebreaker/` (the Python package), `native/pyrt`, `native/pyengine`, `native/pyfront`,
   `native/ported`, `native/typed/cpp`, the ctypes bridges (`formats/native.py`, `engine/native_engine.py`, ...).
   Done in **one branch**, merged once the suite of section 7 is green; the Python stays in git history only.
   `docs/ARCHITECTURE.md`, `docs/NATIVE.md` and the README are updated to say CodeBreaker is C++ (those docs name
   Python throughout and are part of the retirement checklist).
5. *Footprint:* the shipped program is `codebreaker.exe` plus data (`knowledge/`, `platforms` descriptors), no
   Python installation required; the build needs MSVC, CMake, Ninja only.

## 9. Decisions (Logan, 2026-11-11)

1. **Unicorn and Capstone: prebuilt DLLs.** The exe ships the release `unicorn.dll` / `capstone.dll` next to itself
   (in `native/third_party/bin/`), loaded through one loader (`cb::emu::unicorn`, `cb::disasm::capstone`); nothing is
   read from Python's site-packages. Not built from source.
2. **Z3: prebuilt `libz3.dll`**, shipped next to the exe the same way.
3. **Terminal UI stays Python.** `ui/terminal.py` (Textual) is kept as the one Python part; it drives CodeBreaker by
   running `codebreaker.exe` commands (and the MCP/service interfaces), never by importing CodeBreaker's Python
   modules. Everything else is C++.
4. **PCRE2: not now** (only with profile numbers).
5. **`nlohmann` forward declaration: by hand** (no download).
6. **On-disk formats: dropped.** C++-native formats (versioned) replace the Python's pickled caches and repr files;
   existing `state/` caches are rebuilt once. No pickle code in C++.
7. **C++20** for the new tree.
8. **Names:** `native/cb/` and namespace `cb::`.

## Appendix A. Reviewer checklist (a refactor is rejected for any of these)

- `#include "py` or `py::` anywhere in `native/cb` (outside `cb/compat`).
- `std::regex`; a `cb::Regex` built in a function/loop; a regex that is `^[A-Za-z_]\w*$`-style idiom.
- `std::unordered_map`/`unordered_set` iterated into output, a decision, or a file.
- An `int64_t` for an address, offset or size without a reason; signed/unsigned mixing silenced with a cast.
- A binary read without bounds checking; arithmetic on offsets that can overflow.
- `catch (...)` that swallows; an exception type that is not a `cb::Error`; `TypeError`/`AttributeError` equivalents kept.
- A mutable namespace-scope variable, a `static` cache without an owner, a `_reseed` hook.
- `std::shared_ptr` where ownership is not shared; `std::variant<std::string, std::int64_t>` standing in for "a dict value".
- Output built with `repr`/`to_str`/`%`-style helpers; Python float printing.
- Behaviour changes not listed in a `// DIVERGES:` block; golden data copied from the Python without review.
- A shim with logic; a COMPAT line not tied to a named module; a count of COMPAT lines that went up.
- Slower than the port it replaces without an explanation in `TYPED_CPP_SPEED.md`.

## Appendix B. Wave 0 as built (2026-11-11) and the recipe for wave 1

`native/cb/core` exists and `cb_tests` (131 tests, Release and ASan) passes. Build and run: `tools\build_cb.cmd`
(`asan` for the sanitizer build, `-- --filter core.io` for a subset). Adding a module: drop `x.hpp`, `x.cpp`,
`x_test.cpp` into `native/cb/<area>/` -- the CMake globs them; a new area is one `cb_area(<dir>)` line in
`native/cb/CMakeLists.txt`.

**Adjustments to the design, and why**

- **`cb::Regex` is a new engine, not a move of `pyre.cc`.** pyre is welded to the old layer (`py::raise`, `pystr`,
  `value.hpp`, CPython Unicode tables, code-point offsets), so moving it would have dragged `py::` into `cb_core`.
  `native/l2/src/pyre.cc` stays untouched for `py::re` and goes with the layer. The new engine keeps the design
  (compile to a program, explicit backtrack stack, first-byte prefilter) and adds what 3.7 specifies: bytes, ASCII
  classes, step budget (`try_search` returns a `Result`; the throwing forms throw `UnsupportedError`), `${name}`/`$1`
  replacement templates. `\Z` is a ParseError (write `\z`); `$` without Multiline matches only at the very end.
- **`NLOHMANN_DEFINE_TYPE_*` does not work with `cb::Json`** (3.10.5 defines them for `nlohmann::json` only). Use
  `CB_JSON_STRUCT(Type, members...)` from `cb/core/json_struct.hpp` (.cpp files only).
- **Result:** `CB_TRY(expr)` and `CB_TRY_ASSIGN(decl, expr)`; errors keep their dynamic type (`clone()`/`raise()` on
  every `cb::Error` subtype -- a new subtype repeats `CB_ERROR_BOILERPLATE(Type)`).
- **`workers()`** takes a `WorkerPolicy` (settings values) instead of reading a global; env `CB_JOBS`/`CB_LOW_LOAD` win.
- **Not in wave 0:** `cb::paths`, `cb::Progress`, `cb/app/worker`, `semdiff` (wave 1/5, they need the corpus layout,
  the job system, the command table). `cb/app` (the `codebreaker` target and `CB_WITH_PYTHON` hand-off of unknown
  commands to the translated `__main__`) is NOT wired yet: linking `cb_core` into the pyengine-generated project
  would force C++20 and `UNICODE` onto the whole Python-runtime build, so the first shim's own library
  (`pynative_typed`-style, C++20, private) is where `cb_core` joins. The old `tools\build_codebreaker_exe.cmd` is untouched.
  *Wired since:* the generated project (`native/pyengine/src/dyn.cc`, `--dyn-native-typed`) does
  `add_subdirectory(native/cb cb EXCLUDE_FROM_ALL)` before its own compile options, builds `pynative_typed` at C++20
  (py renamed pyt) and links every `CB_AREAS` library into it privately (so C++20 and `UNICODE` stay inside it); the
  whole executable takes the static CRT the cb libraries use. A native module whose port became a header-only shim
  has no `.cpp`: its glue includes the header. `cbtrace.dll` is the `cbtrace` target of `native/cb`
  (`tools\build_cb.cmd`, or `tools\build_native.cmd cb` for that target alone; `state/cxx/cb/bin`, the `tracer_bin` default).
- Third-party headers/DLLs: `native/third_party/README.md` (Capstone is now 5.0.7, not the 4.0.2 the old decompiler
  was written against).

**Recipe for a wave-1 module**

1. Read the old port, the `.py`, the callers; write the header first (no `py::`).
2. `native/cb/<area>/<name>.hpp/.cpp/_test.cpp`; errors from 3.6; text/bytes/containers from section 3.
3. Use `cb::Regex` only after trying code (3.7); `static const cb::Regex re(R"(...)")`.
4. Tests: `CB_TEST("area.module.case", [] { CB_CHECK_EQ(a, b); CB_CHECK_THROWS_AS(Type, expr); })`; use
   `testing::CapturedStd` to assert on stdout/stderr and `fs::ScopedTempDir` for files. A macro argument may contain
   commas (the macros are variadic), but `Type(x);` as a statement is a declaration: write `(void)Type(x)`.
5. Shim at the old header path using `cb/compat/py_bridge.hpp` (`to_py`/`from_py`, `guard`, `rethrow_as_py`);
   its test file builds at /W3 because the old layer is not warning-clean.
6. `tools\build_cb.cmd` and `tools\build_cb.cmd asan`; then the product check of the area.
