# j2ctool — a deterministic Java → C++03 transpiler

`j2ctool` turns Java source into portable **C++03**. It is a source-to-source transpiler built to be
**correct by construction**: the crash classes a naive port hits — uninitialized arrays,
Rule-of-Three double-frees, null-deref virtual calls, dropped constructor registration,
out-of-range array access, static-init-order faults — are prevented in the generator, not patched
afterward. Every run is deterministic: same input, same output, byte for byte.

It ships in two layers:

- a **generic transpiler core** — `parse → IR → type/ownership resolution → emit → pipeline`, a
  structural crash-class **rule engine**, and a structured **event ledger**; and
- a **Minecraft / Xbox-360 profile** — the game- and platform-specific tooling layered on top:
  asset extraction, XACT audio banks, achievements mapping, the hand-written 360 platform
  reference layer, controller/keyboard config, and Visual Studio project scaffolding.

> **Bring your own source and SDK.** This repository contains **no** game code, **no** game
> assets, and **no** platform-SDK content. You supply the Java source to transpile and any target
> SDK needed to build the result. See [`NOTICE.md`](NOTICE.md).

## Install

The core transpiler and CLI need only the Python **standard library** (Python ≥ 3.6). Two optional
dependencies unlock extra features and degrade gracefully when absent:

| Package | Enables | Without it |
|---------|---------|-----------|
| `PySide6` | the desktop GUI (default entry point) | use the CLI |
| `Pillow`  | texture/asset extraction, achievement icons (`j2ctool assets`, `j2ctool achievements`) | those steps warn and skip |

```bash
# editable install — gives you the `j2ctool` command while keeping the repo's data paths working
pip install -e ".[full]"     # or ".[gui]", ".[assets]", or just "-e ." for the stdlib-only core
```

You can also run it without installing, straight from the repo root: `python -m j2ctool <command>`.

## Usage

Running `j2ctool` with **no arguments launches the GUI** — a wizard that walks through platform,
language, paths, and each tool step. Everything the GUI does is also available on the CLI:

```bash
j2ctool convert     <java_dir> <out_dir>   # transpile Java -> C++ (--platform, --cpp-std, --platform-refs, --sdk)
j2ctool report      <out_dir>              # crash-class report from emitted output
j2ctool signatures  <java_dir>             # codebase map: class -> fields/methods, Java type -> C++ type
j2ctool worklist    <out_dir>              # compile the output, write a failing-file worklist
j2ctool vcxproj                            # (re)generate the Visual Studio .vcxproj file list + filters

# Minecraft/Xbox-360 profile tooling
j2ctool assets          <jar> <out>        # extract textures/resources, normalize PNG -> RGBA
j2ctool xact            <sub> ...          # XACT3 audio banks: dump / build / coverage / match
j2ctool achievements    <java_dir>         # map achievements -> Xbox 360 XLAST/SPA + unlock seam
j2ctool platform-config <...>              # controller mapping, keyboard/storage kernel calls
j2ctool ui-mark         <tree>             # tag UI functions with greppable markers + UI_INDEX
j2ctool keybind-mark    <tree>             # tag hardcoded key-press sites + KEYBIND_INDEX
j2ctool bugfix-ledger   <tree>             # catalog FIXED BUG / J2X-BUGFIX markers
j2ctool reflect-factories <tree>           # rewrite Java reflection registries -> C++ factories
j2ctool debug           <...>              # emit a single toggleable Debug.{h,cpp}
j2ctool sdk             <...>              # detect / gatekeep the platform SDK
j2ctool project-layout  <tree>             # impose the VS solution layout on a flat output tree
j2ctool gui                                # launch the GUI explicitly

# tests
python -m pytest -q
```

`convert` automatically runs the reflection-factory, keybind-marking, and bugfix-ledger passes as
part of the pipeline.

## Correct by construction

Durable behavior lives in the generator, so it survives a re-convert; there is no post-hoc
patch directory. The type model is uniform: every Java class is a reference (a C++ pointer); only
primitives, `String` (→ `JavaString`), and containers are held by value; arrays become `JArray<T>`.
Fixes are made in one of two places:

- **Generated code** → a rule / emit / type change under `j2ctool/` (e.g. signed-`byte` scalars,
  null-deref guards, ctor delegation, Rule-of-Three, static-init ordering).
- **Platform glue** the transpiler does not generate → the hand-written
  `docs/platform/reference/*.reference.{h,cpp}` layer, emitted in place of stubs via `--platform-refs`.

## Layout

| Path | What |
|------|------|
| `j2ctool/` | The transpiler package. Core: `ir`, `parse`, `types`/`typemap`, `emit/`, `pipeline`, `rules`, `runtime`, `ledger`/`report`. Profile tooling: `assets`, `xact`, `achievements`, `platform_*`, `keybind_mark`, `ui_mark`, `signatures`, `vcxproj`, `project_*`, `cli`, `gui`. |
| `tests/` | `pytest` suite. `tests/fixtures/java/` is a small **synthetic** (non-game) Java corpus used by the parser/emitter tests. |
| `profiles/` | Conversion profiles (type/ownership defaults, skip lists). |
| `docs/platform/` | The Xbox-360 platform layer: `notes/` (architecture write-ups), `reference/` + `companions/` (hand-written glue), `canonical/` (Java-stdlib reimplementations). |
| `tools/` | Auxiliary developer scripts. |

Generated output (`out/`, `build/`, `dist/`, …) and any locally-supplied game source or SDK are
git-ignored — regenerated or user-provided, never committed.
