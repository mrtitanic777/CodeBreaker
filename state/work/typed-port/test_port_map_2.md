# Test port map, half 2 (suites test_plumbing .. test_z80, and tests/translate)

How this was made: every Python test of the suites below was read (name, docstring and, where the name did not settle it, the
body). Native tests were found in native/cb/**/*_test.cpp by name and, for the doubtful ones, by grepping the bodies for the test's
inputs (function names, byte values, spellings). Test ids are `area.module.case` as `cb_tests --list` prints them.
Status words (one per test, counted at the end):

- COVERED   a native/cb test already checks the behaviour (cited). Many native tests say in their header that they are the port of that
            Python test (a "DIVERGES from the Python" note names what was dropped on purpose).
- PORTED    written in this task (cited); commits `cb: analyze/origin ...`, `cb: formats/omf ...`, `cb: translate/native_l2 ...`.
- NOT NEEDED  tests Python-only plumbing (the Python translator / py:: layer / the Python process model / the terminal UI, which stays
            Python), or a tool-presence check; the reason is given.
- BLOCKED   the behaviour belongs to a module that does not exist in native/cb yet (only as the typed port under native/typed): there
            is nothing to write a native test against. Listed for whoever refactors that module; the Python test is its spec.

Confidence: COVERED citations are by test name plus a grep of the test body for the Python test's own inputs where the name was not
enough. Where two native tests share the work both are cited.

## Findings worth knowing
- tests/translate/test_c_backend.py (43 tests), test_l2_survives.py (4) and the C-back-end parts of test_frame16.py asked the L2 library
  (cbl2.dll) questions that native/cb only tested for 3 passes (translate.native_l2.passes_moved_onto_the_tree). 48 Python tests
  (132 library calls) are now replayed from recorded cases: native/cb/translate/native_l2_cases_test.cpp and
  testdata/c_backend_cases.json (recorder: state/work/typed-port/t2/record_l2.py; only passing Python tests are recorded).
  The replay tests skip (and say so) when cbl2.dll is not built, like the other native_l2 tests.
- Two Python tests do not pass today in this tree (test_module_model: test_emit_makes_each_function_a_fragment and
  test_the_nasm_writers_make_each_function_a_fragment: `from codebreaker.compare import image` fails because compare/image moved to
  the typed port). Not a native problem; logged in _bugs/LIST.md.
- The Xbox formats xdbf (GPD achievements), xpr, xur, xbe, bink and chd are native/cb modules now (native/cb/formats/xbox/{xdbf,xpr,xur,xbe,bink,chd}.hpp, summary.hpp for `info`), tested on the Python's own inputs with the Python's answers as goldens (testdata/assets/make_asset_goldens.py) and on the Minecraft title's real SPA and XUR scenes; the six BLOCKED tests below are PORTED. xwb, xma and xsb are native/cb/xna/xact.
- pe_writer's `nosplit` rewrite of a scaled index (test_pe_writer_nosplit_keeps_the_scaled_index): the Python bug (control characters
  in a regex replacement) cannot recur in the C++ (`"[$1nosplit $2"`), but no native test reaches the line either (Image::nasm_text is
  private, and neither golden PE has a `[reg*scale` operand at the start of the brackets). Left as NOT NEEDED with that note.

---------------------------------------------------------------------------------------------------------------------------------
## tests/test_plumbing.py (18)
- test_decompiler_is_built -> NOT NEEDED: checks the native decompiler binary exists (an environment check; cb_tests link the code).
- test_openwatcom -> NOT NEEDED: tool-presence check; the code that needs OpenWatcom is tested through build.module.build_without_openwatcom_is_a_usage_error_and_a_foreign_kind_is_refused and build.driver.build_a_driver_with_openwatcom.
- test_corpus_rank_prefers_installed_copy -> COVERED: base.corpus.the_copy_the_os_loads_comes_first
- test_corpus_retail -> COVERED: base.corpus.retail_resolve_and_paths, base.corpus.a_bare_name_tries_the_usual_extensions
- test_corpus_modules -> COVERED: base.corpus.modules_smallest_first_by_lane_and_extension
- test_format_detection -> COVERED: formats.detect.identify_by_magic, dos_header, new_header_from_bytes, pe_machine_and_damaged_headers
- test_isolated_separates_failures_and_resumes -> COVERED: jobs.pool.isolated_separates_failures_and_resumes
- test_threads_keep_order_and_raise -> COVERED: core.parallel.parallel_map_keeps_order, core.parallel.parallel_for_rethrows_first_error_after_finishing
- test_throttle_is_safe -> NOT NEEDED: the Python wrapper that lowers a process's priority; the C++ side has no such wrapper (core.process / base.parallel.idle_cores_is_between_none_and_all cover what remains)
- test_workers_scale_and_low_load_caps -> COVERED: core.parallel.workers_policy, base.parallel.slots_hold_at_least_one_and_no_more_than_the_workers
- test_processes_safe_reads_the_guard -> NOT NEEDED: Python's `__main__` guard for multiprocessing; C++ workers are separate programs (jobs.workers.*)
- test_fast_unicorn_reads_are_the_same -> NOT NEEDED: emulate/fast_uc.py is a Python-binding speed-up; the C++ calls Unicorn directly (emulate.unicorn_uc.*)
- test_cache_drops_older_generations -> COVERED: base.cache.another_generation_goes_when_the_kind_is_first_used
- test_cache_holds_its_cap_least_recently_used_first -> COVERED: base.cache.hold_drops_the_least_recently_used_first
- test_fingerprints_are_kept_in_the_cache -> COVERED: verify.compare.fingerprints_round_trip_through_the_cache
- test_run_folders_are_deleted -> COVERED: base.runs.sweep_removes_only_old_run_folders, base.runs.folder_is_deleted_at_scope_end_even_by_an_exception
- test_tidy_state_finds_the_stale -> COVERED: base.tidy_state.stale_lists_each_rule_and_only_what_is_old, run_removes_the_stale_and_keeps_the_rest, dry_run_reports_and_removes_nothing

## tests/test_port.py (11)  (old-C -> modern C port, native/cb/port)
- test_runtime_left_out -> COVERED: port.program_prep.the_old_librarys_files_are_left_out, port.rewrite.preprocess_points_the_old_library_at_port_h
- test_prototypes_and_widths -> COVERED: port.rewrite.old_style_definitions_become_prototypes, port.old_c_text.widths_long_is_int32_and_struct_members_are_words, port.headers.port_h_proto_h_bss_c_and_data_c
- test_formats -> COVERED: port.old_c_text.formats_give_a_long_arguments_the_cast_its_format_says
- test_frame_kept_side_by_side -> COVERED: port.frames.local_decls_in_order_and_their_old_sizes, port.frames.locals_a_call_reads_past_an_array_are_one_structure
- test_widening_passes_on_a_definition_that_never_closes -> COVERED: port.passes.whole_word_stores, port.mend.stored_wide_keeps_declarations_changed_for_an_earlier_file
- test_stored_wide_keeps_the_declarations_it_changed -> COVERED: port.mend.stored_wide_keeps_declarations_changed_for_an_earlier_file
- test_builds_and_runs -> COVERED: port.tool.a_build_with_one_command, port.tool.the_build_script (the run of the built program is the port.verify.* lane)
- test_octal_constant_in_a_pass -> COVERED: port.old_c_scan.integers_are_hex_or_decimal_and_whole, port.old_c_data.fixed_allocators_case
- test_size_table_ends_at_its_own_brace -> COVERED: port.old_c_data.a_size_table_ends_at_its_own_brace, port.old_c_data.a_table_that_never_closes_has_no_rows

(test_port.py defines 9 checks run by main(); the two helper-only functions are not tests. Counted as 9 below.)

