# Archive

Code moved out of the active `src/` package because nothing in it is imported by the rest of
CodeBreaker any more. It is kept, with its history, for reference and so it can be revived.
Nothing here is built, tested or dispatched by `wt`.

| Path | Was | Why archived |
|---|---|---|
| `engines/recompile/` | `src/engines/recompile`, the `wt recompile` subsystem | The GCC-based compile/link lane for lifted C. Superseded by the OpenWatcom lane (`owbuild`, `owlink`, `bodyverify`). `tools/usage_map.py` showed no importers. |

To revive something: `git mv` it back under `src/`, restore its entry in `wt.py`, and fix its imports.
