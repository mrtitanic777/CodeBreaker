# Batch 7: judge-c-java's Python, the module_writer closure, and the Java world's holders

Roots, scripts and conditions as in batches 3-6. New scripts in `python_reach/`: `g7.py` (the import graph of codebreaker/tests/tools from the source, static imports plus string-literal module names, the platform descriptors as roots,
`java.projects.<x>` loaded by name; `g7.py free [globs of files to treat as deleted]` = modules unreachable from tests/tools/`__main__`/ui/descriptors), `del7.sh` (count and `git rm`), `fn_users.py` (test functions that mention a word),
`twin7.py` (a name-similarity hint between Python test cases and native CB_TESTs; a hint, not proof). `cutfuncs.py` as before.
`tools/check_imports.py`: 210 modules, 0 failing, 0 dangling (239 before the batch). Python under `codebreaker/` now: 210 modules, 59,201 lines.

## Group 1: judge-c-java is native (`6310 lines`, 8 modules + 1 tool + 3 tests)

Deleted: `tools/judge_c_java.py`; `translate/{backends/{java,java_objects,java_libc},frontends/clang/{__init__,ctypes,lower,j2x},judge_native_jvm}`;
the tests `translate/{test_java_backend,test_clang_frontend,test_j2x}`. (`judge-c-java` is in `NATIVE` and registered natively.)
Left: `codebreaker/translate/frontends/clang/corpus/*` (data: the programs the old judge read; no Python reads them now, no native reads them either; a later tidy can drop it).

