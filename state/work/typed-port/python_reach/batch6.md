# Batch 6: dev tools, the C# / Python-front-end / C-for-Java translators, and the exact/ draft and plug-ins

Roots, scripts and conditions as in batches 3-5. New scripts in `python_reach/`: `pool6.py` (pool after the retired tools), `keep6.py` (closure of what stays: the boot closure `verify/{authored,gate16}`, `verify/lanes/powerpc`, `source/module_writer`, `ui`, the live roots, and every tool still used),
`free6.py <prefixes>` (what of a package is outside that closure, with its holders), `why6.py <module>` (the import path that keeps one), `cand6.py`.
`tools/check_imports.py` under plain python: 249 modules, 0 failing, 0 dangling (286 before the batch).

## Group 1: dev tools nothing calls

Deleted: **26 files, 3,892 lines**: `tools/{bench_re.py,bench_re.cc,cache_tidy_proof,cb_order,chart,gate_teeth16,ghidra_fork,goto_report,l2check,oss_bench,sample,sleigh_verify,sleigh_verify_ppc,struct_report,trace16,verdict_diff,causes,failures}.py`,
`tools/goto_synthetic/shapes.c` (only goto_report read it), `tools/record16/*` (5 files), and the two `tools/cb_shim/sample_{shapes,text}_cases.py` golden samplers (they read the cleared `state/work/typed-port` harness folders; the goldens they made are checked in).
No suite, cmd file, tool, native source or other repository names any of them (searched `tests tools native/cb docs`; the `state` folder names in `native/cb/base/tidy_state.cpp` are data, not callers). Kept: `check_imports tidy freeze_tests golden l2c carve merge move prune unreachable make_api_table` (suites, `tests/*.py` or the tidy family call them), `judge_c_java.py` (below), `cb_shim/{byop,cmpjson,difflines,showcase}.py` and the `.cmd` files (no `codebreaker` import).
Orphaned by them (tidy): `decompile/ctree` (241 lines, a module). Nothing else: every module those tools reached is also held by a test or the boot closure (the tools-only pool after this group is **0** modules).

## Group 2: translate/*

Deleted: **16 modules, 12,696 lines**: `translate/backends/{c_java,csharp,csharp_java}`, `translate/dotnet/{__init__,cs2cpp,judge,pairs}`, `translate/frontends/csharp/{__init__,lower}`, `translate/frontends/python/{__init__,infer,lower,methods,program,pytypes,structfmt}`;
the tests `test_java_as_c test_csharp_kit test_csharp_lower_trees test_dotnet_toolchain test_python_frontend test_pytypes` (all under `tests/translate`, 746 lines) and the golden generator `native/cb/translate/testdata/csharp_java/make_csharp_java_golden.py` (its goldens are checked in).
`javajudge.build` loses its `'c'` branch (c_java was its only user: `test_java_as_c`); tidy removed what that orphaned in `jmodel` (two unused definitions).

### Left, and why (all of it is the boot closure or still held)

- `translate/{ir,l2,native_l2,desugar}`, `backends/{asm,c,cpp03,cpp17,module_file}`, `frontends/java`: the boot closure (`source/module_writer` -> `source/java_writer` -> `java/transpile` -> `translate.backends.cpp03/cpp17`, `translate.frontends.java`).
- `backends/{java,java_objects,java_libc}`, `frontends/clang/*` (`ctypes lower j2x`), `judge_native_jvm`: held by `tools/judge_c_java.py`. The native round trip (`translate/judge_c_java_command`, branch a266) is NOT registered in the exe (`native/cb/app/native_commands.*` is out of bounds for this session), so the Python command is still the only way to run it. Their tests (`test_java_backend`, `test_clang_frontend`, `test_j2x`) stay with them. When the integrator registers the native command: delete `tools/judge_c_java.py`, then these 7 modules and 3 tests (about 6,200 lines) become free.
- `backends/jmodel`, `javajudge`: held by `tests/translate/test_cpp17_backend.py`. Two of its cases have no native twin: `test_the_program_runs_as_the_jvm_runs_it` (the C++17 program of Modern.java built and run beside the JVM) and `test_inside_the_transpiler_beside_cpp03` (Tally.java converted with `cpp_std=C++17`, compiled with `-fsyntax-only`). They need the C++17 runtime headers and a JVM in a native test; not ported (818 lines, 2 modules; cheap to do next).
- `translate/rewrite`: imported by `java/projects/minecraft/{applet_ping,render_fix,sound_fix}` (below).

