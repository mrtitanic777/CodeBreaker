# Batch 8: the Python Java world is deleted (every case accounted for)

Roots, scripts and conditions as in batch 7 (`python_reach/g7.py`, `twin7.py`, `tools/check_imports.py`). The Python Java component was held by about 57 test files; batch 7 left it because `twin7.py` (a name-similarity hint) found
114 of the 328 cases of `tests/java/*` with no obvious native twin. This batch read each of those 114 against the native tests (the Python case and the native test side by side, not by name), found the native test that checks the
same behaviour, or ported the case as a native test. Earlier maps (`docs/history/python-port/test_port_map.md`, third pass: the 114 literal-flagged cases; `test_port_map_2.md` for `tests/translate/*` and `tests/test_port_fixes.py`) were reused where they cite
a specific native test; 24 of the 114 are in that third pass, 90 were read fresh here.

## Result

- 114 cases: 101 checked against an existing native test (cited below), 12 ported (one new file, `native/cb/java/batch8_ports_test.cpp`, 11 native tests), 1 not needed (why below).
- Ports (the cases whose literals no native test spelled): side-effect-in-branch with a pure branch and a nested early-return guard; silent-dispatch with an interior comment and a terminal `J2X_UNHANDLED`; virtual-call-in-ctor with a known empty hierarchy;
  `derive_nullability` (the native golden had no getter in it); the controller-nav GuiScreen body and pointer draw; the probe's case count for eight doubles; a reflection-factory `typeid(*(this))` key; the runtime's GLContext/Entry/Runtime/clipboard stubs (listFiles is `java.corpus.stub_member_types`);
  an unknown capitalised class is a reference; `BlockBed` makes beta; findings of one line in name order (test_port_fixes).
- Not needed: `test_profiles_selection.test_runtime_mutation_does_not_pollute_profile` (the native profile is const and a conversion hands its profile down: no process-wide typemap to mutate).
- The 5 other holders of the Java world (`tests/translate/{test_cpp03_bodies,test_cpp17_backend,test_port_ownership,test_port_semantics,test_console_patches}`) were mapped case by case in `test_port_map_2.md` (translate.cpp03.*, translate.cpp17.*, translate.desugar.*,
  java.ownership.*, java.minecraft.{sound_fix,applet_ping,render_fix}.*, translate.javajudge/judge_native_jvm; the two Modern.java/Tally.java cases were ported in batch 7, `java.cpp17.*`). `tests/test_port_fixes.test_java_rules_findings_of_one_line_come_in_name_order` is `java.batch8.findings_of_one_line_come_in_name_order`.

## Deleted (one commit)

`codebreaker/java/**` (56 files, 14,657 lines; the Minecraft project, rules, transpile, probe, runtime, emit ...), `codebreaker/verify/lanes/java.py` (224), `codebreaker/translate/backends/{cpp03 (1,857), jmodel (494)}`, `codebreaker/translate/{javajudge (304), rewrite (114)}`;
tests: `tests/java/**` (51 files + fixtures), `tests/test_java.py`, `tests/translate/{test_cpp03_bodies,test_cpp17_backend,test_port_ownership,test_port_semantics,test_console_patches}`, one case of `tests/test_port_fixes`.
Python under `codebreaker/`: 210 -> 149 modules, 59,201 -> 41,585 lines (-17,616 net). Tests: -6,567 lines.

Kept, and the small edits that let the rest stand:
- `codebreaker/translate/{desugar,backends/cpp17,frontends/java/**}`, `translate/{ir,l2,native_l2,lower ...}`: the Java -> L2 front end and the C++17 shim are another component (held by `test_l2`, `test_java_lower`, `test_java_frontend`, `test_native_l2_core`). `desugar` took three small helpers from cpp03
  (`_effects`, `_written`, `_read`, 30 lines); `cpp17.Bodies` (its only callers were cpp03/jmodel/transpile) is gone; the three tests' `verify.lanes.java.jdk()` is now `frontends.java.jdk()`.