## tests/test_port_fixes.py (17 run by main())
- test_pe_writer_nosplit_keeps_the_scaled_index -> NOT NEEDED: the Python bug was control characters in a regex replacement string; the C++ writes the literal "[$1nosplit $2" (source/pe_writer.cpp). No native test reaches it: see Findings.
- test_cpukey_generated_keys_validate -> COVERED: formats.xbox.crypto.xecrypt.generated_keys_are_valid_and_seeded_runs_repeat, formats.xbox.crypto.tool_goldens.cpukey_ecd_agrees_with_the_python_on_its_verdicts
- test_sms_writer_game_gear_rom -> COVERED: source.sms_writer.the_game_gears_own_port_is_named_for_a_game_gear_only
- test_sd_signer_keeps_its_key_in_the_module -> COVERED: formats.xbox.crypto.sd_signer.signs_pads_and_zeroes_the_nonce, sd_signer.the_command_signs_a_file
- test_streamio_offset_labels_and_perform_function_at -> COVERED: formats.xbox.crypto.stream_io.labels_and_functions_at_an_offset
- test_apply_jump_sd_4bl_writes_the_branch -> COVERED: formats.xbox.crypto.build_lib.the_4bl_jump_branches_to_the_end_of_the_image
- test_xsm3_challenge_verify_answers_a_valid_packet -> COVERED: formats.xbox.crypto.tool_logic.xsm3_session_answers_the_challenge_the_way_the_python_does
- test_candidates_localize_twice_lays_out_the_merged_struct_array_the_same -> COVERED: exact.draft.candidates.localize_names_the_routines_and_the_data
- test_forms_library_macros_keep_the_include_a_macro_does_not_name -> COVERED: exact.draft.forms.keeps_the_include_a_macro_does_not_name
- test_forms_register_temps_asks_for_the_whole_register_name -> COVERED: exact.draft.forms.register_temps_names_the_whole_variable
- test_forms_default_after_keeps_the_statements_after_the_default_cases_return -> COVERED: exact.draft.forms.* (default_after cases in exact/draft/forms_test.cpp)
- test_java_rules_findings_of_one_line_come_in_name_order -> COVERED: java.rules.virtual_call_in_ctor.this_arrow_and_bare_calls_are_flagged (sorted-order check), java.rules.map_read_insert, java.rules.leak_drop_no_delete
- test_data_declared_size_takes_the_first_declaration_by_name -> COVERED: exact.data (declared_size is over a std::set: exact/data_test.cpp declared-size cases)
- test_segdiff_compile_group_leaves_no_work_folder -> COVERED: source.ne_asm.segdiff.a_group_is_compiled_and_cut_into_functions, base.runs.folder_is_deleted_at_scope_end_even_by_an_exception
- test_origin_notes_calls_to_compiler_helpers -> PORTED: analyze.origin.a_call_to_a_compiler_helper_is_noted_near_or_far
- test_native_dosexec_loads_the_dll_once_from_two_threads -> COVERED: exact.emu.native_dosexec.the_dlls_are_found_by_environment_or_in_the_checkout (the C++ loader is a function-local static; no two-thread race to test)
- test_draft_shapes_store_joins_at_the_end_of_the_text_finds_nothing -> COVERED: exact.recut / exact.draft.shapes_test.cpp (a store_joins case on a text with only closing braces after the store; DIVERGES note in exact/recut_test.cpp)

## tests/test_powerpc.py (1 script, 17 compiled-C cases)
- the compiled-C differential (arith divide udivide divconst shifts wide uwide compare narrow loop choose fib memory real single store calls) -> COVERED in kind: emulate.powerpc.add_and_immediates, multiply_and_divide, shifts_and_counts, rotates, compares_set_cr_fields, floating_point, loads_and_stores, branches_and_link, run_calls_out_through_externs, plus recompile.powerpc.every_instruction_as_the_reference_makes_it. The harness itself needs a PowerPC C compiler named in settings.json (the script skips without one), so its compiled-C half is not reproduced; the interpreter's behaviour per instruction family is.

## tests/test_ppc_calls.py (3)
- test_register_writes -> COVERED: analyze.ppc_calls.what_an_instruction_writes, register_tokens, floating_point_registers_are_tracked_apart
- test_consensus -> COVERED: analyze.ppc_calls.consensus_of_call_sites, arity_of_a_call_site_counts_argument_registers_set_without_a_gap
- test_real_imports_match_their_prototypes -> COVERED: analyze.ppc_calls.arity_of_internal_functions_and_imports, analyze.ppc_calls_command.a_module_is_analysed_like_the_python_did

## tests/test_readable.py (4)
- test_constants -> COVERED: source.readable.constants
- test_one_function -> COVERED: source.readable.one_function
- test_comparisons_fields_and_strings -> COVERED: source.readable.comparisons_fields_and_strings, source.readable.calls_are_read_with_their_arguments
- test_same_object_code -> COVERED: source.readable.proof

## tests/test_recompile.py (7)
- test_integer_leaves -> COVERED: recompile.arith.every_case_matches_the_reference, recompile.logical.every_case_matches_the_reference
- test_floating_point -> COVERED: recompile.vector.float_arithmetic_flushes_denormals_first, recompile.powerpc.every_instruction_as_the_reference_makes_it
- test_loop_over_memory -> COVERED: recompile.memory.every_case_matches_the_reference (big-endian loads), recompile.powerpc.the_slice_of_real_code_as_the_reference_makes_it
- test_indirect_call_table -> COVERED: recompile.jumptable.absolute_table_with_cmplwi_bound, recompile.powerpc.a_switch_is_a_cxx_switch
- test_vmx128_first -> COVERED: recompile.vector.vmx128_misidentified_unpack_forms, recompile.disassemble.vmx128_primary_opcodes
- test_whole_image -> COVERED: recompile.powerpc.an_image_as_the_reference_makes_it, recompile.powerpc.an_image_meaning
- test_discover_built_addresses -> COVERED: recompile.discover.constants_built_by_lis_and_addi_or_ori, recompile.discover.a_bl_chain_is_followed_to_its_end

## tests/test_recut.py (4)
- test_a_fixups_location_is_four_bits -> COVERED: exact.recut header note (location read as 4 bits) + exact.fixupp.parse_explicit_fixup, exact.fixupp.encode_is_what_parse_read
- test_recut_cuts_where_the_relocations_run_up_again -> COVERED: exact.recut.cuts_where_the_relocations_run_up_again, exact.recut.real_objects_as_the_python_recut_them
- test_recut_leaves_a_module_whose_relocations_are_not_the_programs -> COVERED: exact.recut.left_alone_when_the_relocations_are_not_the_programs
- test_order_is_the_packed_ones_or_the_headers_table -> COVERED: exact.recut.relocation_order

## tests/test_run_record.py (5)
- test_what_a_run_left_changed -> COVERED: verify.lane_common.changed_keeps_what_differs, changed_names_places_and_skips, changed_respects_endianness_most_and_everything; analyze.observe.digest_counts_what_a_run_does_to_each_buffer
- test_a_fingerprint_from_a_record -> COVERED: verify.lane_common.fingerprint_of_a_returned_run, fingerprint_of_a_run_that_did_not_return
- test_every_runner_returns_the_record -> COVERED: emulate.driver.the_run_record_carries_what_the_judge_compares, emulate.m68k.record_is_the_run_record, emulate.sm83.record_is_the_run_record, emulate.z80.record, emulate.powerpc.run_returns_and_counts
- test_the_32_bit_judge_s_memory_digest_is_as_it_was -> COVERED: verify.win32.judge_lz32, verify.driver.a_fingerprint_is_what_the_python_recorded
- test_the_16_bit_judge_s_memory_digest_is_as_it_was -> COVERED: verify.win16.a_fingerprint_is_what_the_python_recorded

## tests/test_service.py (11)
- test_in_process_without_a_service -> COVERED: ui.service.a_link_asks_the_service_when_it_runs_and_answers_here_when_not, ui.service.the_mode_is_read_from_the_environment
- test_started_on_demand -> COVERED: ui.service.a_job_runs_in_a_process_of_its_own_and_keeps_its_output, ui.serve.a_state_folder_has_one_fixed_port_the_python_service_picked_too
- test_the_token_is_needed -> COVERED: ui.mcp.the_http_transport_checks_the_token_the_host_and_the_session, ui.serve.the_wire_is_the_python_services
- test_queries_answered_by_it -> COVERED: ui.service.a_call_by_name_is_answered_and_its_failures_are_said, ui.service.the_client_talks_the_wire_of_the_service
- test_a_job_runs_in_it -> COVERED: ui.service.a_job_runs_in_a_process_of_its_own_and_keeps_its_output, ui.service.starting_a_job_on_a_module
- test_one_instance_per_state_folder -> COVERED: ui.serve.a_second_host_takes_the_next_port_when_the_first_is_held, ui.serve.a_state_folder_has_one_fixed_port_the_python_service_picked_too
- test_facts_through_it -> COVERED: ui.service.a_query_command_prints_into_the_answer
- test_mcp_tools_forwarded -> COVERED: ui.mcp.the_service_answers_the_tools_by_name, ui.mcp.tools_called_through_the_wire_answer_as_the_python_did
- test_gone_then_in_process -> COVERED: ui.service.a_link_asks_the_service_when_it_runs_and_answers_here_when_not
- test_idle_exit -> COVERED: ui.serve.it_exits_when_idle_and_when_its_program_changed
- test_stale_when_the_code_changes -> COVERED: ui.serve.a_code_stamp_changes_with_the_program_and_the_settings, ui.serve.it_exits_when_idle_and_when_its_program_changed

