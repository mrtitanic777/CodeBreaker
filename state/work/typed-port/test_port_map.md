# Test port map, half 1 (final) (tests/test_*.py test_analysis..test_platforms, plus tests/java)

2026-11-13 (updated after the second pass). Tags: [C] covered by an existing native/cb test (cited), [P] ported (this pass or the first), [N] not needed (why). No [G] is left in this half; see Counts.
Native tests are `area.module.test` (CB_TEST). "Covered" means the suite body was read and a native test checks the same behaviour; most native tests are
goldens of what the Python answered, so inputs are often the same real files. tests/java rows were matched by test name and area only (marked "by name").
Python checks that need the retail corpus / DOSBox / MASM / a PowerPC compiler skip; their native twins skip the same way.

## test_analysis.py (12)
- [C] test_image_32 -- analyze.image.a_pe_module_has_the_functions_the_live_loader_found, ...spans_imports_and_relocations...
- [C] test_image_16_and_driver -- analyze.image.an_ne_layout_puts_each_segment_at_its_selector, an_le_layout_names_what_the_device_block_gives_away
- [C] test_arguments_of_a_call -- analyze.conventions.an_argument_slot_the_code_reads_gives_the_argument_count, callers_that_clean_up_give_the_argument_count_away
- [C] test_call_graph -- analyze.call_graph.callers_users_and_cycles, calls_by_kind, reach_and_unreferenced
- [C] test_conventions -- analyze.conventions.a_function_that_removes_its_own_arguments_is_stdcall, golden_the_facts_of_fifteen_real_modules
- [C] test_globals_and_strings -- analyze.globals.golden_..., analyze.strings.conversions_match_the_engine_it_replaces, assemble_matches_the_engine_it_replaces
- [C] test_globals_count_indexed_arrays -- analyze.globals.arrays_are_indexed_types_not_strings
- [C] test_structures -- analyze.structures.a_pointer_field_an_array_a_float_and_a_called_field, the_report_counts_what_was_found
- [C] test_dispatch_and_resources -- analyze.dispatch.the_message_names, a_jump_table_is_a_switch_with_a_default; analyze.resources.reads_the_loads_and_the_controls
- [C] test_classes_none_in_c -- analyze.classes.decorated_names, a_16_bit_module_has_no_classes
- [C] test_observation_arithmetic -- analyze.observe.digest_counts_what_a_run_does_to_each_buffer, digest_reads_the_end_of_a_run, after_applies_the_runs_writes_to_a_buffer
- [C] test_report_and_fact_table -- analyze.report.a_16_bit_module, analyze.facts.*, analyze.report.the_names_of_the_analyses

## test_assembler.py (1: runs native/nasm/test/masm/run.py, flat and --objects)
- [P] flat fixtures vs golden -- build.masm_fixtures.every_fixture_assembles_to_its_golden_bytes (native/cb/build/masm_fixtures_test.cpp; masm_paren_operand known)
- [P] object backends (-f obj OMF, -f win32 COFF) -- build.masm_fixtures.segment_fixtures_give_the_same_code_in_the_object_backends

## test_cmpp.py (1)
- [C] test_the_entry_datas_first_byte_is_no_opcode -- exact.habits.cmpp.loose_bytes_match_the_pythons ({E8 00 00 E8 34 12 C3}), walk_matches_the_pythons_listings

## test_concolic.py (11)
- [C] folded_expressions -- emulate.symbolic.concat_of_neighbours_is_the_whole, conditions_on_boolean_bytes_fold, extract_folds_through_what_it_takes_from
- [C] every_path_found_and_said_to_be_all -- verify.concolic.three_ways_through_two_branches, finds_each_path_of_a_chain_of_tests
- [C] seeds_not_run_again -- verify.concolic.seeds_that_took_a_known_path_are_not_run_again
- [C] loop_sampled_not_all -- verify.concolic.the_budget_limits_runs_and_says_the_paths_are_a_sample
- [C] shadow_gives_a_jumps_condition -- emulate.shadow.a_compare_and_branch_is_a_condition_of_the_path, memory_bytes_are_inputs_until_the_run_writes_them
- [P] a_run_with_its_shadow_is_the_same_run -- emulate.windows16.a_run_with_its_shadow_is_the_same_run (LZEXPAND, 8 entries x 3 inputs, with/without the real pcodecheck shadow: end, steps, calls, writes, results alike; skips without pcodecheck)
- [C] lifter_gives_the_specifications_p_code -- emulate.pcode.lifts_a_16_bit_instruction_in_its_window
- [P] 32_bit_run_with_its_shadow_is_the_same_run -- verify.win32.a_32_bit_run_with_its_shadow_is_the_same_run (LZ32 exports x the fixed+confirm vectors, fast tracer vs one-instruction shadowed run)
- [P] 32_bit_explorer_finds_every_path_of_lzopenfile -- verify.win32.the_explorer_finds_every_path_of_lzopenfile (complete, >=4 paths, each kept input takes a path no seed took)
- [C] powerpc_shadow_condition_over_an_argument -- emulate.shadow.lift_mode_follows_the_processor_language, emulate.pcode.lifts_powerpc
- [C] lifter_names_the_powerpc_registers -- emulate.pcode.lifts_powerpc

