# The Java transpiler in CodeBreaker

J2CTool, the deterministic Java -> C++03 transpiler, is `codebreaker/java` since its merge
(history kept: `git log j2ctool/release -- j2ctool/<file>` and `git blame` reach its commits).

| J2CTool | Here |
|---|---|
| `j2ctool/pipeline.py` | `codebreaker/java/transpile.py` |
| the core (`parse`, `ir`, `types`, `typemap`, `emit/`, `rules`, `runtime`, `profiles/`) | `codebreaker/java/` |
| the Xbox 360 tooling (`assets`, `xact`, `achievements`, `platform_*`, `sdk`, `vcxproj`, the rewrites) | `codebreaker/java/console/` |
| `j2ctool/decompile.py` | `codebreaker/decompile/jar.py` |
| `docs/platform/{canonical,reference,companions,assets}` | `codebreaker/java/platform/` |
| `docs/platform/notes`, the XDK index | `docs/java/platform-notes`, `docs/java/xdk` |
| `apimap/` | `codebreaker/java/api_maps/` |
| `python -m j2ctool <tool>` | `python -m codebreaker java <tool>` |
| the PySide6 window | the web console (`serve`) and the workbench: "Java to C++", "Java tools" |
| its Build button | `codebreaker java build SLN` (settings.json `msbuild`) |
| environment variables and fixed paths (XEDK, JAVA_HOME, J2X_CC, ...) | settings.json: `xbox360_sdk`, `java`, `retromcp`, `host_cxx`, `msbuild`, `java_overlays` |
| `BUGFIX_LEDGER.md` in the checkout | `state/java/BUGFIX_LEDGER.md` |
| `tests/` (pytest) | `tests/java/`, run by `tests/test_java.py` in the gate |

On CodeBreaker's path a folder of Java source is a module of the `java` kind (verify/java.py): its
classes are its functions, each is compiled on its own with the host compiler, verdicts go to the
store, and `codebreaker module FOLDER` writes the C++ with each class's status line and STATUS.md.
A class that compiles is then run beside the JVM where it can be (codebreaker/java/probe.py): its
static methods with primitive parameters and results, on the same inputs, results compared exactly.
The support headers are precompiled once per module, so a class compiles in about a fifth of the time.

Conversions in one process run one at a time (transpile.convert holds a lock): the target profile
and the per-corpus tables are module-level. Side-by-side work runs in separate processes, as
every CodeBreaker job does.