| Python case | native test |
|---|---|
| test_java_backend faithful / idiomatic tier, cpp classes, goto and varargs, refusals say why, literals and names | `translate.java.golden.*` (FaithfulCore, FaithfulArith, FaithfulVa, FaithfulOverloads, GapGoto: goldens the Python wrote, each judged beside javac/JVM), `translate.java.tier_choice`, `analysis_reasons`, `errors`, `jname`, `*_literals`; `translate.java_objects.*`, `translate.java_libc.*` |
| test_clang_frontend (declarators, layouts, C declarations and bodies, C unit is L2, C++ classes and bodies) | `translate.clang.ctypes.{parse_and_spell,tokens,layouts_and_decay}`, `translate.clang.lower.{programs_give_the_reviewed_documents,the_unit_says_what_it_is,...}` (the clang trees and the Python's L2 documents are checked in) |
| test_j2x `types_in_lists_are_lifted` | `translate.j2x.types_in_lists_are_lifted` (+ the six-class round trip `the_native_round_trip_judge_gives_the_python_verdict`) |

## Group 2: the module_writer closure and the XENIX C toolchain (21 modules, 7,904 lines)

Deleted: `source/{module_writer,java_writer,classes_writer}`, `source/ne_asm/{segment,fixups}`, `build/module`, `symbols/names`, `verify/{reconstruct,teeth}`, `verify/gate16/{__init__,setup16,isolated16,contract16}`, `engine/{lanes,win16}`, `base/{runs,ctext}`,
`exact/{toolchain,xenix_c,emu/xenix}` (held only by `build/period`, which batch boot deleted), `invert/__init__` (the empty package).
Tests deleted: `test_whole_module.py`, `test_platforms.py` (its rows are `platforms.*` natively). Cut: `test_engine` (the made-up lane on the lanes' steps), `test_symbols` (names of a module, source files from symbol files),
`test_verification` (5 cases: incremental reconstruct, verdict holds until its inputs change, the teeth's breaks, the call contract's count, the setup screens; the reconstruct half of `a_verdict_is_for_the_compiled_code`),
`test_verdicts` (the `reconstruct.GOOD` line), `test_plumbing` (run folders), `translate/test_c_backend` (two `build/module` document cases), `java/test_kind` (kind_name and the reconstruct+write case; the lane's three stay), `test_ui` (the digest comes from a constant now:
the native service reads the store under its own digests, which already differed from the Python's).
Wiring changed: `__main__.COMMANDS` loses `module scoreboard teeth names build` (all five are in `NATIVE`; the plain-python fallback rows are gone, as batch boot did for `match repair score16`); `decompile/describe.py` takes `powerpc_names` (15 lines, the one thing it
used from `symbols/names`); the platform descriptors lose the `packager.link` and `emulator.call` Refs (their only caller was `build/module`) and `steps` (the engine's lanes' steps were `engine/lanes`; no descriptor has steps now, so `platforms.step_modules()` is empty);
`tests/run_all.py` loses the two deleted suites. tidy removed what that orphaned (10 definitions). The platform descriptors stay: `platforms/__init__` is imported by 9 modules that stay and loads them by file name.

| Python case | native test |
|---|---|
| test_whole_module (resources 32/16, round trips, definitions, data objects, 32/16-bit module, flat thunk) | `build.resources.*`, `build.definitions.*`, `build.data.*`, `build.module.*`, `build.flat_thunk.*` (batch boot's by-name map) |
| test_symbols names of a module, source files | `symbols.names*`, `symbols.debug_build*` |
| test_engine `a_lane_on_the_lanes_steps` | `engine.lanes.*`, `engine.lanes_wiring.*` |
| test_verification `reconstruct_is_incremental`, `a_verdict_holds_until_...changes`, `a_verdict_is_for_the_compiled_code` (signatures) | `verify.reconstruct.{a_run_judges_and_records,a_function_judged_for_its_text_is_not_built_again,signatures_follow_what_a_function_reaches,signatures_of_a_ring_are_one,signatures_of_what_was_built_ignore_the_source,signatures_of_a_module_without_texts_are_the_whole}` |
| `breaks_change_what_a_function_does` | `verify.teeth.{constant,comparison,operator,store,arguments,whole_function,nothing_to_break}` |
| `a_call_s_arguments_are_what_its_callee_removes_of_what_was_pushed` | `verify.gate16.contract16.a_calls_arguments_are_what_its_callee_removes_of_what_was_pushed` |
| `a_screen_shorter_than_the_pages_points_is_not_that_page` | `verify.gate16.setup16` (`is_license`: 640x350 / 720x480 / one pixel wrong), `verify.gate16.screen.*` |
| test_plumbing `run_folders_are_deleted` | `base.runs.{start_makes_a_named_folder_under_the_tools_runs,folder_is_deleted_at_scope_end_even_by_an_exception,sweep_removes_only_old_run_folders}` |
| test_c_backend `a_module_as_one_tree_with_what_stayed_assembly`, `a_32_bit_module_as_one_tree` | `build.module.document16_*`, `document32_*` |
| test_kind `a_folder_of_java_is_a_java_module`, `classes_are_judged_filed_and_written` | `verify.lanes.java.*`, `verify.reconstruct.a_per_function_kind_keeps_the_verdict_when_the_code_is_the_same`, `java.pipeline.*` |
| exact/xenix_c, toolchain (no Python test) | `exact.xenix_c.*`, `exact.toolchain.*` |

## Group 3: translate/javajudge, backends/jmodel: ported what had no twin, NOT deleted (see below)

The two `test_cpp17_backend` cases with no native twin are ported, in `native/cb/translate/java_ports_test.cpp`:

- `java.cpp17.the_program_runs_as_the_jvm_runs_it`: Modern.java (records, sealed interface and patterns, switch expression, string switch, lambdas, streams, a map) through the real front end (TreeDump, javac), the C++17 back end's whole program (`cpp17::program`), built with g++ against the C++17 runtime, run beside the JVM; stdout and the exit code must equal the JVM's, and the program must hold the C++17 idioms (`jrt::fn<Function>(`, `jrt::each<int32_t>(xs)`, `[&]() -> ref<String> {`, `std::make_shared<Modern_Point>(`). Needs a JDK, g++ and cbl2.dll (it says so and passes without them).
- `java.cpp17.inside_the_transpiler_beside_cpp03`: Tally.java converted as C++17 and as C++03: range-for, switch expression as a lambda, `auto* t = new Tally`, no `J2X-GAP` in either, and the C++17 text compiles with `-fsyntax-only`.
  (`jpo_convert` gained the `cpp_std` argument.)

Both ran here and pass (12 checks). `javajudge.py`, `jmodel.py` and `test_cpp17_backend.py` are NOT deleted: the file's other 5 cases (records' implicit members, instanceof bindings, the object model, the program reads as C++17, desugar) read the same Modern.java units through `javajudge.lower_program`/`jmodel`/`desugar`/`ir`,
and their native twins are by name only (`translate.jmodel.*`, `translate.desugar.*`, `translate.cpp17.*`), not a case-by-case check, so the file stays with the Java world below. When that world goes, these go with it.

## Group 4: java/* and java/projects/minecraft: nothing deleted, and why (measured this time)

`g7.py free 'tests/java/*'` (all 51 files of the folder gone) frees ONE module (`java/projects/minecraft/ui_mark`, 153 lines): the Java package is held by more than its own folder. Holders of `java/transpile`, `java/parse`, `java/probe`, `java/ir`, `java/emit`, `java/projects`, `java/rules` outside `tests/java`:
`tests/translate/{test_cpp03_bodies,test_cpp17_backend,test_port_ownership,test_port_semantics,test_console_patches}`, `tests/test_port_fixes` (`java.rules`), `verify/lanes/java` (the platform descriptor's lane, and the 3 cases of `tests/java/test_kind`),
`translate/backends/cpp03` (-> `java/ir`, `java/emit`), `translate/javajudge`. So the Python Java world is one component held by about 57 test files (about 7,900 lines of test, 14,657 lines of `java/*`, plus cpp03/cpp17/javajudge/jmodel/desugar/rewrite/verify.lanes.java).
It can only go as a whole, and it needs every one of those cases mapped to a native case. `twin7.py` (a name hint) over `tests/java/*`: 328 cases, 214 with a native case of nearly the same name, **114 with no obvious twin**; the list is the work for the next batch (it is `python state/work/typed-port/python_reach/twin7.py tests/java/test_*.py`):
the weak ones cluster in `test_rules` (8 of 13; native `java.rules.*`), `test_types` (6 of 19), `test_controller_nav` (7 of 11), `test_reflection_factory` (4 of 7), `test_runnable_entry` (4), `test_runtime_stubs` (4), `test_host_compile_fixes` (4), `test_edition_resolve` (4) and 20-odd single cases. Many will be naming differences
(the native `java.rules`, `java.types`, `java.minecraft.*` areas exist), but a name is not a proof, so none of it was deleted. The Minecraft project (`java/projects/minecraft/**`) stays with it: it is loaded by name from `java/projects/__init__` and chosen by 16 of the tests above.
Native twins of the modules exist (`native/cb/java/**`); condition 5 (all-15 boot) does not touch any of this Python.

## The five conditions, for the 29 modules of groups 1 and 2

1. Their C++ is committed as source: `native/cb/translate/{backends/java*,frontends/clang,judge_native_jvm,judge_c_java_command}`; `native/cb/{source/module_writer*,build/module*,symbols/names*,verify/{reconstruct,teeth},engine/lanes*,verify/gate16/*,base/runs,exact/{xenix_c,toolchain}}` (batch boot section 1 traced the boot closure through them).
2. Their tests pass as C++: `area translate` (below) and the by-name map above (batch boot ran `area source`; batch 6 `area translate`/`exact`).
3. The judge traces agree: these are authoring and verification tools, with the Python-made goldens in the native tests as the evidence (as batch boot said).
4. Every caller runs through codebreaker.exe: `module scoreboard teeth names build judge-c-java` are in `NATIVE`; their COMMANDS rows are gone; `check_imports` 0 dangling; no tool, test, or outside repository imports a deleted module (searched `native tools tests data docs` for the file names and module paths: only comments and the retired plain-python rows).
5. All-15 boot: not run here (integrator's, on master). The boot path reaches none of these (batch boot section 1).

## What was run

- `tools/check_imports.py` after each group, plain python: 210 modules, 0 failing, 0 dangling. `tools/tidy.py --apply`: clean (the one remaining report, `ui/terminal.py:718 undefined name 'e'`, was there before and is in the kept UI). `tools/unreachable.py`: 0 modules unreachable.
- Plain python, the edited suites: `test_verification`, `test_verdicts`, `test_symbols`, `test_plumbing`, `test_engine`, `test_ui`, `tests/java/test_kind`, `tests/translate/test_c_backend`: every failure is "cbengine.dll / cbl2 not built" (the worktree has no native DLLs); no import or name error.
- Native, `tools\build_cb.cmd area translate` (CB_BUILD_JOBS=3, CB_L2_DLL = master's cbl2.dll): `java.cpp17.*` 2 passed (12 checks); the whole area: see the report.