- `codebreaker/platforms/java.py`: the descriptor loses the lane and the `compiler.tool` / `packager.tools` Refs (all three pointed into the deleted lane); `recognise` is inline. The descriptor stays (`platforms/__init__` loads it by name).
- `tests/run_all.py` loses `test_java.py` and the `java` shard; `tests/test_port_fixes.py` loses the one java case. `tidy --apply` removed what that orphaned (`cab.COMPRESSION`, `cpp17.RUNTIME`, `cpp17.source_path`).
- `tests/java/fixtures/java` (nine Java classes) was read by native tests (`java.tools.*`, `source.pipeline` slow case): moved to `native/cb/java/testdata/tools_fixtures` (git mv) and the two `data_path` calls follow it. Found by running `area java` after the deletion (5 `java.tools.*` failures); the first grep had only matched comments.
- `native/cb/translate/java_ports_test.cpp`: the probe-shims data path pointed into `codebreaker/java`; it is `data/java` now (the folder did not hold the shims anyway).
- `native/typed/cpp/codebreaker/java/*.hpp` (shims for the not-yet-refactored ports) and `run_tools.cpp` are not touched: they are C++ source, they do not read the Python.

## The five conditions, for the Python Java world

1. Their C++ is committed as source: `native/cb/java/**` (every module of `codebreaker/java`, 590+ native tests), `native/cb/verify/lanes/java*`, `native/cb/translate/{cpp03, javajudge, jmodel, desugar...}`, `native/cb/engine/lane_kit_java*`.
2. Their tests pass as C++: `area java` and `area translate` (below), 114 cases mapped here, the other 214 by name in `twin7.py` plus the earlier maps.
3. The judge traces agree: the Python-made goldens are checked in and read (batches 3-7); the behavioural judge is native (`java`, `judge-c-java` in `NATIVE`).
4. Every caller runs through codebreaker.exe: `java` is in `NATIVE` (no COMMANDS row); `check_imports` 0 dangling; no tool or test imports a deleted module (searched `native tools tests data docs` for the module paths: comments and the shims' headers only).
5. All-15 boot: not run here (the integrator's); the boot path reaches none of this Python.

## The 114 cases

### tests/java/test_achievements.py

- `scan_finds_all_and_flags_special` -> java.minecraft.achievements.scan_achievements (keys, hidden via setSpecial, parent, NULL no parent)
- `gamerscore_sums_exactly_to_budget` -> java.minecraft.achievements.map_to_platform (1000 total, arcade 200, ids from 1)
- `arcade_count_limit_warns` -> java.minecraft.achievements.map_to_platform_warnings (13 exceeds arcade limit)
- `local_delivery_is_stub` -> java.minecraft.achievements.run_without_art_says_why (delivery local: J2X-STUB[achievements-local] in the code)
- `wire_icons_crops_and_writes_tiles` -> java.minecraft.achievements.run_matches_the_python_module (tiles pixel for pixel, 64x64, locked darker) + tile_pixels_by_hand (third-pass map, read side by side)

### tests/java/test_apimap.py

- `languages_and_dialects_loaded` -> java.apimap.loads_real_data (languages, dialect order)
- `resolve_qualified_and_leaf` -> java.apimap.qualified_and_unknown_names + resolve_matches_python (golden of the Python's answers)
- `version_axis_selects_newest_at_or_below` -> java.apimap.version_selection (c++03 NULL, c++11/17 newest at or below)
- `version_axis_math_overload` -> java.apimap.resolve_matches_python (golden rows over the real data incl. Math.sqrt by dialect) + version_selection
- `equiv_bridge_present` -> java.apimap.legacy_equivalents (the java->xbox360 intent rows; dead = unsupported)
- `symbols_in_dedup_canonical` -> java.apimap.version_selection (symbols_in keeps the most qualified key) + symbols_in_and_stats_match_python
- `render_coverage_md_smoke` -> java.apimap.coverage_report

### tests/java/test_applet_ping.py

- `ping_dropped_level_set_directly` -> java.minecraft.applet_ping.rewrite_matches_golden (same assertions: no URL/catch, level assigned, load kept)

### tests/java/test_awt_stubs.py

- `point_has_x_y_fields` -> java.awt_stubs.the_mouse_stubs_are_generated_and_registered
- `pointerinfo_getlocation_returns_valid_point` -> java.awt_stubs.the_mouse_stubs_are_generated_and_registered
- `mouseinfo_getpointerinfo_returns_pointerinfo` -> java.awt_stubs.the_mouse_stubs_are_generated_and_registered (namespace MouseInfo, PointerInfo getPointerInfo)
- `awt_types_registered_for_static_and_include` -> java.awt_stubs.the_mouse_stubs_are_generated_and_registered

### tests/java/test_beta_fixes.py

- `constant_only_enum_stays_int` -> java.header.constant_enum (struct of int constants) + java.types.typed_enum_is_a_pointer_class (EnumMobType, constants only, stays int/value) + java.types.enums_and_arrays_are_values
- `marker_and_bridge_ctor_dropped_but_base_kept` -> java.corpus.markers_that_are_bases_stay (marker dropped, base kept, bridge ctor dropped, private ctor kept)
- `hotpath_alloc_flags_new_in_loop` -> java.rules.hotpath_alloc (new in a loop of a hot method: High, line, snippet)
- `hotpath_alloc_skips_constructor_loop` -> java.rules.hotpath_alloc (a loop in a constructor is not flagged)
- `factory_and_init_allocations_owned` -> java.ownership.owned_pointers (later/viaFactory: allocated in init() and by an owning factory)
- `borrowed_param_pointer_not_owned` -> java.ownership.owned_pointers (borrowed = this.t from a ctor parameter is not owned) + owned_containers owns_heap
- `ambiguous_field_excluded` -> java.ownership.owned_pointers (mixed: new in one path, aliased in another: excluded)
- `container_of_new_owned_listener_list_not` -> java.ownership.owned_containers (items/slots owned; listeners holding passed-in observers are not)
- `dtor_frees_scalars_and_container_not_borrowed` -> java.beta_fixes.the_destructor_frees_scalars_and_containers_it_owns (ported earlier)
- `getcubes_recognized_as_owned_return` -> java.ownership.returned_ownership (cubes() owned) + program_wide_catalogues
- `field_initialized_new_is_owned` -> java.ownership.owned_pointers (made: declared with new; value: a by-value struct is not)
- `interior_comment_does_not_truncate_chain` -> java.rules.silent_dispatch (comment between branches does not cut the chain; terminal J2X_UNHANDLED/NOOP/else = not flagged) + ported: java.batch8.silent_dispatch_with_a_comment_and_a_terminal
- `still_flags_a_genuinely_silent_chain` -> java.rules.silent_dispatch (High, 'dispatch on pname (2 cases)')
- `not_flagged_when_no_subclass_overrides` -> ported: java.batch8.virtual_call_in_ctor_with_a_known_hierarchy (override_pairs empty); also java.rules.virtual_call_in_ctor.only_overridden_methods_are_flagged_when_the_hierarchy_is_known
- `infinite_loop_flagged_without_break` -> java.rules.infinite_loop
- `infinite_loop_not_flagged_with_break` -> java.rules.infinite_loop (break inside: not flagged)
- `value_return_null_not_derived` -> ported: java.batch8.nullability_is_derived_for_references_only
- `seed_priority_preserved` -> java.init_driver.the_seed_order_of_a_profile_is_kept
- `flags_impure_return_branch` -> java.rules.side_effect_in_branch (loadTexture then return)
- `pure_branch_not_flagged` -> ported: java.batch8.side_effect_in_branch_pure_and_nested_guards
- `return_value_call_not_flagged` -> java.rules.side_effect_in_branch ('the returned value is the call')
- `nested_early_return_guard_not_flagged` -> ported: java.batch8.side_effect_in_branch_pure_and_nested_guards
- `real_side_effect_assignment_flagged` -> java.rules.side_effect_in_branch ('a field assignment is a side effect too')
- `missing_gl11_constants_now_present` -> java.beta_fixes.the_gl11_stub_has_the_constants_the_corpus_uses (ported earlier)
- `missing_gl11_functions_now_present` -> java.beta_fixes.the_gl11_stub_has_the_functions_the_corpus_uses (ported earlier)

### tests/java/test_collection_out_params.py

- `direct_mutation_promoted` -> java.corpus.a_collection_a_method_mutates_is_an_out_parameter (translate/java_ports_test.cpp)
- `nonoverride_forwarding_promoted` -> java.corpus.forwarding_a_collection_to_an_out_slot_makes_it_one
- `emitter_renders_reference` -> java.ownership.a_promoted_collection_is_a_reference_in_the_signature

### tests/java/test_controller_nav.py

- `guiscreen_cpp_gets_ctor_init_and_body` -> java.minecraft.controller_nav.patch_guiscreen_cpp (ctor init inside the init list, includes, body) + ported: java.batch8.controller_nav_bodies (Input::menuNavSelect/menuSnapCursor)
- `callsite_injected_before_updatescreen` -> java.minecraft.controller_nav.patch_minecraft_callsite
- `fail_loud_on_missing_anchor` -> java.minecraft.controller_nav.patch_guiscreen_cpp (no init-list ctor: ok false) + glue_is_all_or_nothing
- `displayguiscreen_injects_cursor_and_grab_invariant` -> java.minecraft.controller_nav.patch_displayguiscreen (grab + g_guiCursorMode, idempotent by its own marker)
- `grab_invariant_applies_without_container` -> java.minecraft.controller_nav.patch_displayguiscreen (container=false: setGrabbed, no g_guiCursorMode)
- `pointer_draw_injected_after_drawscreen` -> java.minecraft.controller_nav.patch_pointer_namespace (d3d9 vs GL11) + ported: java.batch8.controller_nav_bodies (Pointer.png, g_guiCursorMode, after drawScreen)
- `mousehelper_grab_made_console_safe` -> java.minecraft.controller_nav.patch_mousehelper

### tests/java/test_edition_autodetect.py

- `indev_finite_world_no_chunks` -> java.minecraft.edition.detect (finite world: World/LevelLoader/Material = indev, a lone World is not alpha)
- `infdev_chunked_game_package` -> java.minecraft.edition.detect (Chunk/ChunkProvider in net.minecraft.game.level = infdev)
- `alpha_flat_src_with_chunks` -> java.minecraft.edition.detect (Chunk in net.minecraft.src = alpha)

### tests/java/test_edition_convert_integration.py

- `explicit_edition_activates_then_restores` -> java.edition_convert.* (translate/java_ports_test.cpp, the application's assembly); 'restores' is not applicable: the profile of a conversion is handed down, no process-wide profile is switched (java.transpile.a_conversion_that_fails_midway_leaves_the_profiles_alone)

### tests/java/test_edition_resolve.py

- `infdev_wins_over_indev_prefix` -> java.minecraft.edition.version_ids (inf-20100618 -> infdev, in-... -> indev)
- `resolve_precedence_explicit_over_version` -> java.minecraft.edition.resolve_precedence (beta over a1.2.6)
- `resolve_accepts_qualified_profile_name` -> java.minecraft.edition.resolve_precedence (minecraft_infdev explicit)
- `resolve_none_when_nothing_conclusive` -> java.minecraft.edition.resolve_precedence (nothing conclusive: no profile)

### tests/java/test_host_compile_fixes.py

- `a_member_reached_through_a_receiver_is_not_the_scope_name` -> java.emitted.host_compile_fixes_text (translate/java_ports_test.cpp; var3->inventory.length())
- `sized_new_with_an_indexed_size` -> java.emitted.host_compile_fixes_text (j2x_newarr2 and the text after ']')
- `null_arm_of_a_value_return_is_the_empty_value` -> java.emitted.host_compile_fixes_text (JavaString(), JObject(), JArray<Probe* >())
- `no_platform_tree_has_what_its_runtime_includes` -> java.emitted.host_compile_fixes_text (Inflate.h/.cpp, NOGDI, GL_LINEAR_MIPMAP_LINEAR) + java.emitted.every_class_and_the_runtime_compile (g++)

### tests/java/test_ir.py

- `array_dims_and_element` -> java.ir.jtype_text + java.ir.jtype_element_and_flags
- `element_of_scalar_is_self` -> java.ir.jtype_element_and_flags (a scalar is its own element)
- `owns_heap_field_signal` -> java.ir.class_queries (owns_heap_field: a field initialised with new, and not an identifier starting with new)

### tests/java/test_kind.py

- `a_jar_asks_to_be_decompiled_first` -> verify.lanes.java.a_jar_is_refused (the lane's prepare refuses a .jar; the message text is not Python's)

### tests/java/test_pipeline.py

- `inherited_fields_populated` -> java.corpus.passes_match_python (golden: ChildFieldy inherits count, not the private isRunning; Puppy transitive) + java.ir.derived_members_point_into_other_classes
- `guard_leaves_non_minecraft_driver_untouched` -> java.pipeline.* in java/pipeline_ports_test.cpp (Widget::pick with GL_SELECT left intact) + java.minecraft.raycast.*

### tests/java/test_platform_coverage_symbols.py

- `functions_constants_and_enumerators` -> java.minecraft.platform_coverage.provided_symbols (+ _edges)

### tests/java/test_platform_refs.py

- `detects_static_string_array_main` -> java.platform_refs.detect_main_class (RubyDung; no main; instance main not an entry)

### tests/java/test_probe.py

- `cases_are_deterministic_and_cover_every_value` -> java.probe.inputs (deterministic, every pool value of every parameter appears; 15 values of int)
- `floats_compare_by_bits_and_nan_matches_nan` -> java.probe.compare

### tests/java/test_probe_clock.py

- `only_the_static_pure_method_is_probed` -> java.probe.a_class_that_reads_the_clock_in_its_constructor_and_a_method (ported earlier)

### tests/java/test_probe_objects.py

- `cases_never_list_every_combination` -> ported: java.batch8.probe_cases_never_list_every_combination

### tests/java/test_profile_invariants.py

- `nullable_getter_return_types_are_subset` -> java.minecraft.editions.every_profile_satisfies_the_structural_invariants
- `render_rule_precedes_entity_block_fallbacks` -> java.minecraft.editions.every_profile_satisfies_the_structural_invariants
- `all_six_editions_registered` -> java.minecraft.editions.* (minecraft_editions_test.cpp: 'all six editions are there, each under its own name')

### tests/java/test_profiles_selection.py

- `generic_empties_globals` -> java.profiles.generic_baseline (the generic profile is registered, active and empty: no nullable fields, no heavy types, no static-init seed); the native profile is const, no globals to empty
- `runtime_mutation_does_not_pollute_profile` -> not needed: the native profile is const and a conversion hands its profile down; there is no process-wide typemap to mutate or reseed

### tests/java/test_projects.py

- `a_plain_program_loads_no_minecraft_code` -> java.transpile.convert_matches_python (generic profile) + java.projects.project_lookup (a manifest without code in the set is unsupported)
- `the_font_glyph_is_minecrafts` -> java.minecraft.package (char_map 0x2302 -> 127) + translate.backends.cpp03 char_map

### tests/java/test_reflection_factory.py

- `world_arg_factory` -> java.reflection_factory.run_matches_python (EntityList World-arg registry, golden files read and reviewed) + header + process_details
- `dynamic_getclass_not_registered` -> ported: java.batch8.a_dynamic_getclass_key_is_not_a_registration
- `j2xclassof_registration_format` -> java.reflection_factory.process_details (Class(typeid(Foo)) and j2xClassOf(typeid(Bar)) both register) + registrars_match_lines
- `run_generates_and_reuses` -> java.reflection_factory.run_matches_python (generates both registries, reuses them in other files, a third run changes nothing)

### tests/java/test_review_fixes.py

- `convert_restores_profile_and_getters_when_it_raises` -> java.transpile.a_conversion_that_fails_midway_leaves_the_profiles_alone
- `controller_nav_writes_nothing_when_one_anchor_is_missing` -> java.minecraft.controller_nav.glue_is_all_or_nothing (the three files untouched, 'all-or-nothing' reported)
- `beta_only_class_still_detects_beta` -> ported: java.batch8.a_class_only_beta_has_still_detects_beta (BlockBed in net.minecraft.src is beta, not alpha)

### tests/java/test_rules.py

- `call_on_other_object_not_flagged` -> java.rules.virtual_call_in_ctor.calls_that_are_fine
- `cross_static_reference_flagged` -> java.rules.static_init_order
- `compile_time_constant_not_flagged` -> java.rules.static_init_order
- `runtime_value_type_new_not_flagged` -> java.rules.static_init_order
- `zero_init_def_not_flagged` -> java.rules.static_init_order
- `reinterpret_in_nbt_file_flagged` -> java.rules.endianness
- `memcpy_in_save_file_flagged` -> java.rules.endianness
- `raw_cast_in_non_io_file_not_flagged` -> java.rules.endianness

### tests/java/test_runnable_entry.py

- `detects_concrete_toplevel_runnable` -> java.platform_refs.detect_runnable_game_class
- `abstract_runnable_is_ignored_beta_guard` -> java.platform_refs.detect_runnable_game_class
- `ui_runnable_with_superclass_is_ignored` -> java.platform_refs.detect_runnable_game_class
- `glcontext_stub_has_gl3_caps` -> ported: java.batch8.runtime_stub_texts

### tests/java/test_runtime_stubs.py

- `static_factory_from_extra_line` -> java.runtime.class_stub_text_and_members (Toolkit.getDefaultToolkit -> Toolkit*) + ported java.batch8.runtime_stub_texts (Runtime not a stub)
- `ambiguous_returns_omitted` -> java.corpus.stub_member_types (listFiles omitted) + ported java.batch8.runtime_stub_texts (getSystemClipboard, listFiles)
- `jmap_header_emitted` -> java.runtime_stubs.jmap_has_the_java_map_surface_and_list_and_set_an_iterator_member
- `entry_carries_key_value` -> ported: java.batch8.runtime_stub_texts (Entry.h)

### tests/java/test_types.py

- `unknown_capitalized_is_pointer` -> java.types.arrays (Mystery[] is a reference array) + ported java.batch8.an_unknown_capitalized_class_is_a_reference
- `value_type_array_holds_values` -> java.types.arrays (JArray<Vec3D >, JArray<JavaString >)
- `two_dimensional_arrays` -> java.types.arrays
- `generic_vector_of_value` -> java.types.generics_and_raw_containers
- `raw_container_defaults` -> java.types.generics_and_raw_containers
- `generic_map_element_types` -> java.types.generics_and_raw_containers (HashMap<String,Item> -> JMap<JavaString, Item*>)

### tests/java/test_ui_mark.py

- `mark_inserts_greppable_marker` -> java.minecraft.ui_mark.mark_and_index
- `debug_off_bakes_disabled` -> java.debug (debug_test.cpp: DEBUG_ENABLED 0 with enabled_default off)
