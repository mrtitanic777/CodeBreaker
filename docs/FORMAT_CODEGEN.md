# Format descriptions as typed C++ readers (format_gen)

A binary layout is data: `config/formats/<name>.fmt` (the language is described at the top of
`src/formats/format_engine.hpp`). The format engine reads a file through a description into a generic tree of nodes
(each a name, a kind, a type string, an offset, a size, a value, a text and its children), which is what `format_check`,
the probes and the round trip need. As a reader's front end the tree was 4 to 25 times slower than the hand readers it
replaced (a node and a string per field, every name looked up by string at run time), so every reader went back to its
hand-written parsing (51e43e4c6). The rule is never slower.

`format_gen` closes that gap: at build time it turns every description into a C++ header and source, a typed reader
that does what the engine does, field for field, at the cost of a hand reader. A hand reader is switched to its
generated code only when that is proved (below), and then the hand parsing is deleted.

## Where it runs

* `src/formats/format_desc.hpp` holds the parsed description (the engine's own structures: `Format`, `StructDef`,
  `Stmt`, `FieldDef`, `Expr` ...) and `parse_description()`, the engine's parser. The engine and the generator share it:
  there is one parser.
* `src/formats/format_gen_main.cpp` is the generator, `format_gen`, built from `format_engine.cpp` and `cb_core` only
  (not `cb_formats`, which it feeds).
* `src/formats/generate_formats.cmake`, included next to `embed_formats.cmake`, runs it over `config/formats/*.fmt`
  into `<build>/formats_gen/formats/gen/<name>.hpp` and `<name>.cpp` (a file is only rewritten when its text changes,
  so an edit of one description recompiles that reader alone) and adds the sources to `cb_formats`. Code includes
  `"formats/gen/xpr.hpp"`.