### The tests, case by case

| Python case | native test |
|---|---|
| test_java_as_c `test_classes_are_structs_and_tables` | `translate.c_java.own.zoo` (the whole C program of the same Zoo.java against a golden: the struct, vtable, class record, itab, setjmp handler and closure record are in it) |
| `test_the_c_program_runs_as_the_jvm_runs_it` | `translate.c_java.runs.zoo` (built with clang, run beside javac/java, output compared), `runs.mix`, `runs.names` |
| test_csharp_kit `every_corpus_program_lowers_to_valid_l2` | `csharp.frontend.lowers_every_corpus_program_to_valid_l2` |
| `the_bound_tree_says_what_the_compiler_resolved` | `csharp.frontend.the_bound_tree_says_what_the_compiler_resolved` |
| `what_cannot_be_carried_is_refused_with_its_reason` | `csharp.frontend.what_cannot_be_carried_is_refused_with_its_reason`, `csharp.decline.*` |
| `csharp_round_trip_is_equivalent` | `csharp.pairs.every_corpus_program_round_trips_to_equivalent` |
| `the_back_end_writes_csharp_idioms`, `units_of_one_file_stem_are_separate_files_and_big_constants_fold_exactly` | `csharp.corpus.{linq,bank,records,inventory}` (whole files against goldens), `csharp.program.file_names_and_writing` (`Same_2.cs`), `csharp.expr.constants_that_overflow_are_unchecked` |
| `java_to_csharp_is_equivalent`, `java_semantics_are_kept_in_csharp` | `csharp.pairs.java_programs_round_trip_to_equivalent`, `csharp_java.corpus.{calc,fixtures,modern}`, `csharp_java.semantics_in_the_text`, `csharp_java.features` |
| `minecraft_alpha_noise_java_to_csharp_is_equivalent` | not ported: it read `NoiseGeneratorPerlin.java` from a Converted Minecraft tree and skipped without it (the tree was cleared 2026-11-10); `csharp_java.corpus.*` judge the same back end on programs that are checked in |
| `csharp_to_cpp03_is_equivalent`, `..._refuses_on_a_runtime_gap`, `..._refuses_what_it_cannot_carry` | `cs2cpp.*` (`own back corpus gen top tool javart`), `csharp.pairs.what_the_cpp03_path_cannot_carry_is_refused` |
| `the_judge_compares_output_and_exit_code` | `judge.compare_equivalent`, `compare_divergent`, `compare_other_verdicts`, `first_errors` |
| test_csharp_lower_trees (2 cases) | `csharp.lower.corpus_programs_give_the_reviewed_documents`, `cases_give_the_reviewed_documents_and_refusals` (the made tree and the corpus dump) |
| test_dotnet_toolchain (2 cases) | `dotnet.discovery_on_an_installation`, `dotnet.newest_version_dir`, `dotnet.version_key`, `dotnet.discovery_gaps` |
| test_pytypes (5 cases) | `translate.pytypes.{xor_lower_bound,interval_results,every_operation_contains_its_results,unary_operations_contain_their_results}`, `translate.structfmt.{integers,repeat_counts,refused,empty_and_prefix}` |
| test_python_frontend `semantics_translate`, `int_proofs` | `python.lower.functions_are_written_as_l2`, `more_of_what_a_function_can_hold`, `translate.cpp17.write_unit` (the unit's header and source against the Python's text: `py::floordiv`, `total += i;` are in it) |
| `refusals` | `python.lower.refusals_have_words_and_reasons`, `a_refusal_reaches_whoever_calls_the_refused`, `a_call_of_a_function_left_out_refuses_the_caller`, `the_checked_policy_writes_checked_operations` |
| `semantics_compile_and_match` (MSVC build, every `run_*` compared with CPython) | **new** `translate.cpp17.the_python_semantics_snippet_runs_as_cpython_runs_it` (`backends/cpp17_run_test.cpp`): the checked-in golden header and source built with `cl` against `native/ported/include`, 15 functions run, each compared with `testdata/cpp17/semantics.expected.txt`, which CPython made by running the snippet (`python_reach/semantics_expected.py`); skipped without `cl` on PATH, as the Python skipped without MSVC |

## Group 3: exact/*

