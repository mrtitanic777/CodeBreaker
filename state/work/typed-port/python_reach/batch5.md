# Batch 5: the NES lane, recompile/*, the Windows image writers and the DOS / driver lanes

Roots, scripts and conditions as in batch 4 (`python_reach/{graph,live3,cand5}.py`; `cand5.py <prefix,...>` shows who outside a candidate set imports each module, tests and tools included).
`tools/check_imports.py` under plain python before this batch: 5 modules that do not import (`source/{dos,image,le,ne,pe}_writer`: they import `compare.image`, which lives only in the typed port) -- all five are in group 3.

## Group 1: the NES lane

Deleted: **8 modules, 1,105 lines**: `formats/nes`, `platforms/nes`, `verify/lanes/nes`, `verify/lanes/image_lane`, `build/m6502_asm`, `decompile/m6502`, `emulate/m6502`, `source/nes_writer`;
also `tests/test_nes.py` (177 lines) and `tools/sleigh_verify_6502.py` (150 lines: it imported the two deleted 6502 modules; its check is now a native test).

Wiring changed: `platforms.ORDER` loses `"nes"`; `decompile.decompiler.as_c` and `decompile.describe` lose their NES branch (`describe_nes`; the native `decompile.decompiler.nes_cartridge_as_c` is the lane); `tests/test_platforms.py` expects the
shorter `KINDS`/`RUNNERS` tables; `tests/test_run_record.py` loses its 6502 run; `tests/test_verification.py` loses `test_runs_of_an_image_lane_that_differ_in_how_many_places...`; `tests/run_all.py` loses the `test_nes.py` row.

### The tests, case by case