## tests/test_store.py (3)
- test_a_modules_rows_are_in_its_database -> COVERED: engine.function_store.record_and_read_candidates, engine.function_store.the_summary_follows_every_write, store.functions.the_overview_and_the_latest_classes_are_read_from_the_store
- test_an_older_store_is_carried_over_once -> COVERED: store.migrate_state.the_knowledge_base_comes_over_without_replacing_what_is_held, store.migrate_state.verdicts_follow_the_settings_that_are_the_same
- test_the_cap_leaves_the_verdicts -> COVERED: engine.facts.a_database_with_the_function_stores_tables_loses_only_its_facts, engine.facts.the_cap_removes_the_oldest_closed_database

## tests/test_symbols.py (11)
- test_demangle -> COVERED: symbols.demangle.* (constructors_and_destructors, data_and_special_names, command)
- test_definition_file_and_library -> COVERED: build.definitions.win16_definition_file, build.definitions.win32_definition_file, symbols.libraries.coff.archive_labels_and_shapes
- test_ordinals -> COVERED: symbols.ordinals.collect, collect_earlier_files_win_and_bad_files_are_skipped, collect_refuses_a_table_the_code_disagrees_with
- test_prototypes -> COVERED: symbols.prototypes.api_macros_and_windows_line_endings, com_declarations, later_folder_names_parameters_and_adds_functions
- test_signatures -> COVERED: symbols.api_arguments.api_names, resolve_follows_imports_and_forwarders
- test_coff_long_symbol_names -> COVERED: symbols.libraries.coff.damaged_objects, archive_that_is_cut_or_damaged (and the long-name table cases in symbols/libraries/coff_test.cpp)
- test_wine16_reads_the_folder_once -> COVERED: symbols.wine16.specs.a_wine_folder, symbols.wine16.kinds
- test_identify -> COVERED: symbols.identify.a_16_bit_module_is_identified_by_signature_with_segment_and_offset_addresses, a_dos_program_has_no_functions_to_identify
- test_store_identified_and_bodies -> COVERED: store.knowledge_bodies.what_is_recorded_reads_back_and_is_replaced, a_name_given_to_a_body_holds_in_every_module_with_it
- test_names_of_a_module -> COVERED: symbols.names.a_16_bit_module_names_what_it_exports, a_32_bit_module_names_its_exports_and_its_imports
- test_source_files_from_symbol_files -> COVERED: symbols.debug_build.* and verify.match16.source_files

## tests/test_translate.py (0 tests: a runner)
- runs tests/translate/ under pytest -> NOT NEEDED: pytest runner shim; each file of tests/translate is mapped below.

## tests/test_ui.py (10)
- test_describe_32bit -> COVERED: formats.inspect.describe_a_pe_agrees_with_the_reader, describe_equals_the_recorded_original
- test_describe_16bit_driver_and_dos -> COVERED: formats.inspect.describe_an_ne_agrees_with_the_reader, describe_a_driver_and_a_dos_program
- test_describe_never_raises -> COVERED: formats.inspect.damaged_files_are_described_never_thrown, describe_what_is_not_an_executable
- test_strings_bytes_and_resources -> COVERED: formats.inspect.strings_are_the_8_bit_and_16_bit_runs_in_file_order, hexdump_reads_a_window_of_a_file, resource_bodies_are_shown_as_what_they_are
- test_info_command -> COVERED: formats.inspect.info_shows_a_summary_and_the_parts, info_refusals_and_exit_codes, info_markdown_folder_and_unpack
- test_ledger_agrees_with_the_store -> COVERED: ui.mcp.the_ledger_is_kept_and_served_from_the_kept_copy_while_a_new_one_is_made, store.versions_ledger.*
- test_ledger_reads_without_writing -> COVERED: engine.function_store.read_only_stores_read_and_never_write
- test_progress_reading -> COVERED: ui.progress.snapshot_rate_and_eta, ui.progress_store.results_in_the_store_are_the_progress
- test_service_reads_the_store -> COVERED: ui.service.a_functions_answer_is_the_page_document, ui.mcp.the_store_and_the_code_come_through_the_services
- test_terminal_interface -> NOT NEEDED: codebreaker/ui/terminal* stays Python (the one interface that is kept)

## tests/test_verdicts.py (6)
- test_the_classes_are_as_they_were -> COVERED: verify.verdicts.class_words_and_order
- test_every_older_spelling_is_read_as_now -> COVERED: verify.verdicts.older_spellings, verify.verdicts.parse_class
- test_each_class_has_one_title_and_one_label -> COVERED: verify.verdicts.class_words_and_order, verify.verdicts.judge_words
- test_a_store_written_before_reads_in_todays_words -> COVERED: engine.function_store.older_spellings_read_as_the_words_now
- test_the_module_writer_reads_a_verdict_by_provenance -> COVERED: verify.verdicts.of_row_and_index, verify.verdicts.judgement
- test_the_producers_speak_it -> COVERED: verify.verdicts.classify, verify.verdicts.good_and_pair

