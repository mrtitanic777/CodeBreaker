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

On CodeBreaker's path a folder of Java source is a module of the `java` kind (verify/lanes/java.py): its
classes are its functions, each is compiled on its own with the host compiler, verdicts go to the
store, and `codebreaker module FOLDER` writes the C++ with each class's status line and STATUS.md.
A class that compiles is then run beside the JVM where it can be (codebreaker/java/probe.py): its
methods with primitive parameters and results (instance ones on an object made with a constructor that
takes primitives), on the same inputs, results compared exactly. What it found is fixed in the transpiler:
Math.PI as a float, (int) of a float not saturating, an empty array taken for null, a null dereference in
a try not reaching its catch, char signed. Java's char is unsigned 8-bit here (strings are 8-bit): a char
past 255 cannot be held, and the judge shows it.
The support headers are precompiled once per module, so a class compiles in about a fifth of the time.

Conversions in one process run one at a time (transpile.convert holds a lock): the target profile
and the per-corpus tables are module-level. Side-by-side work runs in separate processes, as
every CodeBreaker job does.

## The tools

`python -m codebreaker java <tool>`; `--help` on each gives its options.

| Tool | What it does |
|---|---|
| `convert` | a Java source tree as C++ (`--platform`, `--cpp-std`, `--mc-version` to decompile first) |
| `decompile` | a Minecraft version as Java source, through RetroMCP's engine (`native/retromcp`) |
| `report`, `worklist` | the crash-class report; compile the output and list the files that fail |
| `signatures` | signatures.md: every class's fields and methods, Java type to C++ type |
| `scan-project`, `scaffold`, `apimap` | one-folder project resolver; skeletons in other languages; the API-mapping registry |
| `assets`, `xact`, `achievements` | textures and resources from the game jar; XACT3 audio banks; achievements to XLAST + unlock seam |
| `platform-config`, `sdk`, `debug` | controller mapping and kernel calls; find the platform SDK; the one toggleable debug file |
| `project-layout`, `vcxproj`, `build` | the Visual Studio layout and file list; build the solution with MSBuild |
| `reflect-factories` | Java reflection registries as generated C++ factories |
| `ui-mark`, `keybind-mark`, `bugfix-ledger` | greppable markers and their indexes; the catalogue of FIXED BUG markers |
| `sound-fix`, `controller-nav`, `render-fix`, `applet-ping` | the console fixes on converted Minecraft (run on every xbox360 convert) |

## What is not in the repository

No game code (no decompiled `net.minecraft.*` tree), no game assets and no platform-SDK headers,
libraries or documentation. The transpiler works on Java source you supply, the asset tools on
files you own, and the console build on an SDK installed on this machine (settings.json
`xbox360_sdk`); those inputs live in the gitignored `binaries/java/` (`original_java`,
`PlatformSDKs`, `overlays`). Third-party class and API names appear as data, so they can be
recognised and translated.
