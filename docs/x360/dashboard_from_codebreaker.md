# The Xbox 360 Dashboard built from CodeBreaker

CodeBreaker holds the one XenonRecomp and the one copy of the runtime libraries the Xbox 360 Dashboard shares with the
ports (owner's decision, 2026-10-23). Until now the dashboard took them from `Dashboard/Shared` (a junction to
`Dashboard/Shared`). This page is what changes on the dashboard's side, and what was checked here first.
Nothing in `Shared-unified`, `Sharedv2`, `Shared-recovered` or the nested XenonRecomp is to be deleted until the
dashboard builds and runs from CodeBreaker and the owner agrees.

## What moved, and where it is

| Shared (Shared-unified) | CodeBreaker |
|---|---|
| `tools/XenonRecomp` (branch `dashboard`, b904d22, patches 0001-0022) | `native/xenonrecomp` -- merged with CodeBreaker's line; every change from upstream in `native/xenonrecomp/CHANGES.md` |
| `tools/build_xenonrecomp.bat` | `python -c "from codebreaker.recompile import xenon; xenon.build()"` (clang-cl and Ninja; builds `native/xenonrecomp/build/XenonRecomp/XenonRecomp.exe`) |
| `tools/ppc_tests` | `native/xenonrecomp/ppc_tests/own` (both files there; `xenon.instruction_tests()` runs them with Xenia's) |
| `tools/scripts/recompile.py <config>` | `python -m codebreaker xenon-recompile <config>` (the same: a clean run, changed files synced, stale ones removed), with `PYTHONPATH=<CodeBreaker>` |
| `tools/scripts/gen_import_stubs.py [--forward]` | `native/x360/tools/gen_import_stubs.py` (the same arguments, `--forward` included) |
| `tools/scripts/xex_basic.py` | `codebreaker.formats.xex.basic(path, out)` |
| `tools/scripts/find_switch_tables.py` | `codebreaker.recompile.switches` (`find`, `write`) |
| `tools/scripts/gen_function_bounds.py` | `codebreaker.recompile.config` (CodeBreaker's own function analysis, no Ghidra); the dashboard's configs keep their generated blocks as they are |
| `tools/scripts/xex_resources.py`, `xex_patch.py`, `xex_info.py` | `python -m codebreaker xex ...` (the native XEX engine: retail, devkit and XEX1 keys tried, as the two XEX1 fixes do) |
| `tools/scripts/stfs_extract.py`, `xzp_extract.py`, `nand_fs_extract.py` | `codebreaker.formats.media` (STFS, XZP and NAND are media volumes) |
| `tools/scripts/xdk_libmatch.py`, `rtti_vtables.py`, `xex_imports.py`, `extract_system_ui.py` | `xdk-match`, `classes`, `xex-imports`, `system-ui` |
| `runtime/{xui,guide,live,stfs,pads,transition}` | `native/x360/system/` -- Shared-unified's at 96547b0, whole |
| `docs/` | `docs/x360/` |
| (the server, already) | `native/x360/server/` (`live_server.py`, `xboxunity.py`) |

Not moved: `references/` (XDK symbols and console captures: Microsoft's, never committed), `ghidra_scripts`
(CodeBreaker does not use Ghidra), `binutils-ppc-cygwin` (a tool: CodeBreaker's setting `ppc_binutils` names it),
the patch files (their history carries session and co-author lines; `CHANGES.md` says what each did), `MESSAGES.md`.

## What was checked here

- XenonRecomp: the instruction tests, 162, 0 mismatches. Tetris, Up and Minecraft (CodeBreaker's) regenerate the same
  except that `db16cyc` now writes `PPC_SPIN_HINT()` (a no-op unless the runtime defines it).
- The dashboard's modules, regenerated from copies of their configurations outside the dashboard's folders:
  - bootanim 8498 (`relocatable_code`): identical, but for the same `PPC_SPIN_HINT` default.
  - The prepared Up (`games/Disney-Pixar Up (disc)`) gains CodeBreaker's fixes, with the same 44,101 functions:
    - 191 functions that fall through into the next continue there, as on the console (those lines are the
      bodies they used to repeat);
    - divides guard against zero and the most negative value over -1;
    - 13 calls to addresses where no function is known go through the indirect-call table (they were dropped).
- `ppc_context.h` is the dashboard's, unchanged: the context layout does not move. CodeBreaker keeps CR, CTR and XER in
  the context by its configuration only; the dashboard's configurations keep them as locals.
- `dash.xex` and `bootanim.xex` were not regenerated here: the master build was running and `SystemUpdate/converted`
  did not hold them at the time.
- The runtime libraries: CodeBreaker's runtime builds and runs Tetris on them (it gained the Guide host hooks the
  Guide / LIVE line calls). The settings they read are `CB_<name>` now, and `MC_<name>` is still read: the dashboard's
  `MC_NO_HID_PADS`, `MC_XUI_EXTRA`, `MC_XUI_PICTURE`, `MC_XUI_ERA`, `MC_XUI_ASSETS` keep working.

## Switching the dashboard over (after its master build)

1. Build CodeBreaker's XenonRecomp: from the CodeBreaker folder,
   `python -c "from codebreaker.recompile import xenon; xenon.build()"`.
2. In `Xbox 360 Dashboard/CMakeLists.txt` and `tools/game_module/CMakeLists.txt`, point the two roots at CodeBreaker:
   - `DASH_SHARED_TOOLS` to `<CodeBreaker>/native` (so `XenonRecomp/...` becomes `xenonrecomp/...`: rename those
     uses), or a new `DASH_XENONRECOMP` = `<CodeBreaker>/native/xenonrecomp` for the `thirdparty/simde`, `XenonUtils`
     and `XenonUtils/xbox/*.inc` uses;
   - `DASH_SHARED_RUNTIME` to `<CodeBreaker>/native/x360/system` (and the six `include(".../runtime/<lib>/<lib>.cmake")`
     in `DashRecomp/CMakeLists.txt` to `${DASH_SHARED_RUNTIME}/<lib>/<lib>.cmake`);
   - `gen_import_stubs.py` to `<CodeBreaker>/native/x360/tools/gen_import_stubs.py`.
3. The module builds: each `python -I ../Shared/tools/scripts/recompile.py <config>` becomes
   `python -m codebreaker xenon-recompile <config>` with `PYTHONPATH=<CodeBreaker>`; `build_xenonrecomp.bat` becomes
   step 1; `prepare_unified.py` and `prepare_module.py` call `codebreaker.recompile.xenon.recompile_synced(config)`.
4. Recompile every module once and build: the generated code changes only where listed above, so expect the
   PPC_SPIN_HINT default everywhere and CodeBreaker's fixes in the games.
5. Run the dashboard (the owner runs it): boot, the Guide (sign in, Create Profile, Join Xbox Live), a game launch and
   back, the bootanim versions.
6. When all of that holds, the owner decides about `Shared`, `Shared-unified`, `Sharedv2` and `Shared-recovered`.