## tests/test_verification.py (46)  (the old Win16/Win32 reconstruction + verification lane)
- test_reconstruct_is_incremental -> COVERED: engine.lanes.a_new_process_asks_again_and_nothing_is_prepared_built_or_judged, engine.lanes.judged_is_the_rows_as_records
- test_function_store -> COVERED: engine.function_store.record_and_read_candidates, best_goes_by_class_then_by_the_engine_named_first
- test_variant_store -> COVERED: engine.function_store.variants_are_compared_by_their_bytes
- test_repair_knobs_preserve_meaning -> COVERED: verify.authored.repair_test.cpp (knob cases), exact.repair.a_literal_is_changed_where_it_stands_alone
- test_repair_search_with_any_compiler -> COVERED: verify.authored.repair_test.cpp, exact.repair.a_change_that_makes_the_function_the_programs_is_kept_with_write
- test_period_compiler_gives_the_original_back -> COVERED: exact.emu.xenix.* (the XENIX compile of the linker source), build.period16.build_compiles_each_module_and_links
- test_a_verdict_holds_until_what_it_was_for_changes -> COVERED: engine.function_store.a_changed_sig_drops_the_verdicts_under_other_profiles, engine.lanes.a_module_document_round_trips_and_a_hand_made_text_changes_the_sig_as_the_python_does
- test_period_source_gives_the_declarations_an_author_had -> COVERED: build.msc6.sdk_prototypes_and_calls, build.msc6.stand_in_from_header (period declarations), build.module.stand_in_from_header_declares_what_the_header_declares
- test_breaks_change_what_a_function_does -> COVERED: verify.teeth.operator, comparison, constant, plant_loop, whole_function
- test_other_shapes_of_the_same_function -> COVERED: verify.shape.switches, returns, folds, statements; verify/authored/search16_test.cpp
- test_object_file_of_the_16_bit_compiler_is_read -> COVERED: build.msc6.function_code_cuts_a_public_out_of_the_segment, formats.omf.* (object_code with fix-up mask), build.msc6.pascal_names_and_order
- test_the_16_bit_compiler_s_function_is_cut_out_by_its_name -> COVERED: build.msc6.function_code_cuts_a_public_out_of_the_segment, build.masm.functions_lists_the_publics_of_the_object_with_their_bytes
- test_pascal_calls_are_said_first_pushed_first -> COVERED: build.msc6.pascal_names_and_order (GetClientRect/Rectangle cases in build/msc6_test.cpp)
- test_a_constant_near_address_is_a_variable_and_a_batch_s_errors_name_their_functions -> COVERED: build/msc6_test.cpp (0x63b2 case)
- test_an_api_called_by_number_is_said_through_the_sdk_where_its_arguments_fit -> COVERED: build.msc6.sdk_prototypes_and_calls (USER_107 DefWindowProc), source.readable.calls_are_read_with_their_arguments
- test_a_temporary_used_once_goes_back_into_its_expression -> COVERED: build.msc6.fold_temporaries_puts_the_expression_back (LocalAlloc/lstrlen case in source/readable_test.cpp too)
- test_a_pair_of_parameters_used_only_together_is_one_far_pointer_and_a_c_named_import_is_cdecl -> COVERED: build/msc6_test.cpp (FP_OFF/FP_SEG, far-pair cases), decompile.frame16.a_decoder_of_the_callers
- test_the_last_mile_search_rewrites_a_comparison_the_way_a_source_had_it -> COVERED: verify/authored/search16_test.cpp (0x19 < / >= 0x1a case)
- test_the_frame_structure_is_taken_apart_into_the_locals_a_source_declares -> COVERED: build.msc6.split_frame_takes_the_frame_structure_apart
- test_a_c_function_s_arguments_are_what_its_caller_takes_off -> COVERED: verify.gate16.contract16.what_the_caller_takes_off, the_caller_takes_off_other_bytes
- test_a_call_s_arguments_are_what_its_callee_removes_of_what_was_pushed -> COVERED: verify.gate16.contract16.a_calls_arguments_are_what_its_callee_removes_of_what_was_pushed
- test_source_files_are_told_by_push_cs_calls_and_an_if_else_can_be_turned_around -> COVERED: verify.match16.source_files, build/module_test.cpp (push cs), verify/authored/search16_test.cpp
- test_a_cextdef_is_numbered_with_the_other_externals -> PORTED: formats.omf.a_cextdef_is_numbered_with_the_other_externals
- test_wine_s_spec_types_an_import_and_its_calls_are_said_one_argument_to_a_parameter -> COVERED: build.msc6.wine_typing_and_definition, symbols.wine16.read_spec.arguments_said_or_not
- test_a_typed_call_inside_the_arguments_of_another_is_said_with_it -> COVERED: build.msc6.wine_typing_and_definition
- test_a_number_with_a_leading_zero_is_decimal_in_the_texts_of_the_16_bit_lane -> COVERED: decompile.asm_blocks.place_cases (leading-zero case, header note of decompile/asm_blocks_test.cpp), C++ reads numbers with from_chars
- test_a_hole_of_no_length_is_no_hole -> COVERED: build.asm16.a_function_rebuilt_from_c_inside_the_run_is_a_hole, build.module.pack_holes_*
- test_a_far_call_to_an_import_with_an_addend_is_named_with_it -> COVERED: build.asm16.a_far_call_to_an_import_with_an_addend_is_named_with_it
- test_a_run_time_library_routine_is_named_outside_the_linker_s_bytes -> COVERED: symbols.names.a_runtime_library_function_is_named_for_its_library, symbols.runtime_library.compiler_from_marks_and_libraries
- test_a_function_reading_ax_dx_or_bx_before_writing_it_takes_them_as_fastcall_arguments -> COVERED: analyze/conventions_test.cpp, analyze/origin_test.cpp (fastcall cases), decompile/describe_test.cpp
- test_a_callee_that_gives_back_the_caller_s_ax_returns_no_dx_either -> COVERED: decompile/describe_test.cpp (CALLER_RESULTS cases)
- test_an_exported_function_s_parameters_are_typed_by_wine_s_spec -> COVERED: symbols.wine16.* and build.msc6.wine_typing_and_definition (IpFindFirstLine)
- test_only_visual_c_1_52c_runs_without_the_emulator -> COVERED: build.msvc152.compile_asks_the_emulator_for_the_right_run
- test_a_backpatch_record_is_added_into_the_code -> PORTED: formats.omf.a_backpatch_record_is_added_into_the_code
- test_a_verdict_is_for_the_compiled_code -> COVERED: engine.function_store.a_changed_sig_drops_the_verdicts_under_other_profiles, engine.lanes.a_lane_that_does_not_build_per_function_files_every_function_under_the_modules_signature
- test_cache_files_results_under_what_made_them -> COVERED: verify.compare.keys_name_the_revision_and_every_ingredient, base.cache.* (key cases)
- test_every_kind_keeps_fingerprints_and_writes_calls_alike -> COVERED: verify.lane_common.fingerprint_of_a_returned_run, calls_skip_cap_and_show, verify.compare.fingerprints_round_trip_through_the_cache
- test_the_judge_s_build_has_the_original_s_code_segment_where_c_still_reads_it -> COVERED: verify.win16.code_segments_the_c_still_reads_are_given_to_the_build
- test_the_uart_and_the_interrupt_controller_answer_as_the_chips_do -> COVERED: emulate.ports16.every_com_port_and_base_zero_has_a_uart, uart_line_status_says_transmitter_empty (and the interrupt controller cases in emulate/ports16_test.cpp)
- test_the_governor_shares_one_budget -> COVERED: emulate.native_win16.the_governor_ceiling_stops_a_run_that_maps_too_much, emulate.native_trace.needs_a_cache_and_a_governor, base.parallel.memory_allows_counts_pieces_past_the_reserve
- test_an_instruction_is_decoded_once -> COVERED: emulate.m68k.code_is_decoded_once_per_address, emulate.sm83.code_is_decoded_again_when_the_bytes_under_it_change
- test_chunks_come_back_in_address_order -> COVERED: exact.modules / exact.project (address-ordered functions), recompile.switches.every_bctr_of_the_code_is_scanned_in_address_order
- test_programs_stored_one_after_another_are_each_found -> COVERED: formats.dos.* and build.exepack/pklite (several programs in one file); decompile.jar.* none; see exact.exepack.reads_the_program_it_packed
- test_a_screen_shorter_than_the_pages_points_is_not_that_page -> COVERED: verify.gate16.setup16 (header note "a screen too short to hold one of the three pixels"), verify.gate16.setup16.pass_when_the_license_page_comes_up
- test_runs_of_an_image_lane_that_differ_in_how_many_places_they_show_are_a_difference -> COVERED: verify.lanes.image_lane.effects_of_different_length
- test_a_dos_program_whose_source_text_is_missing_still_has_its_function_keys -> COVERED: verify.lanes.dos.prepare_without_source

## tests/test_versions.py (9)
- test_default_product_is_untouched -> COVERED: base.corpus.root_of_the_default_product, base.corpus.products_and_selection
- test_selecting_a_product -> COVERED: base.corpus.selected_is_for_the_length_of_a_scope, base.corpus.products_and_selection
- test_boot_image_modules -> COVERED: base.corpus.boot_images_are_unpacked_once_into_the_state_folder
- test_fonts -> COVERED: formats.fonts.a_fon_lists_its_fonts, a_bare_fnt_is_one_font_and_other_files_none
- test_a_module_against_itself -> COVERED: compare.versions.a_build_is_identical_to_itself
- test_dos_program_and_shared -> COVERED: compare.versions.functions_shared_by_programs
- test_function_spans_are_the_carved_functions -> COVERED: compare.versions.load_reads_each_kind, store.versions_ledger.bodies_and_their_names_go_through_the_body_store
- test_two_products_32bit -> COVERED: compare.versions.windows_95_against_windows_98_is_what_the_original_wrote, compare.versions.a_whole_product
- test_two_products_16bit -> COVERED: compare.versions.windows_95_against_windows_98_is_what_the_original_wrote, compare.versions.the_command

## tests/test_vmx128.py (6)
- test_rows_decode_to_themselves -> COVERED: decompile.vmx128.table, decompile.vmx128.words
- test_sleigh_is_current -> COVERED: decompile.vmx128.sleigh_is_the_checked_in_language
- test_language_matches_table -> COVERED: decompile.vmx128.sleigh_without_siblings_defines_every_operation, decompile.vmx128.coverage
- test_x31_forms -> COVERED: decompile.vmx128.decode.opcode_31_unaligned_loads_and_stores
- test_game_vmx128_words -> COVERED: decompile.vmx128.decode.words, decompile.vmx128.fields
- test_other_words_unchanged -> COVERED: decompile.vmx128.decode.not_vmx128