Deleted: **18 modules, 9,503 lines**: `exact/{aim,batch,carve,compare,data,library,modules,mspascal,plm86,hooks_lattice,x86}` (11), `exact/draft/{__init__,candidates,forms,prove,shapes,text}` (6), `exact/emu/isis`; tests `test_exact_fixes.py` and 6 of the 10 cases of `test_port_fixes.py` (the 4 that stay test `java.rules`, `source.ne_asm.segdiff`, `analyze.origin` and `exact.emu.native_dosexec`).
The plug-ins named in `data/exact/toolchains/*.json` (`codebreaker.exact.plm86:PLM86`, `mspascal:MSPascal`, `xenix_c:XenixC`, `hooks_lattice:cmp_test`) are opaque keys to the native `exact/toolchain.cpp` (`register_plugin`); the JSON is unchanged. What the Python `exact/toolchain.load` still loads: only `xenixc` (`build/period.py`, reached from `verify/authored/repair`, the boot closure) and `msc203` (no plug-in), so `exact/xenix_c`, `exact/emu/xenix`, `exact/toolchain`, `exact/aout` and `exact/emu/{dos,dpmi,native_dosexec}` stay. `plm86`, `mspascal`, `isis`, `hooks_lattice` were loaded by name only; nothing in the kept Python loads the profiles that name them (searched `toolchain.load` and the tests).

| Python case | native test |
|---|---|
| test_exact_fixes `aim_reads_a_decimal_with_a_leading_zero` | `exact.aim.a_decimal_with_a_leading_zero_is_read_as_decimal` (the same three operands) |
| test_port_fixes `candidates_localize_twice_lays_out_the_merged_struct_array_the_same` | **new** `exact.draft.candidates.localize_twice_lays_out_the_merged_struct_array_the_same` (the tables are the caller's `State` now, so the check is that two runs on one state agree and the struct text is written) |
| `forms_library_macros_keep_the_include_a_macro_does_not_name` | `exact.draft.forms.keeps_the_include_a_macro_does_not_name` |
| `forms_register_temps_asks_for_the_whole_register_name` | `exact.draft.forms.register_temps_names_the_whole_variable` |
| `forms_default_after_keeps_the_statements_after_the_default_cases_return` | `exact.draft.forms` (`default_after`, line 366 of `forms_test.cpp`) |
| `data_declared_size_takes_the_first_declaration_by_name` | `exact.data.*` (`declared_size({{"int","a"},{"char","a"}}) == 1`) |
| `draft_shapes_store_joins_at_the_end_of_the_text_finds_nothing` | `exact.draft.shapes.*` (`store_joins`, the tail case of `shapes_test.cpp`) |

## Group 4: java/*: nothing deleted, and why

`java/transpile` and its 33 modules are the boot closure (`source/module_writer` -> `source/java_writer` -> `java/transpile`); the Minecraft project (`java/projects/minecraft/**`, 28 modules, 3,300 lines) is loaded by name from there (`java/projects/__init__.for_profile` -> `importlib`; `java/profiles` calls it) and by 16 kept tests of the transpiler that select a `minecraft_*` profile
(`test_beta_fixes test_build3_fixes test_call_arg_cast test_ctor_delegation_cast test_edition_convert_integration test_int_array_unbox test_noequals_identity test_pc_platform test_platform_refs test_probe_libraries test_profiles_selection test_profile_invariants test_ptr_factory test_reflection_factory test_runnable_entry ...`). Deleting the project means retiring all of `java/*` and those tests together, which waits on the
module_writer decision (`java_writer` is the only way in). `java/console/xdk_scan` is held by `minecraft/platform_coverage`. Native twins of every one of them exist (`native/cb/java/**`, `translate/java_ports_test.cpp`); what each test maps to is already recorded in those files' headers.

## Totals and what is left

| | modules / files | lines |
|---|---|---|
| dev tools | 26 files | 3,892 |
| translate/* | 16 modules | 12,696 |
| exact/* | 18 modules | 9,503 |
| orphaned by them (tidy) | 3 modules (`decompile/ctree`, `invert/flow16`, `invert/native_invert`) | 586 |
| tests deleted or cut | 7 deleted, 1 cut (`test_port_fixes`) | about 970 |
| golden generators | 1 | 66 |

**37 modules, 22,807 lines** under `codebreaker/` (`git diff --stat` over the three commits), 4,000-odd more in tools, tests and generators. Python modules left: 249 under `codebreaker/` (286 before). New native: 2 tests (`translate.cpp17.the_python_semantics_snippet_runs_as_cpython_runs_it`, `exact.draft.candidates.localize_twice_*`) and `testdata/cpp17/semantics.expected.txt`.