## test_converted.py (4)
- [C] layout_and_readme / one_project_per_binary -- base.converted.a_project_is_made_once_and_recorded_like_the_python_did, recording_a_command_again_replaces_its_step
- [C] generic_names -- base.converted.default_name_is_the_file_name_unless_generic
- [C] an_output_beside_the_project_is_not_inside_it -- base.converted.an_output_beside_the_project_is_not_inside_it

## test_dos.py (20)
- [C] kinds -- formats.dos.classification; com_and_driver_layout -- formats.dos.com_file_and_dos_stub_of_a_windows_module, drivers; relocations_are_applied -- formats.dos.mz_header
- [C] exepack_static_equals_emulated -- formats.dos.exepack_is_undone_statically, emulated_unpack_reports_failure; unpacked_file -- formats.dos.unpacked_file_reads_back
- [C] kernel_image_parts -- formats.dos.chain_of_programs; function_discovery -- analyze.dos_functions.find_exe, xcopy_exe; installed_handler_is_found -- analyze.dos_functions.an_installed_interrupt_handler_is_a_function
- [C] interrupt_names -- symbols.dos_interrupts.names_services, the_mouse_selects_by_all_of_ax, annotate; description_for_the_decompiler -- decompile.describe.dos_program_is_what_the_original_wrote
- [C] boot_floppy -- build.floppy.windows_95_setup_floppy_reads_and_makes_like_the_python; cd_image -- formats.iso9660.list; not_an_image -- formats.media.things_that_are_not_images
- [C] native_dos_switch -- build.native_dos.the_switch; native_masm_assembles -- build.native_dos.assemble_reads_back_the_products; native_masm_matches_dosbox -- build.masm.when_the_native_run_cannot_complete_dosbox_takes_over (DOSBox half skips as before)
- [C] dpmi_host_protected_mode -- exact.emu.dpmi.the_mode_switch_builds_the_tables_and_the_trampoline, simulating_a_real_mode_int21; mcb_arena_walk -- exact.emu.dos.the_chain_is_read_from_the_headers
- [C] native_tnt_ml_assembles / matches_dosbox -- build.native_dos.ml_toolchain

## test_dpmi.py (2)
- [C] ldt_overrun -- exact.emu.dpmi.a_full_ldt_is_an_error_not_an_overrun; gdt_selector_refused -- exact.emu.dpmi.specific_descriptors_and_raw_descriptor_access (0x8022 check)

## test_drivers.py (16)
- [C] table_names_services/identifiers/a_real_driver -- symbols.driver_services.size_of_the_real_table, labels, coverage_of_a_driver
- [C] reader_descriptor_and_entries/fixups_encode_back/container -- formats.le.splitter_descriptor_and_services, splitter_fixups, w3_container_of_the_real_vmm32
- [C] writer_round_trip -- source.vxd.le_writer.a_driver_written_back_reads_as_the_same_driver
- [C] judge_agrees_with_itself / run_records_service_calls -- verify.driver.each_entry_point_judged_against_itself_is_the_pythons_verdict, a_fingerprint_is_what_the_python_recorded
- [C] compare_share_ignores_patched_places -- build.driver.compare_counts_equal_bytes_outside_patched_places
- [C] build_from_source / source_written_builds_back -- build.driver.build_a_driver_with_openwatcom, source.driver_writer.a_driver_is_written_built_and_comes_back_the_original_module
- [C] kit_macro_source (+builds_back) -- source.vxd.idiom.the_ddb_as_a_macro, a_service_call_as_a_macro; source.vxd.from_source.splitter_comes_back_as_the_original
- [C] boot_disk_injection/boot_screens/boot_bisect -- source.vxd.boot.where_each_driver_goes_on_the_disk, what_a_screen_shows, bisection_finds_the_units_that_fail_alone

## test_engine.py (13)
- [C] made_once / provenance / given_fact_recomputes / same_value_stops_the_change / new_step_version / changed_binary / stale_side_by_side / reads_only_what_it_declares / batch_and_unkept / files_are_facts / values_and_blobs / old_versions_and_cap
  -- engine.engine.a_fact_is_made_once_and_kept, engine.facts.rows_keep_how_they_were_made, engine.engine.a_given_fact_changed_makes_stale_what_was_made_from_it,
  a_new_step_version_remakes_but_an_equal_result_leaves_dependents_alone, engine.facts.databases_are_per_binary_and_one_open_per_file, engine.engine.many_makes_the_stale_side_by_side_and_a_batch_step_in_one_call,
  a_step_may_only_read_what_it_declared, an_unkept_value_is_used_this_run_and_made_again_the_next, a_file_that_is_not_a_fact_is_an_input_by_its_hash, engine.facts.a_large_value_is_kept_once_by_its_hash, the_cap_removes_the_oldest_closed_database
- [C] a_lane_on_the_lanes_steps -- engine.lanes.the_steps_give_what_the_pythons_steps_gave_stage_by_stage

## test_exact_fixes.py (2)
- [P] aim_reads_a_decimal_with_a_leading_zero -- exact.aim.a_decimal_with_a_leading_zero_is_read_as_decimal
- [P] standin_keeps_a_four_bit_location -- exact.standin.a_fixup_location_is_four_bits

## test_exepack.py (1) [C] build.exepack.pack_roundtrip_both_headers
## test_flat_thunk.py (2) [C] build.flat_thunk.a_ret_cut_off_inside_its_operand_resolves_nothing, classify_finds_kernel32_whatever_the_case_of_its_name

