# Test port map, half 1 (tests/test_*.py test_analysis..test_platforms, plus tests/java)

2026-11-13. Tags: [C] covered by an existing native/cb test (cited), [P] ported now, [N] not needed (why), [G] gap (not covered, not ported this pass).
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
- [G] a_run_with_its_shadow_is_the_same_run (real LZEXPAND functions, with/without shadow)
- [C] lifter_gives_the_specifications_p_code -- emulate.pcode.lifts_a_16_bit_instruction_in_its_window
- [G] 32_bit_run_with_its_shadow_is_the_same_run (LZ32 exports; shadow vs fast tracer fingerprint)
- [G] 32_bit_explorer_finds_every_path_of_lzopenfile
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
- [G] c_text _far_loads/_global_type/_loads_dgroup/_more_helpers/_ram_type (5): private passes; behaviour in decompile.c_text.shared_selector_structs, ss_store_places, constants, but recorded cases not replayed
- [C] formats ne_lzexpand, pe_lz32, resources_lzexpand, inspect_lzexpand, pe_functions_lz32, cab_mini, fonts_vgafix (7) -- formats.ne.*, formats.pe.lz32_*, formats.resources.the_resource_table_of_a_real_16bit_module, formats.inspect.describe_equals_the_recorded_original, formats.pe_functions.lz32_*, formats.cab.real_windows_95_mini_cab_against_expand, formats.fonts.a_fon_lists_its_fonts
- [G] formats ne_keyboard, pe_notepad, le_vmouse, dos_mode, resources_notepad, detect_vmouse, detect_mode (7): same readers tested on other files, these recorded outputs not replayed
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
- [G] one_function_whole, masks_are_fields, distance, original_stops_at_padding: `match` on PowerPC (verify/authored/match.py ARCH["powerpc"]) has no native module (native has match16, repair, search16, shape only). Port the module, then these.

## test_mcp.py (13)
- [C] -- ui.mcp.the_command_table_is_the_catalog, the_code_is_read_as_the_python_did, the_service_answers_the_tools_by_name, the_names_and_the_runtime_libraries_are_read_as_the_python_did, commands_and_jobs, the_reading_tools_only_when_there_are_no_writes, slow_values_are_worked_out_in_the_background_and_kept, reading_what_was_made, one_function_is_decompiled_now_as_the_python_did, two_builds_are_compared_and_one_function_shown_side_by_side, the_ledger_is_kept_and_served_from_the_kept_copy_while_a_new_one_is_made, the_http_transport_checks_the_token_the_host_and_the_session (one each, same order as the Python tests)

## test_media.py (12)
- [C] cabinet round trips/expand/rejects/checksum/real mszip/real quantum -- formats.cab.build_and_extract_round_trip, what_we_write_expand_reads, limits, checksum, real_windows_95_mini_cab_against_expand, real_quantum_cabinet
- [C] disc image round trip/bootable/name clashes -- build.disc_image.the_images_are_the_pythons, names_are_cut_to_level_one, formats.media.iso_built_disc, disc_with_an_el_torito_floppy_and_the_same_disc_in_raw_sectors
- [C] fat16 in partition / fat32 -- formats.media.fat16_and_fat32_with_a_folder_that_grows, partitions_primary_and_logical

## test_native_engine.py (4)
- [C] same facts and runs/store/knowledge/verdict words -- engine.engine.*, engine.facts.*, engine.function_store.a_store_written_by_the_c_engine_reads_the_same, knowledge.store.digests_follow_the_piece, engine.function_store.older_spellings_read_as_the_words_now (behaviour)
- [G] replay of tests/data/native_engine_golden.txt (Python-literal rows of 4 cases) not done natively

## test_native_formats.py (1) [C] formats.pe.* (truncation/damaged), formats.ne.damage_never_crashes, formats.le.errors_and_damage, formats.dos.errors, reader facts on real files; [N] the Python-attribute digest mechanism