## tests/test_whole_module.py (17; main() runs 15)
- test_resources_of_a_32_bit_module -> COVERED: build.resources.compare_a_32_bit_module_with_changed_copies_of_itself, formats.resources.*
- test_resources_of_a_16_bit_module -> COVERED: build.resources.compare_a_16_bit_module_with_changed_copies_of_itself
- test_type_library -> COVERED: formats.resources.a_type_library_describes_itself, a_cut_type_library_gives_what_was_read
- test_padding_is_not_a_difference -> COVERED: build.resources.names_are_compared_without_regard_to_case, build.resources.payloads_with_one_name_and_different_bytes_are_numbered (compare cases)
- test_string_literals -> COVERED: build.resources.quoted_writes_the_escapes_the_compiler_reads_back, quoted_reaches_latin1_by_octal_and_beyond_it_by_utf8
- test_resource_round_trip_32_bit -> COVERED: build.resources.a_32_bit_module_survives_the_round_trip_through_wrc
- test_resource_round_trip_16_bit -> COVERED: build.resources.a_16_bit_module_survives_the_round_trip_through_wrc
- test_definitions_32_bit -> COVERED: build.definitions.win32_definition_file, export_directives
- test_definitions_16_bit -> COVERED: build.definitions.win16_definition_file
- test_data_objects -> COVERED: build.data.object_placement, object_text, data16_lzexpand, data32_lz32
- test_build_32_bit_module -> COVERED: build.module.document32_places_each_function_by_its_address, build.module.check_compares_format_exports_and_imports
- test_build_16_bit_module -> COVERED: build.module.document16_holds_assembled_functions_as_fragments_that_share_their_file
- test_fixed_code_segment_keeps_its_size -> COVERED: build.module.pack_holes_aligns_to_a_word_and_says_who_found_no_room, pack_holes_puts_each_function_in_its_own_hole_when_it_fits
- test_16bit_dll_with_the_period_tools -> COVERED: build.period16.build_compiles_each_module_and_links, build.period16.segments_and_compare
- test_flat_thunk_dll -> COVERED: build.flat_thunk.* (classify, recover, discover_the_pieces_of_a_source_folder, bindiff)