## test_frozen.py (22 recorded cases) and test_frozen_slow.py (7)
- [C] c_text interrupt_helper_name/interrupt_helpers/interrupt_routines -- decompile.c_text.* (same names) (3)
- [P] c_text _far_loads/_global_type/_loads_dgroup/_more_helpers/_ram_type (5) -- decompile.c_text.frozen_* (the recorded cases copied to decompile/testdata/frozen/c_text; far_loads16/loads_dgroup16 exposed from c_text_win16.cpp)
- [C] formats ne_lzexpand, pe_lz32, resources_lzexpand, inspect_lzexpand, pe_functions_lz32, cab_mini, fonts_vgafix (7) -- formats.ne.*, formats.pe.lz32_*, formats.resources.the_resource_table_of_a_real_16bit_module, formats.inspect.describe_equals_the_recorded_original, formats.pe_functions.lz32_*, formats.cab.real_windows_95_mini_cab_against_expand, formats.fonts.a_fon_lists_its_fonts
- [P] formats ne_keyboard, pe_notepad, le_vmouse, dos_mode, resources_notepad, detect_vmouse, detect_mode (7) -- formats.frozen.* (recorded outputs in formats/testdata/frozen, compared fact by fact; keyboard.drv and vmouse.vxd are native testdata copies, notepad.exe and mode.com are read from the Win95 corpus and pass without it)
- [C] slow: describe ne_lzexpand, pe_lz32, le_vmouse, dos_mode, ne_keyboard -- decompile.describe.*_is_what_the_original_wrote, ne_driver_and_every_policy_switch (5); c_text_module lzexpand/lz32 -- decompile.c_text.compilable16_on_lzexpand, compilable32 (2)

## test_inspect_render.py (2) [C] formats.inspect.render_makes_records_and_tables (both: nested-by-any-row and unchanged cases)

## test_invert.py (15)
- [C] early_c_reads_a_function/frame_model -- invert.early_c.golden_cases, invert.early_c_frame.ground_truth_frames; flow -- invert.flow16.*; msc6 reads/declines -- invert.msc6.the_documented_function, declines_what_it_does_not_cover; msvc2 -- invert.msvc2.*
- [C] known_differences_table -- invert.known_differences.*; every_model_has_a_command -- invert.command.the_table_has_every_tool_of_the_python; against_the_real_compiler -- invert.command.the_tools_that_need_a_compiler_*
- [C] a_module -- invert.module_pass.*; object_file_functions -- invert.quad_align.functions_of_real_objects; dos_tools -- build.dos_tools.*; routing_thunks_and_grammars -- invert.grammars.*
- [C] 16bit_assembly_source -- source.ne_asm.*, build.asm16.*; debug_build_names -- symbols.debug_build.*

## test_java.py (1) [N] runner for tests/java (rows below)

## test_knowledge.py (8)
- [C] stamp_remake -- knowledge.store.ensure_makes_a_kind_again_only_when_the_stamp_changes; values_keep_their_shape -- knowledge.value.round_trip; name_follows_the_body -- knowledge.store.bodies_and_names; older_stores_bodies -- knowledge.store.carried_notes, store.knowledge_bodies.*
- [C] authored_files -- knowledge.store.authored_kinds_load_from_the_checkouts_files, knowledge.sources.real_files_every_kind_loads; stand_ins_are_data -- engine.win16.*, knowledge.package.win16_memory_is_scoped (same behaviour, not read side by side)
- [C] size_rule -- engine.facts.a_database_with_the_function_stores_tables_loses_only_its_facts; platform_names -- knowledge.package.platform_names

## test_knowledge_reads.py (7)
- [C] changed_prototype / absent_piece / argc_reads / named_body -- engine.engine.what_a_step_read_of_the_knowledge_base_is_an_input, knowledge.reads.digests_freshen_once_per_kind, knowledge.package.known_and_learn_record_the_reads, names_of_bodies
- [C] threads_and_workers -- knowledge.reads.adopted_on_another_thread, carried_through_a_worker_boundary; batch_keys -- knowledge.reads.recording_collects_sorted_unique; outside_a_step -- knowledge.reads.nothing_outside_a_recording, inner_recording_keeps_its_own

## test_m68k.py (6)
- [C] table_round_trip -- decompile.m68k.round_trip_of_every_opcode_word; assembler_refuses -- decompile.m68k.assemble.errors, build.m68k_asm.errors_name_the_line; flags -- emulate.m68k.add_sets_all_five_flags, sub_and_cmp_flags, shifts, divide, multiply
- [C] platform_recognises -- formats.genesis.probe; source_rebuilds -- source.genesis_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom
- [P] lane_judges -- engine.lane_kit.machines.the_genesis_demo_cartridge_is_judged (tests/data/genesis/demo.asm, native 68000 assembler)

## test_match_powerpc.py (4)
- [P] one_function_whole, masks_are_fields, distance, original_stops_at_padding -- source.pipeline_ppc.* (native PowerPC match: pipeline_ppc exposes ppc_compile_function / ppc_original_code / ppc_compare; the Xbox 360 compiler of settings.json is used, the tests skip without it)

## test_mcp.py (13)
- [C] -- ui.mcp.the_command_table_is_the_catalog, the_code_is_read_as_the_python_did, the_service_answers_the_tools_by_name, the_names_and_the_runtime_libraries_are_read_as_the_python_did, commands_and_jobs, the_reading_tools_only_when_there_are_no_writes, slow_values_are_worked_out_in_the_background_and_kept, reading_what_was_made, one_function_is_decompiled_now_as_the_python_did, two_builds_are_compared_and_one_function_shown_side_by_side, the_ledger_is_kept_and_served_from_the_kept_copy_while_a_new_one_is_made, the_http_transport_checks_the_token_the_host_and_the_session (one each, same order as the Python tests)

