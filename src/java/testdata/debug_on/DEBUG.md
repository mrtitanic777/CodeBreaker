# DEBUG.md -- the single debug file

All debug instrumentation for the port lives behind one switch: `DEBUG_ENABLED` in `Debug.h`.

- **Retail build:** set `DEBUG_ENABLED 0` (or define it in the project's Release config). Every `DBG_*`
  macro compiles to `((void)0)`, so all debug code across the codebase goes dark from this one place.
- **Debug build:** leave it `1`. Toggle individual channels with `DBG_CAT_BUG/LEAK/CRASH/NET/PERF`.

## Rules (so it never becomes console spam)

| Need | Macro | Behavior |
|---|---|---|
| Mark a spot hit once | `DBG_ONCE(DBGC_BUG, DBG_CAT_BUG, "...")` | prints once per call site |
| High-frequency pulse | `DBG_EVERY(DBGC_PERF, DBG_CAT_PERF, 600, "...")` | every 600th hit |
| Per-frame condition | `DBG_THROTTLE(DBGC_NET, DBG_CAT_NET, 1000, "...")` | at most once/second/site |
| Invariant | `DBG_ASSERT(ptr != 0, "null chunk")` | BUG channel, does not abort |
| Recovered error | `DBG_CRASH_SOFT("bad id %d", id)` | one line |
| Fatal | `DBG_CRASH_HARD("heap corrupt")` | logs + summary + break |
| Allocation | `DBG_LEAK_NEW(p, size)` / `DBG_LEAK_DELETE(p)` | accumulated, not per-line |
| Networking | `DBG_NET(lost, total, rttMs)` | silent until >=5% loss burst |
| Dump totals | `DBG_SUMMARY()` | mem live/peak + net loss + live allocs |

Leaks and packet loss are **accumulated and summarized** (`DBG_SUMMARY()` at shutdown / on a key),
never a line per event -- so the console stays readable and a real leak stands out.

Do **not** add raw `printf` / `OutputDebugString` debug code elsewhere; route it through a `DBG_*`
macro so the master switch governs it. The converter flags stray debug output as
`J2X-REVIEW[debug-bypass]`.