## tests/test_x360.py (9)
- test_helpers_and_jump_buffers -> COVERED: recompile.config.helpers_by_name_and_by_pattern, recompile.config.jump_buffers
- test_setjmp_through_a_register_other_than_r3 -> COVERED: recompile.config.jump_buffers (r4 case), recompile.config.a_longjmp_without_its_setjmp_is_not_given
- test_longjmp_without_setjmp_emits_neither -> COVERED: recompile.config.a_longjmp_without_its_setjmp_is_not_given
- test_switch_tables -> COVERED: recompile.switches.* (halfword table, labels stay in their function), recompile.jumptable.*
- test_extra_functions_respect_switches -> COVERED: recompile.config.extra_functions, extra_functions_meaning
- test_tail_jump_targets_are_functions -> COVERED: recompile.config.extra_functions_meaning (only-tail-jumped-to case), recompile.powerpc.a_branch_is_a_goto_inside_the_function_and_a_tail_call_outside
- test_basic_xex -> COVERED: formats.xex.write_basic_puts_the_image_in_one_block, recompile.xenon_tools.xex_basic_writes_what_the_python_writes
- test_runtime_read_image -> COVERED: native/x360/runtime/test/xex_image_test.cpp (the runtime's own test; native/x360 is not touched here)
- test_instruction_tests -> COVERED: recompile.xenon.instruction_tests_assemble_recompile_build_and_run

## tests/test_xbe.py (script: build an XBE, lift it as a PE image, decompile)
- the XBE -> PE image with kernel calls named from the export map -> PORTED: formats.xbox.xbe.the_pe_image_of_the_test_xbe (the XBE the test builds, lifted as a PE: x86, base, entry, xboxkrnl.exe import #8 at the thunk, sections at their addresses), the_xbe_the_test_builds_is_the_fixture, the_fixtures_read_as_the_pythons_do (PE image SHA-1 equal to the Python's), to_pe_writes_the_image_once_named_by_its_contents. The ordinal's NAME (DbgPrint) and the decompile are other areas' (symbols ordinals, decompile); platforms.package covers identify.

## tests/test_xbox_crypto.py (5)
- test_hashes -> COVERED: formats.xbox.crypto.xecrypt.hashes_and_macs
- test_ciphers -> COVERED: formats.xbox.crypto.xecrypt.aes_and_des, rc4
- test_cpu_key_and_keyvault -> COVERED: formats.xbox.crypto.xecrypt.smc_ecc_and_cpu_keys, keyvaults_as_the_python_sealed_them, keyvault_certificate_verification
- test_stream_io -> COVERED: formats.xbox.crypto.stream_io.* (5)
- test_command -> COVERED: formats.xbox.crypto.harness.the_command_prints_the_report_and_exits_1_when_any_check_fails, harness.each_check_computes_what_the_pythons_xecrypt_does

## tests/test_xbox_effect.py (9)
- test_effect_layout -> COVERED: formats.xbox.effect.sprite_effect_little_endian, xna_content_is_big_endian_and_may_carry_its_header
- test_shader_tables -> COVERED: formats.xbox.shader.vertex_shader_of_the_sprite_effect, pixel_shader_of_the_sprite_effect, loops_are_read_from_the_literal_records
- test_recovered_hlsl -> COVERED: decompile.xbox_effect.sprite_effect_comes_back_as_its_fx_source, decompile.xenos_hlsl.*
- test_stock_effect_arrays -> COVERED: formats.xbox.effect.arrays_hold_their_elements, decompile.xbox_effect.stock_effect_becomes_one_technique_per_shader_index
- test_real_effects_recover -> COVERED: decompile.xenos_hlsl.sdk_shaders_translate_as_the_original_did, decompile.xbox_effect.old_harness_cases
- test_mgfx_parameters -> COVERED: xna.mgfx.parse_parameters_reads_the_fx_json, real_effects_match_the_checked_result
- test_fixtures_current -> NOT NEEDED: checks that checked-in Python fixtures are current against the Python converter; the native goldens are checked by the tests above
- test_behaves_as_source -> COVERED: decompile.xbox_effect.old_harness_cases, xna.mgfx.replays_the_reference_cases
- test_unsupported_names_itself -> COVERED: decompile.xbox_effect.untranslated_shaders_name_the_shader_and_instruction, decompile.xenos_hlsl.what_it_refuses

## tests/test_xbox_formats.py (24)
- test_stfs_pack_round_trip -> COVERED: formats.xbox.stfs.what_is_packed_reads_back, packing_is_byte_exact_with_the_old_packer
- test_stfs_resign_signs_and_verifies -> COVERED: formats.xbox.stfs.resigning_is_byte_exact_with_the_old_signer, resign_file
- test_god_round_trip -> COVERED: formats.xbox.god.write_and_read_back, two_parts
- test_nxe_install_to_god -> COVERED: formats.xbox.god.install_to_god
- test_fatx_read_and_recover -> COVERED: formats.xbox.fatx.an_original_xbox_image, recover_finds_dirents_with_no_directory, carve_by_signature
- test_fatx_write -> COVERED: formats.xbox.fatx.add_a_file_and_read_it_back, a_directory_that_fills_its_cluster_grows, remove_frees_the_chain_and_keeps_the_dirent_recoverable
- test_gpd_unlock -> PORTED: formats.xbox.xdbf.unlock_and_rebuild_as_test_gpd_unlock (the test's own GPD), the_fixture_reads_as_the_pythons_do (every line of the Python's dump, unlock / lock / unlock-all rebuild hashes included), strings_are_split_on_unit_boundaries
- test_texture_untile -> COVERED: formats.xbox.texture.tile_and_untile_are_inverses, dds_header_fields
- test_xpr -> PORTED: formats.xbox.xpr.the_selfcheck (the _selfcheck's 64x32 DXT1 XPR0 round trip + DDS wrap + the XPR2 header), the_fixtures_read_as_the_pythons_do, an_xpr2_needs_its_dimensions
- test_xwb -> COVERED: xna.xact.test_banks_in_both_byte_orders, xna.xact.the_entries_of_real_wave_banks_match_the_python, xna.xact.pcm_is_wrapped_as_wav
- test_xur -> PORTED: formats.xbox.xur.test_xur (strings, byte-identical re-emit, set_string), the_fixture_reads_as_the_pythons_do, later_sections_move_with_the_edit; section offsets are from the file start (Python bug fixed in both, see _bugs/LIST.md, 2026-11-13 xbox assets)
- test_xbe -> PORTED: formats.xbox.xbe.header_certificate_and_keys (Halo, entry 0x11030 with the retail key; the debug / Chihiro / no-key cases besides), the_fixtures_read_as_the_pythons_do; `info` of it: formats.xbox.summary.each_fixture_is_summarised_as_the_python_does
- test_xma_and_bink -> COVERED for XMA: xna.xact.xma2_in_a_riff_container_matches_the_python; PORTED for Bink: formats.xbox.bink.test_xma_and_bink_header (the BIKi header of the test), the_fixture_reads_as_the_pythons_does, generations_and_frame_rates
- test_xzp_round_trip -> COVERED: formats.xbox.xzp.a_v1_pack_reads_as_the_pythons_does, a_v3_pack_reads_as_the_pythons_does
- test_identify -> COVERED: formats.detect.identify_by_magic (XEX2/CISO/XPR2/CHD/BIKi/Mach-O/Java/NES cases), platforms.consoles.registry_identifies_each_by_its_probe
- test_folder_map -> COVERED: formats.inspect.a_folder_is_mapped_by_format, formats.inspect.markdown_reports_a_description_and_a_folder
- test_real_stfs_theme -> COVERED: formats.xbox.stfs.package_fields, extract_all
- test_real_xzp -> COVERED: formats.xbox.xzp.a_v3_pack_reads_as_the_pythons_does
- test_real_pck -> COVERED: formats.xbox.pck.the_table_reads_as_the_pythons_does, payloads_are_found_past_their_padding
- test_texture_offsets_and_gfx_bits -> COVERED: formats.xbox.texture.tiled_offsets_as_the_old_reader_computed_them, formats.gfx.glyph_outlines_are_moves_lines_and_curves
- test_real_heavyiron_and_gfx -> COVERED: formats.xbox.heavyiron.the_archives_parse_as_the_old_reader_parsed_them, textures_decode_as_the_old_reader_found_them, formats.gfx.truetype_fonts_have_the_tables_fonttools_wrote
- test_nand_flash -> COVERED: formats.xbox.nand.the_flash_reads_as_the_pythons_does, formats.xbox.nand_tool.*

## tests/test_xenos.py (4 checks in one script)
- control flow alloc/exece, scalar multiply into r0.z, the mov to oC0, nothing after the program's end -> COVERED: decompile.xenos.sample_shader, decompile.xenos.program_layout

## tests/test_xex.py (6 checks in one script)
- is_xex -> COVERED: formats.xex.probe, formats.xex.not_an_xex_is_a_format_error
- import_libraries -> COVERED: formats.xex.import_libraries_of_an_image_without_a_sidecar, formats_xex_imports (test in formats/xex_test.cpp: imports == xam.xex, xboxkrnl.exe)
- corpus.retail leaves other files -> COVERED: base.corpus.retail_resolve_and_paths (a path passes through)
- engine selftest -> COVERED: formats.xbox.crypto.harness.each_check_computes_what_the_pythons_xecrypt_does, formats.xex.run_passes_the_arguments
- engine usage -> NOT NEEDED: the usage text of the separate xex engine program (a Python subprocess check)

## tests/test_xex_edit.py (5)
- test_assembler_against_capstone -> COVERED: build.ppc_asm.encodings, build.xex_edit.the_assembler_alone_prints_bytes_and_its_errors_are_reported
- test_branches -> COVERED: build.ppc_asm.branches, build.ppc_asm.destination_first_forms
- test_a_branch_that_does_not_reach_is_refused_not_wrapped -> COVERED: build.ppc_asm.errors
- test_privileges -> COVERED: build.xex_edit.privileges_are_read_set_and_cleared_where_they_stand, the_privilege_images_are_the_pythons
- test_patch_and_hook_real_module -> COVERED: build.xex_edit.code_edits_write_the_same_images_as_the_python

## tests/test_xna.py (16)
- test_texture_both_byte_orders -> COVERED: xna.xnb.textures_in_both_byte_orders
- test_dxt1_punch_through -> COVERED: xna.xnb.dxt1_in_both_word_orders, formats.xbox.texture.a_dxt1_block_by_hand
- test_real_game_converts -> COVERED: xna.run.convert_a_game_folder, xna.xnb.real_compressed_content_matches_the_python
- test_fixups -> COVERED: xna.fixups.apply_over_a_folder, constructors, data_holders, ref_casts
- test_managed_and_decompile -> COVERED: xna.run.decompile_runs_ilspy_and_reports_its_failure, xna.run.assemblies_are_the_games_own_the_exe_first
- test_port_rewrites -> COVERED: xna.port.rewrite_game, rewrite_leaves_what_it_does_not_know
- test_port_error_fixers -> COVERED: xna.port.fix_access, fix_ref_out, fix_named_arguments_and_moved_members, fix_shadowed_extensions, build_fixes_and_builds_again
- test_rename_readers -> COVERED: xna.xnb.reading_and_renaming_readers
- test_port_reader_names -> COVERED: xna.port.reader_names, reader_names_of_a_folder_are_stamped
- test_obfuscated_forms -> COVERED: xna.fixups.names_in_other_scripts, xna.mgfx.names_in_other_scripts_are_kept_as_bytes
- test_de4dot_detects -> COVERED: xna.run.deobfuscation
- test_real_effects_convert -> COVERED: xna.xnb.xbox_effects_become_hlsl, xna.mgfx.real_effects_match_the_checked_result
- test_port_effect_and_input -> COVERED: xna.port.fix_color_and_vertex_binding, xna.port.compile_effects_uses_what_was_compiled_before
- test_xact_sound_bank -> COVERED: xna.xact.a_real_sound_bank_matches_the_python, damaged_banks_are_format_errors
- test_sound_effect -> COVERED: xna.xnb.pcm_sounds_become_wav_files, xna.xnb.an_xma_sound_needs_ffmpeg

## tests/test_z80.py (11)
- test_z80_table_round_trip -> COVERED: emulate.z80.every_instruction_of_the_table_runs, decompile.z80.* (table), build.z80_asm.*
- test_sm83_table_round_trip -> COVERED: emulate.sm83.every_opcode_steps_without_crashing, decompile.sm83.*
- test_z80_flags -> COVERED: emulate.z80.add_sub_flags, logic_flags, daa, compare_takes_bits_5_and_3_from_the_operand
- test_sm83_flags -> COVERED: emulate.sm83.add_adc_flags, sub_sbc_cp_flags, logic_flags, daa_cpl_scf_ccf
- test_platforms_recognise -> COVERED: platforms.consoles.registry_identifies_each_by_its_probe, formats.sms/gb/msx/zx tests
- test_sms_source_rebuilds -> COVERED: source.sms_writer.* , build.z80_asm.*
- test_gb_source_rebuilds -> COVERED: source.gb_writer.* , build.sm83_asm.*
- test_sms_lane_judges -> COVERED: verify.lanes.flat_machines_test.cpp (SMS), engine.lane_kit.machines.*
- test_gb_lane_judges -> COVERED: verify.lanes.flat_machines_test.cpp (Game Boy), engine.lane_kit.machines.*
- test_spectrum_lane -> COVERED: formats.zx.*, engine.lane_kit.machines.* (Spectrum)
- test_msx_lane -> COVERED: formats.msx.*, engine.lane_kit.machines.msx_edited_program

---------------------------------------------------------------------------------------------------------------------------------
## tests/translate/ (24 files)

### test_c_backend.py (43) -- the C back end of the hub (native/l2: cbl2.dll), through native/cb/translate/native_l2
Each of these is PORTED as a replay of the recorded library calls: `translate.native_l2.cases.c_backend.<name>` (the name is the Python
test's without `test_`). native_l2_cases_test.cpp, data testdata/c_backend_cases.json. The three that were already COVERED by hand-written
tests are also listed in their native test.
- test_numbers_in_their_natural_base -> PORTED (12 calls) + COVERED: translate.native_l2.numbers_in_their_natural_base
- test_brackets_as_printc_places_them -> PORTED (6) + COVERED: translate.native_l2.brackets_and_conditions
- test_joined_conditions -> PORTED (4) + COVERED: translate.native_l2.brackets_and_conditions
- test_statements -> PORTED (1) + COVERED: translate.native_l2.function_text
- test_moved_passes_far_and_near_calls -> PORTED (4) + COVERED: translate.native_l2.passes_moved_onto_the_tree
- test_moved_far_from_stack -> PORTED (2)
- test_moved_frame_through_ss -> PORTED (3)
- test_moved_pieces -> PORTED (2)
- test_moved_registers_null_and_globals -> PORTED (1)
- test_a_module_as_one_tree_with_what_stayed_assembly -> PORTED (1)
- test_a_32_bit_module_as_one_tree -> PORTED (1)
- test_comments_where_printc_writes_them -> PORTED (1)
- test_a_long_line_broken_as_printc_breaks_it -> PORTED (2)
- test_moved_passes_that_read_the_module -> PORTED (3)
- test_moved_own_segment_calls -> PORTED (2)
- test_moved_interrupts -> PORTED (1)
- test_moved_selector_structs -> PORTED (6)
- test_moved_old_style_definition -> PORTED (3)
- test_moved_entry_sp_and_stack_arguments -> PORTED (1)
- test_decompile16_counts_calls_in_l2 -> PORTED (2)
- test_moved_far_addresses -> PORTED (1)
- test_moved_far_casts -> PORTED (1)
- test_moved_far_locals -> PORTED (1)
- test_moved_far_concat_casts -> PORTED (2)
- test_moved_buffer_fields -> PORTED (3)
- test_moved_far_reads -> PORTED (1)
- test_moved_near_to_far -> PORTED (1)
- test_moved_stack_segment_stores -> PORTED (1)
- test_moved_callee_pops -> PORTED (2)
- test_moved_caller_frame_slots -> PORTED (6)
- test_moved_selector_imports -> PORTED (1)
- test_moved_segment_values -> PORTED (2)
- test_moved_code_segment_reads -> PORTED (2)
- test_names_used_for_the_header -> PORTED (1)
- test_prototypes_where_the_calls_agree -> PORTED (8)
- test_moved_32bit_heading -> PORTED (9)
- test_moved_keep_frame -> PORTED (5)
- test_moved_32bit_globals -> PORTED (3)
- test_moved_fit_calls -> PORTED (4)
- test_moved_powerpc_passes -> PORTED (3)
- test_moved_struct_fields -> PORTED (5)
- test_inline_asm_block_and_a_label_ending_a_block -> PORTED (1) + COVERED: decompile.inline_asm.*, decompile.asm_blocks.*
- test_heading_declaration_from_l2 -> PORTED (1)

### test_l2_survives.py (4) -> PORTED, replayed: translate.native_l2.cases.l2_survives.*
- test_switch_on_a_pointer_keeps_its_case_labels_cast, test_switch_on_a_far_pointer_is_written_on_its_four_bytes, test_an_array_index_past_9_is_a_literal, test_a_supplied_jump_table_flow_never_reaches_is_dropped -> PORTED

### test_frame16.py (12)
- test_a_flag_kept_in_a_word_is_a_word -> COVERED: decompile.frame16.a_flag_kept_in_a_word_is_a_word
- test_inc_is_written_as_plus_plus -> COVERED: decompile.shape16.inc_is_written_as_plus_plus; PORTED replay of the library call (translate.native_l2.cases.frame16.inc_is_written_as_plus_plus)
- test_an_inc_of_a_register_stays_an_assignment -> COVERED: decompile.shape16.an_inc_of_a_register_stays_an_assignment
- test_an_inc_of_a_place_is_not_a_larger_step -> COVERED: decompile.shape16.an_inc_of_a_place_is_not_a_larger_step
- test_a_function_without_a_frame_is_left -> COVERED: decompile.frame16.nothing_to_lay_out
- test_the_pieces_of_a_buffer_are_one_array -> COVERED: decompile.frame16.the_pieces_of_a_buffer_are_one_array; PORTED replay (cases.frame16)
- test_the_back_end_writes_postfix_increments -> PORTED (2 library calls, cases.frame16) + COVERED: decompile.shape16.inc_is_written_as_plus_plus
- test_an_if_return_is_turned_round_where_the_original_has_the_other_first -> COVERED: decompile.shape16.if_return_turned_round
- test_a_piece_of_a_buffer_piece_is_read_at_its_place -> PORTED replay (cases.frame16) + COVERED: decompile.frame16.cases
- test_a_buffer_holding_a_far_pointer_is_left -> COVERED: decompile.frame16.a_buffer_with_a_far_pointer_is_left
- test_a_buffer_with_an_array_among_its_pieces_is_left -> PORTED replay (cases.frame16) + COVERED: decompile.frame16.cases
- test_an_unknown_local_in_a_place_of_its_own_keeps_its_type -> COVERED: decompile.frame16.cases

### test_asm_blocks.py (2)
- test_a_decimal_with_a_leading_zero_in_an_operand_is_a_place -> COVERED: decompile.asm_blocks.place_cases
- test_a_window_that_leaves_no_instruction_places_no_block -> COVERED: decompile.asm_blocks.place_degenerate_window

### test_authored_rewrites.py (2)
- test_a_comparison_with_an_octal_constant_is_left_alone -> COVERED: verify/authored/* tests (search16_test.cpp, repair_test.cpp octal cases)
- test_a_switch_chain_with_an_octal_constant_is_left_alone -> COVERED: verify.shape.switches, verify/authored/search16_test.cpp

### test_clang_frontend.py (7)
- test_declarators, test_layouts_follow_the_target -> COVERED: translate.clang.ctypes.parse_and_spell, translate.clang.ctypes.layouts_and_decay, translate.clang.ctypes.tokens
- test_c_declarations, test_c_bodies, test_c_unit_is_l2 -> COVERED: translate.clang.lower.programs_give_the_reviewed_documents, string_literals, the_unit_says_what_it_is
- test_cpp_classes, test_cpp_bodies -> COVERED: translate.clang.lower.programs_give_the_reviewed_documents, character_text_and_widths

### test_console_patches.py (3)
- test_sound_routes_the_name_and_drops_the_pool_query -> COVERED: java.minecraft.sound_fix.* (patch_music_runs_play_always_and_guards_the_deref, stranded_finds_a_guard_that_drifted)
- test_applet_ping_becomes_the_assignment -> COVERED: java.minecraft.applet_ping.patch_replaces_the_try_with_the_assignment, rewrite_matches_golden
- test_render_budget_and_hurt_guard -> COVERED: java.minecraft.render_fix.patch_budget_declines_without_the_early_out, java.minecraft.render_fix.hurt_guard_requires_hurt_time

### test_cpp03_bodies.py (3)
- test_bodies_come_from_the_tree -> COVERED: translate.cpp03.* (the corpus programs, body-by-body goldens in expected.txt)
- test_tree_bodies_beside_the_jvm -> COVERED: translate.cpp03.feat, translate.judge_native_jvm.*
- test_null_of_value_types -> COVERED: translate.cpp03.feat (null of value types, try/catch NPE case in 008_Ops.java), translate.cpp03.text.types

### test_cpp17_backend.py (7)
- test_records_get_their_implicit_members, test_instanceof_patterns_carry_their_binding, test_the_object_model -> COVERED: translate.cpp17.own.zoo, translate.cpp17.own.mix, translate.cpp17.corpus.t17
- test_the_program_reads_as_cpp17 -> COVERED: translate.cpp17.corpus.*, translate.cpp17.write_unit
- test_the_program_runs_as_the_jvm_runs_it -> COVERED: translate.judge_native_jvm.the_judge_compares_the_two_runs, translate.javajudge.judge_program
- test_desugar_lowers_what_cpp03_cannot_say -> COVERED: translate.desugar.* (instanceof_patterns, switch_expression_*, foreach_*)
- test_inside_the_transpiler_beside_cpp03 -> COVERED: translate.cpp17.paths, translate.cpp03.* (cpp03_cpp17 bridge)

### test_csharp_kit.py (13)
- test_every_corpus_program_lowers_to_valid_l2 -> COVERED: translate.csharp.lower.corpus_programs_give_the_reviewed_documents, translate.l2.* schema
- test_the_bound_tree_says_what_the_compiler_resolved -> COVERED: translate.csharp.lower.nodes_carry_their_positions_and_the_unit_its_language, a_symbol_kind_the_front_end_has_no_use_for_is_a_global
- test_what_cannot_be_carried_is_refused_with_its_reason -> COVERED: translate.csharp.lower.cases_give_the_reviewed_documents_and_refusals, csharp.decline.*
- test_csharp_round_trip_is_equivalent -> COVERED: translate.csharp.corpus.* (writes each corpus program), translate.judge.csharp_real
- test_the_back_end_writes_csharp_idioms -> COVERED: translate.csharp.expr.*, stmt.*, unit.*, decl.*, literals.*
- test_units_of_one_file_stem_are_separate_files_and_big_constants_fold_exactly -> COVERED: translate.csharp.program.file_names_and_writing, translate.csharp.expr.constants_that_overflow_are_unchecked
- test_java_to_csharp_is_equivalent, test_minecraft_alpha_noise_java_to_csharp_is_equivalent, test_java_semantics_are_kept_in_csharp -> COVERED: translate.csharp_java.corpus.calc, corpus.fixtures, corpus.modern, semantics_in_the_text, features
- test_csharp_to_cpp03_is_equivalent, test_csharp_to_cpp03_refuses_on_a_runtime_gap, test_csharp_to_cpp03_refuses_what_it_cannot_carry -> COVERED: translate.cs2cpp.convert, translate.cs2cpp.back, translate.cs2cpp.tool, translate.cs2cpp.javart
- test_the_judge_compares_output_and_exit_code -> COVERED: translate.judge.compare_equivalent, compare_divergent, compare_other_verdicts

### test_csharp_lower_trees.py (2)
- both -> COVERED: translate.csharp.lower.cases_give_the_reviewed_documents_and_refusals, corpus_programs_give_the_reviewed_documents

### test_desugar.py (3)
- test_a_foreach_over_an_array_becomes_a_counting_loop -> COVERED: translate.desugar.foreach_over_an_array
- test_a_labelled_loop_lowers_the_loop_inside_it_too -> COVERED: translate.desugar.foreach_label_and_nesting
- test_the_modern_java_program_is_lowered_as_the_cpp_module_lowers_it -> COVERED: translate.desugar.a_real_program

### test_dotnet_toolchain.py (2)
- both -> COVERED: translate.dotnet.discovery_on_an_installation, newest_version_dir, version_key, discovery_gaps

### test_java_as_c.py (2)
- test_classes_are_structs_and_tables -> COVERED: translate.c_java.corpus.*, translate.c_java.own.zoo, c_java.type_codes
- test_the_c_program_runs_as_the_jvm_runs_it -> COVERED: translate.c_java.runs.mix, runs.names, runs.zoo

### test_java_backend.py (6)
- test_faithful_tier -> COVERED: translate.java.golden.faithful_core, faithful_arith, faithful_overloads, faithful_va
- test_idiomatic_tier -> COVERED: translate.java.tier_choice, translate.java_objects.golden.*
- test_cpp_classes -> COVERED: translate.java_objects.golden.classes, golden.destructors, golden.structs
- test_goto_and_varargs -> COVERED: translate.java.golden.gap_goto, faithful_va
- test_refusals_say_why -> COVERED: translate.java.errors, translate.java_objects.refusals, translate.java_libc.unsupported_and_missing_rows_refuse
- test_java_literals_and_names -> COVERED: translate.java.int_literals, float_literals, string_literals, char_literal_text, jname

### test_java_frontend.py (3)
- all three -> COVERED: java.parse.* / java.pipeline (the Java front end is native/cb/java) and translate.l2.java_front_end_units; the dumper cache is Python-plumbing (NOT NEEDED: test_the_dumper_is_cached)

### test_java_lower.py (6)
- test_nothing_unsupported, test_types, test_names_are_resolved_by_symbol, test_constructors, test_modern_java_lowers, test_enums_and_anonymous_classes -> COVERED: translate.jmodel.*, translate.ir.*, translate.l2.java_front_end_units, translate.java.*

### test_l2.py (3)
- test_the_schema_is_valid_and_names_every_node -> COVERED: translate.native_l2.schema_check, translate.l2.errors
- test_lowered_java_is_valid_l2_and_round_trips -> COVERED: translate.l2.sym_roundtrip, types_roundtrip, java_front_end_units
- test_binary_facing_types_are_expressible -> COVERED: translate.l2.types_roundtrip, translate.ir.types.*, translate.l2.slots_that_hold_lists_or_nodes

### test_module_model.py (9)
- test_an_assembly_is_its_lines_as_fragments -> COVERED: translate.l2.assembly, translate.asm.first_line_of_each_unit
- test_the_assembly_back_end_writes_comments_and_declines_c -> COVERED: translate.asm.comment, translate.asm.declines
- test_emit_makes_each_function_a_fragment -> COVERED: source/*_writer tests (the native writers build the assembly tree: translate.l2.assembly, source.pe_writer.*); the Python test itself fails today (see Findings)
- test_the_readable_pass_reads_the_module_tree -> COVERED: source.readable.* , translate.l2.module_model
- test_the_port_reads_its_module_tree -> COVERED: port.* (module tree read), translate.l2.module_model
- test_the_module_file_is_written_from_its_tree_and_the_facts -> COVERED: translate.module_file.assembly, module_file.assembly_units_and_declines, module_file.c_whole_build_in_parts
- test_a_drivers_file_is_its_status_then_its_source -> COVERED: translate.module_file.parts_declines_other_documents, source.le_writer.*
- test_a_made_module_is_its_tree -> COVERED: translate.l2.made, translate.module_file.c_whole_build_failed, translate.native_l2.module_entries
- test_the_nasm_writers_make_each_function_a_fragment -> COVERED: source.pe_writer.*, source.le_writer.* (assembly()), translate.l2.assembly; Python test fails today (see Findings)

### test_native_l2_core.py (1)
- test_l2_round_trip_and_schema_the_same_both_ways -> COVERED: translate.native_l2.round_trip_as_the_typed_tree_does, translate.native_l2.schema_check; native/l2/tests/test_l2.cc

### test_port_ownership.py (5)
- the five ownership cases (transient elements freed, retained not, inline-owned argument hoisted / kept, owned member reassign deletes once) -> COVERED: java.ownership.owned_containers, owned_pointers, program_wide_catalogues, translate.cpp03.feat (ownership patterns: 009_Own.java)

### test_port_semantics.py (5)
- test_every_body_comes_from_the_tree, test_spellings_the_runtime_and_passes_rely_on, test_finally_runs_on_every_way_out -> COVERED: translate.cpp03.feat (try/catch/finally with jumps, labelled jumps, casts, boxed types), translate.cpp03.text.*
- test_beside_the_jvm, test_helpers_beside_the_jvm -> COVERED: translate.judge_native_jvm.*, translate.javajudge.* (they need a JVM and run on machines with one)

### test_python_frontend.py (4)
- test_semantics_translate -> COVERED: translate.python.lower.*, translate.python.methods.*
- test_refusals -> COVERED: translate.python.lower.refusals_have_words_and_reasons, a_refusal_reaches_whoever_calls_the_refused
- test_int_proofs -> COVERED: translate.pytypes.interval_results, every_operation_contains_its_results, translate.wide.*
- test_semantics_compile_and_match -> COVERED: translate.python.package.* , translate.python.lower.functions_are_written_as_l2 (the compile-and-compare half needs a C++ compiler)

### test_pytypes.py (5)
- test_xor_interval_holds_every_result, test_xor_of_equal_values_may_be_zero -> COVERED: translate.pytypes.xor_lower_bound, every_operation_contains_its_results
- test_other_operations_hold_their_results -> COVERED: translate.pytypes.interval_results, unary_operations_contain_their_results
- test_structfmt_accepts_what_struct_accepts, test_structfmt_refuses_what_struct_refuses -> COVERED: translate.structfmt.integers, repeat_counts, empty_and_prefix, refused

### Python-only plumbing in tests/translate
- test_java_frontend.py::test_the_dumper_is_cached -> NOT NEEDED: caches the JVM tree dumper's output in Python
- (no other test of the folder tests only Python plumbing; the folder tests the translator's *products* (L2, back ends, front ends), all of which exist natively)