## test_media.py (12)
- [C] cabinet round trips/expand/rejects/checksum/real mszip/real quantum -- formats.cab.build_and_extract_round_trip, what_we_write_expand_reads, limits, checksum, real_windows_95_mini_cab_against_expand, real_quantum_cabinet
- [C] disc image round trip/bootable/name clashes -- build.disc_image.the_images_are_the_pythons, names_are_cut_to_level_one, formats.media.iso_built_disc, disc_with_an_el_torito_floppy_and_the_same_disc_in_raw_sectors
- [C] fat16 in partition / fat32 -- formats.media.fat16_and_fat32_with_a_folder_that_grows, partitions_primary_and_logical

## test_native_engine.py (4)
- [C] same facts and runs/store/knowledge/verdict words -- engine.engine.*, engine.facts.*, engine.function_store.a_store_written_by_the_c_engine_reads_the_same, knowledge.store.digests_follow_the_piece, engine.function_store.older_spellings_read_as_the_words_now (behaviour)
- [P] replay of tests/data/native_engine_golden.txt -- engine.engine_golden.the_same_facts_and_runs_as_the_python_engine (the "facts" case: same pipeline, same 18 asks and changes; value, kept/made counts per kind, what was made, every fact row incl. SHA-1 hashes; the golden converted to engine/testdata/engine_facts.golden.json). store/kb/verdicts cases stay [C] (above). Differences: the binary size, a file fact's folder, the engine's own file/binary step versions, and a batch step's input recorded twice by the Python are left out

## test_native_formats.py (1) [C] formats.pe.* (truncation/damaged), formats.ne.damage_never_crashes, formats.le.errors_and_damage, formats.dos.errors, reader facts on real files; [N] the Python-attribute digest mechanism

## test_native_trace.py (6)
- [C] same_record_synthetic/shadow32/driver/win16/lz32/record_has_what_was_run -- emulate.native_trace.*, emulate.native_shadow.*, emulate.native_driver.*, emulate.native_win16.*, emulate.emulator.the_real_lz32_dll_traces_the_same_fresh_and_kept (behaviour)
- [N] python_raises_through (Python hook plumbing); [P] tests/data/native_trace_golden.txt -- emulate.emulator.cases_the_recorder_agreed_gave_one_record_give_one_record: the golden is a digest of the Python's repr of each record, which cannot be reproduced without writing Python's text; what it pins that can be is which cases gave the SAME record (plain/stub/stub-unique-reads for f(7), g, h; judge/judge-capped for g, h) -- those groups are checked on the same module and the same eight configurations, with f(5) worked out from the code. The 16-bit / LZ32 / driver / shadow32 digests (lz16, lz32, driver, shadow) are not replayed: [C] by the native twins (emulate.native_win16.*, emulate.emulator.the_real_lz32_dll..., emulate.native_driver.*, emulate.native_shadow.*)

## test_nes.py (8)
- [C] cartridge -- formats.nes.*; table_round_trip -- decompile.m6502.roundtrip_every_opcode; source_rebuilds -- source.nes_writer.the_program_comes_back_as_source_that_builds_the_same_rom; interpreter -- emulate.m6502.run_multiplies, run_logs_device_accesses_in_order; flags_documented -- emulate.m6502.adc_binary, sbc_carry_is_no_borrow, jsr_rts_brk_rti, indirect_jmp_page_bug; decompiles -- decompile.decompiler.nes_cartridge_as_c
- [N] sleigh_agrees -- compared with Ghidra SLEIGH; Ghidra is gone
- [P] lane_judges -- engine.lane_kit.machines.the_nes_demo_cartridge_is_judged

## test_pklite.py (1) [C] build.pklite.reads_a_real_module, encoding_what_was_read_gives_the_stream_again, parse_makes_the_choices_the_real_packer_made, pack_rebuilds_the_real_module

## test_platforms.py (8)
- [C] every lane/runner tables -- platforms.* tests, engine.lane_kit.machines.a_known_machine_has_a_lane_and_another_name_has_none; names unique, nothing recognises, plain file readable -- platforms.*, formats.detect.identify_by_magic; reach kinds -- coverage.reach.*
- [C] languages -- decompile.describe.address_spaces
- [N] every platform fills the six slots; slots equal the lanes' own tables -- compare Python module objects (Ref.target() is module._build16 ...): Python import plumbing