## test_native_trace.py (6)
- [C] same_record_synthetic/shadow32/driver/win16/lz32/record_has_what_was_run -- emulate.native_trace.*, emulate.native_shadow.*, emulate.native_driver.*, emulate.native_win16.*, emulate.emulator.the_real_lz32_dll_traces_the_same_fresh_and_kept (behaviour)
- [N] python_raises_through (Python hook plumbing); [G] tests/data/native_trace_golden.txt digests not replayed

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
- test_beta_fixes (35): [C] 32 -- java.header.typed_enum, constant_enum, java.rules.hotpath_alloc, silent_dispatch, infinite_loop, side_effect_in_branch, virtual_call_in_ctor.*, java.ownership.*, java.console.platform_equivalents.*; [G] 3: scalar_signed_buffer_unsigned, dependency_ordered_before_dependent, seed_priority_preserved
- [G] test_build3_fixes (5): static shadowing renames; array field null init/reassign
- [C] beside-the-JVM translations (test_bytes 2, test_finally 1, test_input_io 1, test_java_semantics 2, test_null_model 1, test_strings_regex 1, two_phase block_opacity 1 = 9) -- translate.java.*, translate.java_libc.*, translate.java_objects.*, translate.javajudge.*, translate.judge_native_jvm.* (same behaviour, by name)
- [G] emitted-C++ cases: call_arg_cast 1, ctor_delegation_cast 1, int_array_unbox 2, int_cast 1, noequals_identity 2, ptr_factory 1, collection_out_params 5 (13) -- java.ownership.*, translate.cpp03.* cover by area
- [C] test_controller_nav (11) -- java.minecraft.controller_nav.*; test_edition_* (20) -- java.minecraft.edition.*, java.minecraft.package.profile_for_follows_edition_then_version_then_source
- [C] test_emit (12), test_emit_source (22) -- java.header.*, java.emit.naming.*, java.source.cases_match_python (by name; goldens of the same IR shapes)
- [G] test_host_compile_fixes (5) -- host-compile of emitted C++, no native twin (includes half: java.runtime.every_canonical_header_is_wired)
- [C] test_ir (11) -- java.ir.*; test_kind (5) -- verify.lanes.java.*; test_parse (11) -- java.parse.*
- [G] test_pc_platform (3) -- builds/runs the PC platform layer with g++
- [C] test_pipeline (19) -- java.transpile.*, java.report.*, java.runtime.file_sets_match_python, java.minecraft.raycast.*, java.prune.*, java.overlays.*
- [C] test_platform_config (12) -- java.console.sdk.*, java.console.platform_config.*; test_platform_coverage_symbols (3) -- java.minecraft.platform_coverage.*; test_platform_refs (5), test_runnable_entry (6) -- java.platform_refs.*
- [C] test_probe* (14) -- java.probe.*; test_profile_invariants (5), test_profiles_selection (4) -- java.profiles.*, java.minecraft.editions.*, xbox360.*
- [C] test_project_layout (8), test_project_scan (4), test_projects (4), test_vcxproj_filters (4), test_ui_mark (7) -- java.console.project_layout.*, project_scan.*, java.projects.*, java.console.vcxproj.*, java.minecraft.ui_mark.*, java.debug.*
- [C] test_reflection_factory (7) -- java.reflection_factory.*; test_render_fix (5) -- java.minecraft.render_fix.*
- test_review_fixes (11): [C] 5 -- controller_nav anchors, convert restores, applet_ping registered, alpha/beta edition (java.minecraft.controller_nav.*, java.run.tool_table, java.minecraft.edition.detect); [G] 6 -- tochararray/getbytes memory, save360 mount gate x2, Math.sqrt/atan2, alpha_test toggle, viewport clamp (platform-layer C++ sources)
- [C] test_rules (13) -- java.rules.*; test_runtime_stubs (9) -- java.stubs.*, java.runtime.*; test_scaffold (5) -- java.scaffold.*; test_two_phase (4 of 5) -- java.twophase.*; test_types (19) -- java.types.*

# Counts
Python tests/checks listed: 410 (suites 29 + java). Covered about 360 (incl. ~60 java matched by name only), ported 8 (6 cases: aim, standin, 2 assembler, nes lane, genesis lane = 6 tests covering 6 Python tests; the assembler 2 cover 1), not needed 6, gaps below.
GAP list (follow-up ports): concolic x3, match_powerpc x4, frozen 12 recorded cases, native_engine/native_trace golden replays 2, java 38 (beta_fixes 3, build3 5, emitted-C++ 13, host_compile 5, pc_platform 3, review_fixes 6, plus pipeline-level none).