* `src/formats/format_gen_runtime.hpp` is the little the generated code shares: the context (the data, the options, the
  format's name for the errors), the loads, the error throwers and the built-in functions (peek, scan, sum8 ...).

## What it writes

Per description `F`, in `namespace cb::formats::gen::F`:

* one C++ struct per description struct reachable from `F` (its own and those of what it `use`s: a used struct is
  generated again in each format that reads it, because the default byte order is the reading format's, elfbe reading
  elf's structures big-endian, and the errors name the reading format).
* a member per field and `let`, in the order of the description:
  * `u8`..`u64`, `i8`..`i64`: `std::uint8_t`..`std::int64_t` (`u24` is `std::uint32_t`); a `let` is `std::int64_t`;
  * `str(n)` and `cstr`: `std::string_view` into the input (the raw bytes, NULs and a cstr's terminator included, as the
    engine's text); `bytes(n)`: `ByteView` into the input;
  * a struct: the generated struct; `T[...]`: `std::vector<T>`;
  * a field that is not read on every path (`if`, inside an `if` block, `try`, `part`) is `std::optional<T>`; a number
    read `try` (on every path) is `std::optional` too, so a reader can tell it was not in the data, while the
    description's expressions see it as 0 (the engine's rule: its tree holds a 0 for it);
  * a name with one type in one branch and another in the other (both numbers: the wider, `std::int64_t` when they
    differ in sign or width) is one member; a name that is a number in one branch and a structure in the other is one
    member per type, `name_Type`.
  * every struct has `_offset` and `_size` (the bytes it covers, as the engine's node).
* `struct Parts` (a `bool` per `part NAME`), `struct Vars` (an `std::int64_t` per `$name`), `struct Options { Parts
  parts; bool all_parts; Vars vars; }`.
* `File read(ByteView data, const Options& = {})` (the root struct at 0) and `T read_T(ByteView data, std::uint64_t at,
  const Options& = {})` for every struct (the engine's `ReadOptions::root` and `::at`).
* `values(data)`: what `read()` gives as (path, value) pairs in the engine tree's terms (a number its value, a str, bytes or
  struct "@offset+size", an array "[count]"), and `registry.cpp`, the table of them. `format_check --gen FILES...` reads
  every file through every generated reader and through the engine and compares the two (the same error class, or the
  same values at the same paths): the generator's own proof, over any corpus, for every description at once
  (formats/format_gen_check; the test formats.format_gen.agrees_with_engine runs it on the test data and on cut and
  damaged copies of it).

## How the engine's behaviour is kept

The generated reader is the engine's `Reader` unrolled for one description; every rule of `read_field` is a branch the
generator either writes or leaves out because the field cannot need it.

* The cursor, `$start`, `$end`, `$remaining`, `$size`, `$index`/`_index`: a struct is a function of `(at, end, index)`
  with its own cursor; a field's elements advance a cursor of their own and the struct's moves when the field is done
  (so `$pos` inside a field's length is where the field starts, and in a `while`/`until` condition it is the cursor
  after the last element, as in the engine). A pointer field (`@ expr`) reads to the end of the data and leaves the
  cursor; `size(n)` limits the field to n bytes and moves the cursor past all of them.
* Errors: a field cut short is `TruncatedError(format, offset, wanted, available)`, a magic that differs, an assertion,
  a negative length, a division by zero, an index outside an array, an unknown name are `FormatError(format, offset,
  ...)` with the engine's words. A damaged count over 2^24 is cut short when the data cannot hold it at a byte an
  element, else a format error; a `while` stops being one at 2^24 elements.
* `try`: any `FormatError` while the field is read (its pointer, size, count, elements, magic) leaves it out; an array
  keeps the elements read before the one that failed; a number that is not in the data reads as 0 without an
  exception; a field of the sequence still takes its place (its `size(n)`, its width, its `str(n)`/`bytes(n)` length).
* `if` on a field and `if` blocks; `part`: read only when the options ask for it; `$name`: the options' variable, 0
  when not given.
* Names. The engine looks a name up at run time: the fields read so far in the struct, then in the struct that reads
  it, and so on up the stack. The generator decides it at compile time. For each struct it knows every place it is
  read from (a site: the encloser and the fields read before that statement) and resolves each name the struct uses but
  does not hold (its free names, and those of the structs it reads in turn) at every site: a field the encloser has
  certainly read is a member access, one it may have read is a test of the optional and then the next encloser, one it
  cannot have read yet is the next encloser. The encloser is a template parameter (`Up`): each site is a small class
  with one accessor per name the struct needs, so elf.fmt's `Header` reads `is64` from `Elf` in elf and from `PlainElf`
  or `Self` in ps3, with no lookup at run time. A name no encloser holds throws the engine's error when (and only when)
  it is evaluated. A struct read inside itself (omf.fmt's iterated-data `Block`) cannot take its encloser as a template
  (the types would never end): it takes it through an interface (`UpBlock`, one virtual accessor per name it needs),
  which each of its sites implements. The types of the free names are known to the generator (the same name must have the same type at
  every site), so member access, indexing, `count()` and `xlat()` are typed C++.
* `fn name(args) = expr` is expanded where it is called, its names resolved in the caller's scope (the engine's rule).
* Expressions are the engine's arithmetic: 64-bit, `+ - *` wrapping, `<< >>` 0 outside 0..63, `&& ||` short-circuit,
  the ternary evaluating one side.

What is left out: the tree itself (offsets of leaves, labels of `as` enums: a reader that wants a label calls the
generated `label_Enum(v)`), detect rules (formats/detect runs them through the engine) and writing.

## Proof of a switch

A hand reader is switched to its generated code only when both hold:

1. `format_check --readers` is identical before and after over the corpora: the main corpus (~27,800 files, the
   synthetic ones among them), the Xbox 360 data (2,512), the CD images (599) and the generated cut and damaged copies in
   `work/unify-fmt/corpus`;
2. `format_check --bench` (built from the snapshot with `work/unify-fmt/bench/apply_bench.py`) shows the generated
   reader no slower than the hand reader (best of the repetitions, the hand reader's build in `work/unify-fmt/build_old`).

Then the hand parsing is deleted: the reader keeps what the format means (a texture's format word, where the pixels
end, which table is which), the layout is the description's.

Order: xpr, xur, xzp, xdbf, mz; then iso, pck, cab, xbe, the ELF family, nro, ps3, vita, orbis; then OMF.