| Python case | native test |
|---|---|
| test_nes `test_cartridge` (mapper, mirroring, sizes, vectors, functions `[C000, C04F, C05C, C076]`, 16 KB mirrored, identified as nes) | `engine.nes_demo.the_cartridge_and_its_functions` (new, on the demo cartridge built by our assembler), `formats.nes.*`, `platforms.consoles.registry_identifies_each_by_its_probe` |
| `test_table_round_trip` | `decompile.m6502.table`, `roundtrip_every_opcode` (151 x 9 forms) |
| `test_source_rebuilds` | `engine.nes_demo.the_source_rebuilds_the_cartridge` (new: the demo's text has `reset:`, `JSR sub_C04F`, `BIT PPUSTATUS`, `LDA D_C077,x`, `.word nmi, reset, irq`), `source.nes_writer.*` |
| `test_interpreter` (6 x 7 = 42; reset against a PPU in vblank: the PPU writes, the palette, `$12 = 42`, ends at the step limit) | `engine.nes_demo.the_program_runs_on_the_interpreter` (new), `emulate.m6502.run_multiplies`, `run_logs_device_accesses_in_order` |
| `test_flags_documented` (ADC overflow and carry-in, SBC no-borrow, JSR pushes its last byte, JMP ($30FF) page wrap) | `emulate.m6502.adc_binary`, `sbc_carry_is_no_borrow`, `jsr_rts_brk_rti`, `indirect_jmp_page_bug` |
| `test_sleigh_agrees` (`tools/sleigh_verify_6502.py --states 8`: 151 opcodes x 8 states on the SLEIGH p-code vs the interpreter; skipped without pcodecheck) | `engine.nes_demo.sleigh_6502_agrees_with_the_interpreter` (new: the same cases, run on `pcodecheck.exe`; every form agrees; prints SKIP without it) |
| `test_decompiles` (the native decompiler on the cartridge: 4 of 4 functions, `RAM2006 = 0x3f;`, `RAM2006 = 0;`, the multiply's loop) | `engine.nes_demo.the_native_decompiler_gives_the_c` (new: the real `decompiler.exe`, found in this checkout, `CB_DECOMPILER_BIN` or the main checkout; SKIP without it), `decompile.decompiler.nes_cartridge_as_c` (the fake-program path) |
| `test_lane_judges` (equivalent for every function; ADC -> EOR is divergent for the multiply, equivalent for irq) | `engine.lane_kit.machines.the_nes_demo_cartridge_is_judged` |
| test_verification `test_runs_of_an_image_lane_that_differ_in_how_many_places...` | `verify.lanes.image_lane.effects_of_different_length`, `first_differing_state_is_named` |
| test_platforms `every lane` / `every runner` rows for nes | `platforms.nes.descriptor` |
| test_run_record 6502 run (LDA #5 / RTS: run record exit "return", A = 5) | `emulate.m6502.record` |

## Group 2: recompile/*

Deleted: **15 modules, 5,510 lines**: `recompile/{__init__,config,disassemble,discover,jumptable,powerpc,switches,xenon}` and `recompile/builders/{__init__,arith,floating,logical,memory,system,vector}`;
`tests/test_recompile.py` (226), `tests/test_x360.py` (206) and the seven golden generators `native/cb/recompile/testdata/gen_{builders,config,discover,floating,powerpc,ppc_slice,switch_samples}*.py` (962: they imported the deleted package; the goldens they made are checked in, as in batch 4).
`tests/translate/test_python_frontend.py` found MSVC through `recompile.xenon.msvc_env`: it now carries its own 12-line `_msvc_env` (the setting `visual_studio`, else vswhere, then vcvars64). `__main__.COMMANDS` loses `recompile` and `xenon-recompile` (both are in `NATIVE`); `tests/run_all.py` loses the `test_recompile.py` row.

The Python tests compiled and ran things (the PowerPC compiler of the Xbox 360 SDK, g++, clang-cl, the PowerPC binutils); the native tests of `native/cb/recompile` compared with goldens. New native tests that do the same builds and runs:

- `native/cb/source/ppc_recompiled_runs_test.cpp` (area source: it sees the compiler's wrapper `source/pipeline_ppc` and the recompiler): C compiled with the SDK's `cl.exe` (`powerpc_tools`), recompiled by `recompile::powerpc::recompile_function`, built with g++ against the rexglue kernel headers, run, compared with the same computation. All five ran for real here (74 checks).
- `native/cb/recompile/x360_real_test.cpp`: the two slow checks (`CB_SLOW_TESTS=1`): `xex_image_test` built with clang-cl from `native/x360/runtime/test` against the xex engine's basefile (ran for real here: passes on LoganMinecraftTest.xex), and Xenia's instruction tests through XenonRecomp (SKIP here: no `ppc_binutils` set up, as the Python SKIPPED without them).

### The tests, case by case

| Python case | native test |
|---|---|
| test_recompile `test_integer_leaves` (10 expressions through `cl` + recompile + g++ + run) | `source.ppc_recompiled.integer_leaves_give_what_the_c_gives` (new) |
| `test_floating_point` | `source.ppc_recompiled.floating_point_gives_what_the_c_gives` (new) |
| `test_loop_over_memory` | `source.ppc_recompiled.a_loop_over_big_endian_memory` (new) |
| `test_indirect_call_table` | `source.ppc_recompiled.the_indirect_call_table_dispatches_by_guest_address` (new) |
| `test_vmx128_first` | `source.ppc_recompiled.a_vmx128_word_is_read_as_vmx128` (new), `recompile.disassemble.vmx128_primary_opcodes` |
| `test_whole_image` (the whole Minecraft image recompiled, only padding untranslated; 200 functions compiled with g++; skipped without `Converted/Minecraft`) | not ported as such: it ran on one title's image in `Converted/`, which is cleared and never committed. The whole-title recompile is covered by `recompile.command.the_rexglue_project_matches_the_python` (LoganMinecraftTest: 8561 functions, every file of the project byte-compared with what the Python wrote, 99.9% of 749,758 instructions), `recompile.command.the_first_functions_match_the_python` and `recompile.powerpc.an_image_as_the_reference_makes_it` / `an_image_meaning` (goldens made by the Python), and the generated code's compile in `source.ppc_recompiled.*` |
| `test_discover_built_addresses` (536 constants, 27,700 `.pdata`, four leaf functions; the same image) | `recompile.discover.constants_built_by_lis_and_addi_or_ori`, `a_functions_first_instruction`, `a_bl_chain_is_followed_to_its_end`, `every_function_with_and_without_the_scan_and_seeds` (the same findings on the made program and the slice of real code) |
| test_x360 `test_helpers_and_jump_buffers` | `recompile.config.helpers_by_name_and_by_pattern`, `jump_buffers` |
| `test_setjmp_through_a_register_other_than_r3`, `test_longjmp_without_setjmp_emits_neither` | `recompile.config.jump_buffers`, `a_longjmp_without_its_setjmp_is_not_given` |
| `test_switch_tables` | `recompile.switches.real_tables_of_minecraft_match_the_original` (15+ real samples of Minecraft's tables), `unguarded_table_ends_at_the_function_bounds` |
| `test_extra_functions_respect_switches`, `test_tail_jump_targets_are_functions` | `recompile.config.extra_functions`, `extra_functions_meaning` |
| `test_basic_xex` | `recompile.xenon_tools.xex_basic_writes_what_the_python_writes` |
| `test_runtime_read_image` (slow) | `recompile.x360.the_runtimes_read_image_is_the_xex_engines_basefile` (new, slow, real) |
| `test_instruction_tests` (slow) | `recompile.x360.xenias_instruction_tests_through_xenonrecomp` (new, slow, SKIP here), `recompile.xenon.instruction_tests_assemble_recompile_build_and_run` (the module's logic with stand-in tools) |
| test_python_frontend `_msvc` | stays Python; own `_msvc_env` |

Note for the owner of `native/x360`: `native/x360/runtime/test/CMakeLists.txt` and `xex_image_test.cpp` still name `tests/test_x360.py` in their comments; the test is now `recompile.x360.the_runtimes_read_image_is_the_xex_engines_basefile`. Not touched.

## Group 3: the Windows image writers and the DOS / driver lanes

Deleted: **8 modules, 4,122 lines**: `source/{dos,driver,le,ne,pe,image}_writer` (1,005 + 625 + 518 + 458 + 529 + 345) and `verify/lanes/{dos,driver}` (75 + 567); the golden generator `native/cb/verify/lanes/testdata/gen_driver_lane.py` (52).
Under plain python these five writers already did not import (`compare.image` lives only in the typed port); the exe ran them through the typed manifest. `tools/check_imports.py`: 289 modules, 0 failing, 0 dangling (it was 5 failing, 13 dangling before this group).
Kept, as told: `verify/authored/*`, `verify/gate16/*`, `verify/lanes/powerpc`, the `module_writer` closure (none of the eight is in its static closure: only `platforms/{dos,vxd}` named them, as strings).

Wiring changed: `platforms/dos.py` and `platforms/vxd.py` lose `lane` (now `None`) and the `writer` of their packager (the lane and the writer are the native `cb/verify/lanes/{dos,driver}` and `cb/source/{dos,driver}_writer`; the Python registry keeps their reader, cpu, convention, emulator and tools); `__main__.COMMANDS` loses `driver-source dos-source pe-source ne-source le-source driver-judge`
(all six are in `NATIVE`); `tests/test_platforms.py` expects `KINDS` without `le`/`dos`.

### The tests, case by case

| Python case | native test |
|---|---|
| test_drivers `test_judge_agrees_with_itself_and_catches_a_break` | `verify.driver.each_entry_point_judged_against_itself_is_the_pythons_verdict`, `a_copy_with_one_executed_instruction_broken_is_judged_as_the_python_judged_it` |
| `test_run_records_service_calls` | `verify.driver.a_fingerprint_is_what_the_python_recorded`, `emulate.driver.every_entry_point_runs_as_the_python_recorded_it`, `the_run_record_carries_what_the_judge_compares` |
| `test_build_from_source` (a driver's source built, the structure checked; skipped without the kit's sources) | `source.driver_writer.a_driver_is_written_built_and_comes_back_the_original_module` (wasm + wlink, structure compared), `build.driver.structure_of_real_drivers`, `an_image_agrees_with_itself`, `build_without_a_device_declaration` |
| `test_source_written_from_the_image_builds_back` (writer: no address in the text, builds, same objects/descriptor/services/exports; one L2 tree; every entry point judged equivalent) | `source.driver_writer.the_source_with_every_instruction_a_mnemonic_is_what_the_python_writer_wrote`, `a_driver_is_written_built_and_comes_back_the_original_module`, `the_whole_driver_is_one_l2_tree_each_function_a_fragment_at_its_place`, `a_service_call_is_the_int_and_its_two_words_with_the_service_named_beside`, `verify.driver.prepare_and_build_make_the_entry_points_the_units_judged` |
| test_run_record the 32-bit VxD run (`platforms.of("le").emulator.run` on splitter.vxd) | `emulate.driver.every_entry_point_runs_as_the_python_recorded_it`, `the_run_record_carries_what_the_judge_compares` (the Python emulate.driver, which that test called, stays) |
| test_verification `test_a_dos_program_whose_source_text_is_missing_still_has_its_function_keys` | `verify.lanes.dos.prepare_without_source`, `prepare`, `different_program_says_how` |
| test_port_fixes `test_pe_writer_nosplit_keeps_the_scaled_index` | `source.pe_writer.a_scaled_index_is_written_nosplit_so_the_assembler_keeps_it_one_register` |
| test_module_model `test_emit_makes_each_function_a_fragment` | `source.image_writer.emit_writes_labels_functions_instructions_and_data`, `functions_keep_the_names_given_and_make_up_the_rest`, `data_lines_*` |
| `test_the_nasm_writers_make_each_function_a_fragment` (lzexpand.dll, lz32.dll, splitter.vxd: more than 5 functions, each with a length and the module as its origin; the fragments joined are the file) | `source.nasm_fragments.{an_ne_program,a_pe_program,an_le_driver}_is_one_fragment_a_function` (new: the same three files and checks) |
| test_platforms `every lane` rows for `le`, `dos` | `verify.lanes.dos.profile_follows_the_tools`, `verify.driver.the_profile_depends_on_what_a_verdict_depends_on` |
| (never tested in Python, only by running) the writers' commands, `pe-source` etc. | `source.{pe,ne,le,dos,driver}_writer.the_command_*` |

## What was run

- `tools/check_imports.py` after each group, under plain python: group 1: 307 pass / 18 fail (all in group 3's modules, as before); group 2: 292 / 18; group 3: 289 modules, 0 failing, 0 dangling.
- Native, area builds (CB_BUILD_JOBS=3): `area engine --filter engine.nes_demo` 5 passed (185 checks, none skipped); `area source --filter source.ppc_recompiled` 5 passed (74 checks, the SDK compiler, g++ and the kernel headers all present); `area source --filter source.nasm_fragments` 3 passed (it failed on the first try, on a wrong key: the check has teeth);
  `area recompile --filter recompile.x360` 11 passed with the two slow ones skipped, and `CB_SLOW_TESTS=1 ... recompile.x360.the_runtimes` passed (clang-cl build of `xex_image_test`, ReadImage vs basefile on LoganMinecraftTest.xex).
