# XEXMagic 0.1.0

Reverse-engineering tool for Xbox 360 XEX2 images and PPC COFF archives:
loader, PPC disassembler (whole ISA including VMX128), decompiler, static
recompiler, a PPC interpreter, SDK type and symbol matching, and a browser UI.

## Build

```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Output: `build/bin/xexmagic.exe`. `extra_seeds.txt` and `rt360_runtime.h` are
staged next to it automatically — the tool reads both at runtime.

Open `build/XEXMagic.sln` to work in Visual Studio; `xexmagic` is the startup
project.

**Adding a `.cc` means re-running the configure step**, otherwise the generated
project will not contain it.

## Use

```
xexmagic.exe --serve --port 8360 <image.xex>     # browser UI
xexmagic.exe --serve --port 8360                 # UI with a drop target
xexmagic.exe --func 0x82345678 --pseudo <image>  # decompile one function
xexmagic.exe --func 0x82345678 --disasm <image>  # disassemble one function
```

Addresses need the `0x` prefix — without it the argument is parsed as decimal
and silently resolves to the wrong place.

A `.lib` works anywhere an image does: the archive is laid out into one flat
address space and served through the same UI.

The port comes up immediately and the page shows what the load is doing —
phase, percentage, and the functions being decompiled right now — refreshing
every five seconds. With no file on the command line the page is a drop target;
dropping a XEX, EXE or LIB opens it.

The analysis runs across every core. On a stock XDK library (13,958 functions)
a cold start is about a minute; after that it is seconds, because the result is
cached (below).

## Where things are stored

Everything the tool writes lives under `%APPDATA%\XEXMagic`, never beside the
image — an SDK under Program Files is not writable and a build directory is not
permanent.

| Path | Contents |
|---|---|
| `xexmagic.json` | SDK path and settings |
| `db\<HASH>\image.xdb` | your renames, types, signatures and notes |
| `db\<HASH>\image.xdb.analysis` | the saved analysis, so a re-open is instant |
| `images\<HASH>\` | files dropped on the UI, kept under their content hash |
| `Libraries.XDG` | the indexed SDK libraries: symbols, demangled names, code signatures, inline windows |

`<HASH>` is the image's content hash, so two different `default.xex` files keep
separate databases instead of overwriting each other.

Caches carry the XEXMagic build they were written by and are rebuilt after an
upgrade — a newer decompiler produces different output from the same input, so
an old cache is stale by definition. The `.xdb` is yours and is never discarded;
derived names (SDK matches, import shims) are kept in the analysis cache
instead, so they can never be confused with something you named.

## Imports

The **Imports** tab searches the complete `xam` and `xboxkrnl` ordinal tables by
name, module or ordinal — not only what the open image imports. Entries the
image does import show their thunk address and call count, and expand to the
functions that call them. Calls that reach an import indirectly, by loading its
value slot, are counted too.

A function whose whole body is "call one import and return" is named
`Call_<Ordinal>` automatically.

## Export

**Export C** writes the whole image as one C file — every function through the
same renderer the UI uses, so renames, types, signatures and notes are all in
it. It streams to the browser as a download and a copy is written to disk, so a
browser that gives up does not throw the work away.

## Xbox 360 SDK

The **SDK** button in the UI takes a path to the installed SDK (the folder with
`include\` and `lib\`), remembers it in `%APPDATA%\XEXMagic\xexmagic.json`, and
indexes it: on a stock XDK, 867 types from 468 headers, 6,472 prototypes, and
444,532 library symbols with 231,549 code signatures from 132 libs. Headers are
parsed across every core; the library half is kept in `Libraries.XDG` and
reloaded rather than parsed again.

Once configured, the scan runs automatically the first time an image is
analysed — the names are in place before anything else looks at the image. It
is skipped for a `.lib` input, where matching the SDK against itself would only
rediscover the archive.

What it buys:

- **Type names for inferred structs.** The decompiler recovers offsets and sizes
  but never names. `GET /api/sdk/match?size=64&offsets=0,8,48` returns the SDK
  types that explain that layout — for those numbers, `XSESSION_INFO`.
  A candidate is only offered when *every* observed offset lands on a declared
  field; a struct that explains half the accesses is the wrong struct.
- **Real names for statically linked SDK code.** *Scan image* compares each
  unnamed function against the library bodies. Linking rewrites the fields the
  archive marks relocatable and nothing else, so those are masked and the rest
  must match byte-for-byte. On MW3 that names ~590 functions (D3D, XAM, XNet…).
  Bodies under 32 bytes are skipped as indistinctive, and anything matching two
  different names is reported as ambiguous rather than guessed.

- **Inlined library code.** *Find inlined* catches what whole-function matching
  cannot: a library body the compiler spliced into its caller. Eight-instruction
  windows from every library body are indexed, windows shared by more than one
  routine are discarded, and matching runs are extended and reported longest
  first. On MW3 that finds ~1500 regions across ~865 functions in under a
  second — `atan2`, `qsort`, `XContentCreateEx`, D3D and XACT helpers.
  These are **annotations, never renames**: the host function *contains* the
  code, it is not that function.

Endpoints: `GET/POST /api/sdk`, `POST /api/sdk/index`, `POST /api/sdk/scan`
(`?apply=1` to rename), `POST /api/sdk/inlined`, `GET /api/sdk/structs?q=`,
`/api/sdk/symbols?q=`, `/api/sdk/match?size=&offsets=`.

Windows paths in JSON need escaped backslashes (`C:\\XDK`); forward slashes work
too and are easier to type.

## Layout

| Path | Contents |
|---|---|
| `src/` | all tool sources |
| `third_party/mspack` | LZX decompression (XEX2 images, IW5 fastfiles) |
| `third_party/crypto` | AES/Rijndael for XEX2 key decryption |
| `test/` | `test_il` unit target (`ctest` or run the exe) |

Source groups in the solution mirror the roles: Container, Analysis, Codegen,
Symbols, Interpreter, Web UI, App.

## Notes

`src/rt360_runtime.h` is **not compiled into the tool**. It is the ABI header
that statically recompiled output `#include`s, so it ships as data next to the
binary.

`web_server.cc` embeds the UI as string literals; MSVC caps a single literal at
16380 chars, so the HTML is split across several and the target builds with
`/bigobj`. Split again if it ever errors with C2026.