# tests/java (330 tests, by name)
- [C] test_achievements (12) -- java.minecraft.achievements.* (scan_achievements, humanize_and_const_name, distribute_score, map_to_platform_warnings, load_lang_names, run_matches_the_python_module, resolve_icon_atlas, normalize_title_id, suggest_title_id, title_id_from_config, wire_icons_*)
- [C] test_apimap (12) -- java.apimap.*; test_applet_ping (4) -- java.minecraft.applet_ping.*; test_assets (11) -- java.console.assets.*, java.console.xact.*, java.rgba_png.*
- [C] test_awt_stubs (6) -- java.runtime.stub_members_match_python, class_stub_text_and_members (runtime.golden.txt holds Point/PointerInfo/MouseInfo)
- test_beta_fixes (35): [C] 32 -- java.header.typed_enum, constant_enum, java.rules.hotpath_alloc, silent_dispatch, infinite_loop, side_effect_in_branch, virtual_call_in_ctor.*, java.ownership.*, java.console.platform_equivalents.* (by name; see the flagged list for the 9 whose literals were not all found); [P] 3 -- java.types.a_byte_scalar_is_signed_and_its_buffer_unsigned, java.init_driver.a_dependency_runs_before_its_dependent, java.init_driver.the_seed_order_of_a_profile_is_kept
- [P] test_build3_fixes (5) -- java.corpus.a_static_shadowing_an_inherited_instance_method_is_renamed, a_static_with_nothing_to_shadow_is_not_renamed, a_static_overload_beside_an_instance_override_is_not_renamed; java.emitted.an_array_field_initialised_to_null_is_left_empty, an_array_field_assigned_null_becomes_the_empty_array (real front end, javac of the JDK)
- [C] beside-the-JVM translations (test_bytes 2, test_finally 1, test_input_io 1, test_java_semantics 2, test_null_model 1, test_strings_regex 1, two_phase block_opacity 1 = 9) -- translate.java.*, translate.java_libc.*, translate.java_objects.*, translate.javajudge.*, translate.judge_native_jvm.* (same behaviour, by name)
- [P] emitted-C++ cases (13): call_arg_cast 1, ctor_delegation_cast 1, int_array_unbox 2, noequals_identity 2, ptr_factory 1, int_cast 1 -- java.emitted.* (translate/java_ports_test.cpp: Java source -> the application's assembly with the real javac front end -> the emitted C++ text; int_cast compiles the class with g++ and runs it: Java's saturated values); collection_out_params 5 -- java.corpus.* + java.ownership.a_promoted_collection_is_a_reference_in_the_signature
- [C] test_controller_nav (11) -- java.minecraft.controller_nav.*; test_edition_* (20) -- java.minecraft.edition.*, java.minecraft.package.profile_for_follows_edition_then_version_then_source
- [P] test_emit (12), test_emit_source (22) -- java.emit_ports.* (java/emit/emit_ports_test.cpp: the same classes, resolvers and substrings, one native test each; the Python built the IR by hand, here the Java that parses to it; bodies are the source test's stand-in) and java.emitted.a_nested_static_aggregate_array_is_not_a_braced_new (the real translator). Were by name only.
- [P] test_host_compile_fixes (5) -- java.emitted.host_compile_fixes_text (4 text checks in one) and java.emitted.every_class_and_the_runtime_compile (g++ over Probe, Inflate and the runtime unit, then runs it)
- [C] test_ir (11) -- java.ir.*; test_kind (5) -- verify.lanes.java.*; test_parse (11) -- java.parse.*
- [P] test_pc_platform (3) -- java.pc_platform.the_pc_layer_replaces_the_stubs (passes); it_builds_and_runs and sound_saving_and_resources_work build with g++ and SDL2 but SKIP here: without LWJGL / the sound system on javac's classpath the game's bodies are unsupported (throw at start-up), as the Python skipped without LWJGL
- [C] test_pipeline (19) -- java.transpile.*, java.report.*, java.runtime.file_sets_match_python, java.minecraft.raycast.*, java.prune.*, java.overlays.*
- [C] test_platform_config (12) -- java.console.sdk.*, java.console.platform_config.*; test_platform_coverage_symbols (3) -- java.minecraft.platform_coverage.*; test_platform_refs (5), test_runnable_entry (6) -- java.platform_refs.*
- [C] test_probe* (14) -- java.probe.*; test_profile_invariants (5), test_profiles_selection (4) -- java.profiles.*, java.minecraft.editions.*, xbox360.*
- [C] test_project_layout (8), test_project_scan (4), test_projects (4), test_vcxproj_filters (4), test_ui_mark (7) -- java.console.project_layout.*, project_scan.*, java.projects.*, java.console.vcxproj.*, java.minecraft.ui_mark.*, java.debug.*
- [C] test_reflection_factory (7) -- java.reflection_factory.*; test_render_fix (5) -- java.minecraft.render_fix.*
- test_review_fixes (11): [C] 5 -- controller_nav anchors, convert restores, applet_ping registered, alpha/beta edition (java.minecraft.controller_nav.*, java.run.tool_table, java.minecraft.edition.detect); [P] 6 -- java.platform.tochararray_and_getbytes_memory_is_bounded, save360_shutdown_releases_the_mount_gate, save360_unwritable_mount_is_closed_before_the_fallback, javamathsqrt_resolves_atan2_with_a_float_argument, toggling_alpha_test_does_not_reset_the_alpha_ref, the_viewport_height_comes_from_the_backbuffer_and_y_is_clamped (the platform layer's C++ read and compiled with g++)
- [C] test_rules (13) -- java.rules.*; test_runtime_stubs (9) -- java.stubs.*, java.runtime.*; test_scaffold (5) -- java.scaffold.*; test_two_phase (4 of 5) -- java.twophase.*; test_types (19) -- java.types.*

# Counts (final, 2026-11-13, second pass)

Python tests/checks listed: 410 (29 suites + tests/java 375 test functions by the literal check; the first pass counted 330 java tests by name -- parametrised and class-nested ones differ).

Closed in this pass (all green in `area` builds: source 418, decompile 189, emulate 269, verify 292, formats 558, engine 91, java 557, translate 405 passed, 0 failed):
- test_concolic 3 -> ported (emulate.windows16.a_run_with_its_shadow_is_the_same_run, verify.win32.a_32_bit_run_with_its_shadow_is_the_same_run, verify.win32.the_explorer_finds_every_path_of_lzopenfile)
- test_match_powerpc 4 -> ported (source.pipeline_ppc.*, with the Xbox 360 compiler)
- test_frozen 12 recorded cases -> ported (decompile.c_text.frozen_* 5, formats.frozen.* 7)
- native_engine_golden.txt -> ported (engine.engine_golden..., hashes included); native_trace_golden.txt -> ported in the form the digests allow (the cases that gave one record give one record; the digests themselves are hashes of Python text)
- java gap tests: 35 ported (beta_fixes 3, build3 5, emitted-C++ 13, host_compile 5, pc_platform 3 -- 2 of them skip without LWJGL / the sound system on javac's classpath, review_fixes 6) -- translate/java_ports_test.cpp (33 native tests)
- test_emit 12 + test_emit_source 22 (were by name) -> ported one for one (java/emit/emit_ports_test.cpp, 33 native tests, + 1 in java_ports_test.cpp)
New native tests: 83 in six new files + 3 (concolic) + 1 (trace golden) added to existing files = 87.

The by-name java matches (the ~55 of the first pass; in all 375 java tests): checked by a LITERAL CROSS-CHECK, not side by side -- every string an assertion or call of a Python test names (6+ characters) looked up in native/cb/java, translate, verify, engine, platforms (sources and testdata): 230 of 375 tests have every literal found natively (confirmed), 34 more are the emit ports above, ~11 are the gap ports above; 114 have literals missing (state/work/typed-port/test_port_map_flagged.txt, test by test, with how many literals): most are inputs the native tests spell differently (a golden file holds the whole text, a skip reason, a file name), so the check is a lower bound, not a finding -- they stay [C by name] and are NOT individually confirmed. Highest-risk among them for a follow-up read: test_pipeline (12: pruning, overlays, forwarders), test_two_phase constructors_become_j2xinit, test_beta_fixes header/dtor ownership (9), test_project_layout (7), test_ui_mark (5), test_runnable_entry (5), the "beside the JVM" differentials (test_bytes, finally, input_io, java_semantics, null_model, strings_regex, probe*: native twins are translate.javajudge*/judge_native_jvm, whose fixtures are not these programs).

Not ported, with reason: nothing of the 38 listed gaps is left. Findings: (1) the emitted PC programs (pc_platform) abort at start-up with UnsupportedOperationException when LWJGL is not on the front end's classpath -- the two run tests skip; whether the native front end is given the LWJGL stand-ins by the application was not checked. (2) native_trace digests cannot be replayed (Python text); (3) a worktree build finds pcodecheck in the main checkout (win32 judge tests that look for it beside native/cb skip in a worktree; the new tests search upward like pcode_test does).


# Third pass (2026-11-13): the 114 flagged tests/java functions, each read against its native test

Every test of state/work/typed-port/test_port_map_flagged.txt was read next to its native counterpart (the Python test and the native test, side by side). The list of
literals missing natively (a script over the flagged tests, scratch) showed most were file names, skip reasons and printf-style messages; where they were behaviour
the native test either checks the same behaviour on the same kind of input (CONFIRMED, cited) or was missing and was ported (PORTED: same inputs, same expectations).
Status: [CONFIRMED] a native test checks it (cited), [PORTED] a new native test (file::test), [N] not applicable (why). New native test files:
native/cb/java/pipeline_ports_test.cpp, native/cb/java/beta_fixes_ports_test.cpp, native/cb/java/console/console_ports_test.cpp; additions to
native/cb/translate/java_ports_test.cpp, native/cb/java/transpile_test.cpp, native/cb/java/probe_test.cpp, native/cb/java/projects/minecraft/{achievements,edition}_test.cpp,
native/cb/java/projects/minecraft/profiles/minecraft_editions_test.cpp.

## highest risk first
- test_pipeline (12): collision_renamed [PORTED] java.pipeline.a_method_named_like_a_field_is_renamed_in_the_header (translate/java_ports_test.cpp, real front end); files_written + translation_applied [PORTED] java.pipeline.a_class_gives_header_and_source_and_an_interface_only_a_header; driver_becomes_raycast [PORTED] java.pipeline.the_pick_driver_becomes_a_raycast (+ helpers_emptied, guard, no_op); the five prune tests [PORTED, also in java.prune.tree_passes_match_python as one golden] java.pipeline.prune_cuts_unreached_keeps_reachable_and_linkcritical, prune_overlay_written_files_are_roots, prune_overlay_roots_accept_nested_relpaths, prune_a_header_drags_its_cpp_partner_and_its_includes, prune_a_cross_named_implementation_is_kept_by_its_symbols; the three forwarder tests [PORTED] java.pipeline.forwarder_is_folded_and_includes_are_rewritten, a_chain_of_forwarders_resolves_to_the_end, a_header_with_a_body_is_not_a_forwarder.
- test_two_phase (2): constructors_become_j2xinit [PORTED] java.twophase.the_constructors_become_j2xinit (Block/BlockGlass/Plain through the real converter: C() + j2xInit, Block::j2xInit(var1, 2), (new BlockGlass())->j2xInit(, Plain keeps its constructor); block_opacity_beside_the_jvm [PORTED] java.beside_the_jvm.a_glass_block_is_not_opaque.
- test_beta_fixes (9): header_has_singletons_and_fields [CONFIRMED] java.header.typed_enum (the same typed enum: singletons, field, method, exact text); marker_and_bridge_ctor [CONFIRMED] java.corpus.markers_that_are_bases_stay (marker dropped, base kept, bridge ctor dropped); container_of_new_owned [CONFIRMED] java.ownership.owned_containers (items/mixed/listeners); dtor_frees_scalars_and_container [PORTED] java.beta_fixes.the_destructor_frees_scalars_and_containers_it_owns; getcubes_owned_return + borrowed_return [CONFIRMED] java.ownership.returned_ownership (cubes owned, field borrowed) + program_wide_catalogues; gl11 constants, gl11 functions, nvfogdistance [PORTED] java.beta_fixes.the_gl11_stub_has_the_constants..., ..._functions..., nvfogdistance_is_a_static_holder_with_its_constants.
- test_project_layout (7): flat_dump_gets_structured [PORTED] java.console.ports.a_flat_dump_is_structured_with_a_bound_solution (also the golden java.console.project_layout.matches_the_original); sln_binds_to_project_and_guid [PORTED, same test]; template_instantiates_to_valid_xml [PORTED] java.console.ports.the_template_filled_in_is_well_formed_xml (no minidom natively: comments closed and without "--", tags balanced); idempotent_no_triple_nest [PORTED] java.console.ports.a_second_layout_moves_nothing_and_never_nests_again; guid_is_deterministic [CONFIRMED] java.console.project_layout.pieces; existing_vcxproj_guid_is_reused [PORTED] java.console.ports.a_project_that_has_its_own_guid_keeps_it; already_structured_is_left_alone [PORTED] java.console.ports.sources_already_in_the_project_folder_stay.
- test_ui_mark (5): all [CONFIRMED] java.minecraft.ui_mark.discover (by name, base, transitively), mark_and_index (marker inserted, a call is not a definition, index lists the method), idempotent (no double marker, skipped counted), dry_run (nothing edited, still reported).
- test_runnable_entry (5): detects_concrete / abstract_ignored / ui_with_superclass / ambiguous [CONFIRMED] java.platform_refs.detect_runnable_game_class; entry_point_constructs_and_runs [CONFIRMED] runnable_entry_constructs_with_default_arguments + [PORTED] java.console.ports.the_runnable_entry_starts_the_game_up_and_runs_it (j2x_initAll, Display::create, no MinecraftImpl).
- the "beside the JVM" differentials (the translated program and the JVM on the same inputs; they need a JDK and g++ and pass saying so without them; java_ports_test.cpp, namespace java.beside_the_jvm): test_bytes 2 [PORTED] a_constructor_zeroes_the_byte_fields, bytes; test_finally [PORTED] the_finally_counter_comes_back_down; test_java_semantics 2 [PORTED] char_is_unsigned_in_the_emitted_code, empty_arrays_null_handlers_and_unsigned_chars; test_null_model [PORTED] null_strings_lists_maps_and_sets; test_strings_regex [PORTED] strings_and_regular_expressions; test_probe 3: floats_compare_by_bits [CONFIRMED] java.probe.compare, methods_run_beside_the_jvm [PORTED] methods_with_primitives_and_a_char_that_cannot_be_held, a_result_java_itself_does_not_repeat [PORTED] a_result_java_does_not_repeat_is_left_out; test_probe_objects [PORTED] strings_objects_and_number_text; test_probe_sequence [PORTED] cases_left_out_leave_no_state_behind; test_input_io [PORTED] keyboard_names_and_a_missing_resource (needs LWJGL in the settings' java_libs, skips without, as the Python); test_probe_libraries [PORTED] codec_mus_compiles_against_the_stand_in (needs the sound system in java_libs, skips without); test_probe_clock 2 [PORTED] java.probe.a_class_that_reads_the_clock_in_its_constructor_and_a_method (java/probe_test.cpp).

## the rest (the lower-risk flagged)
- test_achievements (7): humanize_names / lang_names_override / title_id_normalize_and_suggest [CONFIRMED] java.minecraft.achievements.humanize_and_const_name, load_lang_names + scan_achievements, normalize_title_id + suggest_title_id; run_emits_all_artifacts (hidden="true") [CONFIRMED] run_matches_the_python_module (expected/achievements.xlast, byte for byte, hidden="true" for the special one); run_writes_title_id [CONFIRMED] title_id_precedence + run_matches_the_python_module; wire_icons_crops_and_writes_tiles [CONFIRMED] run_matches_the_python_module (tiles pixel for pixel, 64x64, locked darker) + tile_pixels_by_hand; block_texture_resolver [PORTED] java.minecraft.achievements.block_textures_come_from_a_subclass_literal_a_second_argument_or_the_base.
- test_apimap (2) [CONFIRMED] java.apimap.qualified_and_unknown_names, extract_symbols_matches_python (golden incl. GL11.GL_TRIANGLES constants). test_assets (6) [CONFIRMED] java.console.assets.matches_the_original (extract mirrors paths, coverage present/missing/extra), extract_options (indexed PNG normalised / not, .class never extracted), case_drift_is_tolerated; xact match_sources [CONFIRMED] java.console.xact.coverage_and_sources_match_the_original, dig_reuses_the_step_samples.
- test_awt_stubs (2) [PORTED] java.awt_stubs.the_mouse_stubs_are_generated_and_registered. test_runtime_stubs (3) [PORTED] java.runtime_stubs.jmap_has_the_java_map_surface_and_list_and_set_an_iterator_member, a_corpus_class_inherits_the_methods_of_a_stub_base.
- test_controller_nav (2) [CONFIRMED] java.minecraft.controller_nav.patch_displayguiscreen, patch_mousehelper (the bodies are replaced wholesale: no AWT call can remain; Mouse::setGrabbed set directly).
- test_edition_autodetect (3) [PORTED] java.minecraft.edition.a_beta_world_with_biome_and_region_markers, the_server_package_makes_a_release, one_unknown_class_is_ambiguous; test_edition_convert_integration (4) [PORTED] java.edition_convert.* (translate/java_ports_test.cpp, the application's assembly: explicit edition, beta detected, nothing conclusive, version hint; "restores the default" is [N]: the profile of a conversion is handed down, no process-wide profile is switched); test_edition_resolve (1) [CONFIRMED] java.minecraft.edition.version_ids (the parametrised ids).
- test_ir (1) [CONFIRMED] java.ir.method_signature. test_parse (1) [CONFIRMED] java.parse.fixtures_match_the_reviewed_golden (every fixture parses to a named class). test_types (1) [CONFIRMED] java.types: JArray<Block* >, JArray<JArray<Block* > >. test_kind (2) [CONFIRMED] verify.lanes.java.the_host_compiler_judges_what_compiles, a_class_that_does_not_compile_is_no_compile...; source.java_writer.* (status labels, STATUS.md); verify.reconstruct (a second run judges nothing). test_scaffold (4) [CONFIRMED] java.scaffold.c_skeleton, csharp_skeleton, rust_skeleton (hand-checked exact text; String -> const char* / string / String; static without self) + languages (an unknown language is no Language).
- test_platform_config (3) [CONFIRMED] java.console.sdk.detect_order, require_gate, platform_config.gate_requires_an_sdk_that_declares_the_symbols, run_uses_a_given_config_path_and_gates_before_writing. test_platform_refs (1) [CONFIRMED] java.platform_refs.entry_points_name_the_detected_class.
- test_profile_invariants (4) [PORTED] java.minecraft.editions.every_profile_satisfies_the_structural_invariants (all six editions + xbox360: filters compile, return-type keys are nullable getters, no duplicate static-init, Rendering before Entities\Types). test_profiles_selection (1) [N] a runtime mutation of the profile's nullable getters cannot leak: there is no process-wide typemap set to reseed (the corpus's getters are derived per conversion; the profile is const).
- test_project_scan (1) [CONFIRMED] java.console.project_scan.jar_choice (deobfuscated.jar preferred over any other jar). test_projects (3) [CONFIRMED] java.projects.matches, choose (by marks, by claimed options, generic otherwise), java.minecraft.package (char_map 0x2302 -> 127) + translate.backends.cpp03 (char_map applied); "a plain program loads no Minecraft code" -- java.transpile.convert_matches_python (profile generic) and java.projects.project_lookup (a manifest without code in the set is UnsupportedError: Minecraft's code is only there when the application registers it).
- test_reflection_factory (3) [CONFIRMED] java.reflection_factory.run_matches_python (World-arg and no-arg registries, reuse in other files with the include injected, a third run changes nothing, golden files reviewed), process_details, reuse_choices.
- test_review_fixes (1) [PORTED] java.transpile.a_conversion_that_fails_midway_leaves_the_profiles_alone (the shared profile set is as it was).
- test_vcxproj_filters (2) [CONFIRMED] java.console.vcxproj.the_real_profile_rules_classify_as_before (Platform\Config for Kernel/KernelConfig/ControllerConfig, Debug), filters_match_the_original, parent_map_matches_the_original.

## Counts, third pass (the 114 flagged tests/java functions)

- CONFIRMED against an existing native test, read side by side: 55
- PORTED (a new native test, same inputs, same expectations): 58 (java area: pipeline 12, beta-fixes/awt/runtime-stub 7 tests, project layout + runnable entry 6, achievements 1, edition 3, profile invariants 1, probe clock 1, transpile profile 2; translate area: 21 -- the "beside the JVM" differentials 14, edition conversion 4, pipeline files 2, two-phase constructors 1)
- NOT APPLICABLE: 1 (test_profiles_selection.test_runtime_mutation_does_not_pollute_profile: the native design has no process-wide typemap set to reseed; the profile is const)
- Left: 0.
- New native tests: java 33 (557 -> 590 passed), translate 21 (405 -> 426 passed), 0 failed in either; the translate-area differentials ran against a real JDK and g++ here and found no difference between the JVM and the converted C++. Two of them (keyboard names/missing resource, CodecMus against the sound-system stand-in) need LWJGL / the sound system in the settings' java_libs, which this machine does not have: they say so and pass, exactly as the Python skipped them -- they are written but have not been run here.
- Findings: (1) no behaviour difference was found between the Python tests and the native code in any ported test (no entry added to _bugs/LIST.md). (2) The native design has no process-wide "active profile" or typemap sets, so the Python tests that restored or reseeded them (edition_convert "restores the default", review_fixes restore-on-raise, profiles_selection) have no native counterpart beyond "the shared ProfileSet is unchanged after a conversion". (3) java.transpile edition tests need the Minecraft project, which the java area's fake-translator world does not register (a manifest without code is an UnsupportedError there): the edition-conversion cases run in the translate area on the application's own assembly.
