# Trash

Code moved out of the active `src/` package because nothing can reach it any more. It is kept,
with its history, so it can be read or revived. Nothing here is built, tested or run by `wt`.

A module lands here when `python tools/dead_code.py` puts it in one of two classes:

- **broken** — it cannot run: it imports a module that no longer exists
- **no entry** — nothing refers to it and it has no `__main__` block, so nothing can start it

Scripts that are merely unreferenced but runnable by name (`wt --target win95 <name>`) are
commands, not trash, and stay in `src/`.

Paths mirror where the file lived under `src/`.

| Path | Why it is here |
|---|---|
| `engines/recompile/` | The GCC-based compile/link lane for lifted C (`wt recompile`). Superseded by the OpenWatcom lane (`owbuild`, `owlink`, `bodyverify`); no importers. |
| `targets/win3.1/kwajwrap.py` | Broken: imports `mscomp`, which no longer exists. |
| `xbox/xbt/crypto/xcp_dumper.py`, `kv_checker.py` | Broken: import `StreamIO`, which no longer exists. |
| `xbox/xbt/xbox/stfs2.py`, `kv_extract.py`, `xex_deep.py`, `debug_monitor.py`, `sysextract.py`, `systemlink.py`, `key_scanner.py`, `memdiff.py` | No caller and no entry point. |
| `xbox/xbt/core/transport.py` | No caller and no entry point. |
| `xbox/xbt/ppc/_regen_tetris_src.py` | No caller and no entry point (underscore files are not commands). |

To revive something: `git mv` it back to the same path under `src/` and fix its imports.
