# Batch 4: the Xbox / XNA group (36 modules) and the retro-machine lanes of the tangle

Roots as in batch 3 (every command but `ui` is native; the Python still reachable is the UI, `__main__`/`corpus`, the tools still used, and the tests the gate runs).
Scripts: `python_reach/{graph,live3,pool3,report,list4,cand4}.py` (`list4.py` prints the component of `test_platforms`, `cand4.py <mod,mod,...>` shows who outside a candidate set still imports it).

## Group 1: Xbox / XNA

Deleted: **36 modules, 9,603 lines** (`wc -l`): `xna/{__init__,fixups,mgfx,port,run}`, `formats/xnb`, `formats/xbox/*` (bink chd ciso effect fatx gdf god heavyiron nand pck shader stfs texture volumes xbe xdbf xma xpr xsb xur xwb xzp zar, `__init__`),
`formats/{gfx,inspect,media}`, `decompile/{xbox_effect,xenos_hlsl}`, `platforms/xbox` (the original Xbox's descriptor). Kept on purpose: `formats/xbox/{empty_live.bin,THIRD_PARTY.txt,crypto/{LICENSE,ATTRIBUTION.md}}`
(data and licences; docs/PYTHON_RETIREMENT.md 4.4 lists them) and `data/xna/*` (the C# the native code builds). Also deleted: the two golden generators under `native/cb/formats/xbox/testdata/assets`
(`make_asset_goldens.py`, `bench_assets.py`: they imported the deleted readers; the goldens they made are checked in).

Wiring changed: `COMMANDS` rows `media info heavyiron gfx xna` (all in `NATIVE`), `platforms.ORDER` loses `"xbox"`, `corpus._readable` no longer opens an `.xbe` (the native `corpus` does: `platforms.package.identify_xbe_reads_the_magic`,
`formats.xbox.xbe.to_pe_*`), `tests/run_all.py` rows `test_xna test_xbox_effect test_xbox_formats test_xbe test_inspect_render`.

Native twins of what was new (nothing of the Python is left without one):

- `native/cb/xna/effect_proof_test.cpp` (new): `test_xbox_effect.py::test_behaves_as_source` (every SDK sample effect recovered as HLSL, both compiled by mgfxc, drawn by `data/xna/shadercheck`, pixels agree within one step; skips without mgfxc/dotnet, as the Python did).
- `formats.xbox.shader.the_tables_of_the_skinned_effect_are_what_fxc_lists` (new): `test_shader_tables`.
- `formats.xbox.xzp.a_folder_is_packed_and_reads_back` (new) and `xzp::build` in `native/cb/formats/xbox/xzp.{hpp,cpp}`: the Python `xzp.build` / `xzp pack` had no native writer.
- `symbols.ordinals.the_xbox_kernels_ordinal_8_is_dbgprint` (new): the one check of `test_xbe.py` that was about the shipped ordinal table.

### The tests, case by case

| Python case | native test |
|---|---|
| test_xna `test_texture_both_byte_orders` | `xna.xnb.textures_in_both_byte_orders` |
| `test_dxt1_punch_through` | `formats.xbox.texture.a_dxt1_block_by_hand`, `xna.xnb.dxt1_in_both_word_orders` |
| `test_real_game_converts`, `test_xact_sound_bank` (skip without Arcadecraft) | `xna.xnb.real_compressed_content_matches_the_python`, `xna.xact.a_real_sound_bank_matches_the_python`, `xna.proof.*` |
| `test_fixups`, `test_obfuscated_forms` (data holders) | `xna.fixups.{ref_casts,constructors,cast_this_form_is_the_base_constructor,names_in_other_scripts,data_holders,apply_over_a_folder}` |
| `test_managed_and_decompile` | `formats.pe.*` (`is_dotnet`, `is_dotnet_file`), `xna.run.decompile_runs_ilspy_and_reports_its_failure` |
| `test_port_rewrites`, `test_port_effect_and_input` | `xna.port.rewrite_game`, `rewrite_leaves_what_it_does_not_know` (own types, ForPC, SetShaderIndex, Input.GetState, once only) |
| `test_port_error_fixers` | `xna.port.fix_{ref_out,access,color_and_vertex_binding,named_arguments_and_moved_members,shadowed_extensions}` |
| `test_rename_readers`, `test_port_reader_names` | `xna.xnb.reading_and_renaming_readers`, `xna.port.reader_names*` |
| `test_de4dot_detects` | `xna.run.deobfuscation` |
| `test_real_effects_convert`, `test_sound_effect` | `xna.xnb.xbox_effects_become_hlsl`, `an_effect_on_its_own`, `pcm_sounds_become_wav_files` |
| test_xbox_effect `test_effect_layout`, `test_unsupported_names_itself` | `formats.xbox.effect.*`, `decompile.xbox_effect.untranslated_shaders_name_the_shader_and_instruction` |
| `test_shader_tables` | `formats.xbox.shader.the_tables_of_the_skinned_effect_are_what_fxc_lists` (new), `loops_are_read_from_the_literal_records` |
| `test_recovered_hlsl`, `test_stock_effect_arrays`, `test_real_effects_recover` | `decompile.xbox_effect.old_harness_cases` (44 effects, 484 cases, goldens made by the Python), `stock_effect_becomes_one_technique_per_shader_index` |
| `test_mgfx_parameters` | `xna.mgfx.applied_twice_it_changes_nothing`, `parameter_names_lists_the_effects_own`, `real_effects_match_the_checked_result` |
| `test_behaves_as_source` | `xna.proof.recovered_hlsl_behaves_as_the_source` (new) |
| `test_fixtures_current` (needs the Xbox 360 SDK's fxc: it checks the committed .fxo against the SDK) | not ported: a check of test inputs against a tool this machine does not have; the .fxo files are inputs of the native tests |
| test_xbox_formats `test_stfs_*`, `test_god_round_trip`, `test_nxe_install_to_god` | `formats.xbox.stfs.*`, `formats.xbox.god.{write_and_read_back,install_to_god}`, `formats.xbox.volumes.*` |
| `test_fatx_read_and_recover`, `test_fatx_write` | `formats.xbox.fatx.{an_xbox_360_image,recover_finds_dirents_with_no_directory,add_a_file_and_read_it_back,directories_and_paths}` |
| `test_gpd_unlock`, `test_gpd_strings_split_on_unit_boundaries` | `formats.xbox.xdbf.{unlock_and_rebuild_as_test_gpd_unlock,strings_are_split_on_unit_boundaries}` |
| `test_xur`, `test_xur_offsets_are_from_the_start_of_the_file` | `formats.xbox.xur.{test_xur,later_sections_move_with_the_edit,text_outside_the_basic_plane_reads_back}` |
| `test_texture_untile`, `test_texture_offsets_and_gfx_bits` | `formats.xbox.texture.{tile_and_untile_are_inverses,tiled_offsets_*,dds_header_fields}`, `formats.gfx.*` |
| `test_xpr`, `test_xwb`, `test_xma_and_bink`, `test_nand_flash` | `formats.xbox.xpr.the_selfcheck`, `xna.xact.test_banks_in_both_byte_orders`, `pcm_is_wrapped_as_wav`, `xma2_in_a_riff_container_matches_the_python`, `formats.xbox.bink.test_xma_and_bink_header`, `formats.xbox.nand.the_flash_reads_as_the_pythons_does` |
| `test_xbe`, `test_xzp_round_trip` | `formats.xbox.xbe.header_certificate_and_keys`, `formats.xbox.summary.*`; `formats.xbox.xzp.a_folder_is_packed_and_reads_back` (new) |
| `test_identify`, `test_folder_map` | `formats.detect.identify_by_magic`, `formats.inspect.a_folder_is_mapped_by_format` |
| `test_real_stfs_theme/xzp/pck`, `test_real_heavyiron_and_gfx` (skip without ../xbox-samples; the fixture one runs) | `formats.xbox.volumes.*`, `formats.xbox.pck.*`, `formats.xbox.heavyiron.*`, `formats.gfx.truetype_fonts_have_the_tables_fonttools_wrote` |
| test_xbe.py (5 checks: platform, PE image, kernel thunk imports, ordinal 8, 32-bit lane, decompile) | `platforms.package.identify_xbe_reads_the_magic`, `formats.xbox.xbe.the_pe_image_of_the_test_xbe`, `symbols.ordinals.the_xbox_kernels_ordinal_8_is_dbgprint` (new); the decompile check skipped without the native decompiler |
| test_inspect_render (2) | `formats.inspect.render_makes_records_and_tables` (both nested-column cases, word for word) |
| test_media `test_fat16_put_in_partition`, `test_fat32_put` (cut; the cabinet cases stay) | `formats.media.{fat12_put_and_read_back,put_refuses_what_it_cannot_write,fat16_and_fat32_with_a_folder_that_grows}` |
| test_dos `test_boot_floppy`, `test_cd_image`, `test_not_an_image` (cut; the first two skip: no boot.img / Win1.0.iso in this corpus) | `formats.media.{old_floppy_without_a_parameter_block,iso_built_disc,things_that_are_not_images}` |
| test_ui inspector cases (5: `describe_32bit`, `16bit_driver_and_dos`, `never_raises`, `strings_bytes_and_resources`, `info_command`; cut; `test_terminal_interface` stays and reads exports with `formats.pe`) | `formats.inspect.{describe_a_pe_agrees_with_the_reader,describe_an_ne_agrees_with_the_reader,describe_a_driver_and_a_dos_program,damaged_files_are_described_never_thrown,strings_*,resource_bodies_are_shown_as_what_they_are,info_*}` |
| frozen subject `inspect_lzexpand` (+ fixture) | `formats.inspect.describe_equals_the_recorded_original` (lzexpand.dll among its 8 modules) |

### The five conditions (group 1)

1. native C++ committed: `native/cb/{xna,formats,formats/xbox,decompile}` (one file each; the older `native/typed/cpp/codebreaker/**` shims are untouched).
2. tests pass as C++: `tools\build_cb.cmd area formats` 606+ passed, `area xna` and `area symbols` below; every Python test that imported a deleted module was deleted or cut.
3. agreement with the Python: the native tests compare against goldens the Python made (`matches_the_original`, `old_harness_cases`, `real_compressed_content_matches_the_python`, ...).
4. callers: no live root, tool, or surviving module imports any of the 36 (`cand4.py`); Dashboard, Windows95, Win95VM, XboxMCP, LoganOS, Xbox, Xbox360Code, XboxTranspiler, Nexia360, xam: no hit but three comments naming `gdf.py`/`stfs.py` (Dashboard).
5. boot: none of the 36 is in the closure of `verify/gate16/*` / `source/module_writer`.

## Group 2a: the retro-machine lanes of the tangle (Z80 family, 68000, ZX/MSX/SMS/GB/Genesis)

Deleted: **31 modules, 5,100 lines**: `decompile/{z80,sm83,m68k}`, `emulate/{z80,sm83,m68k}`, `formats/{sms,gb,genesis,msx,zx}`, `build/{z80_asm,sm83_asm,m68k_asm}`,
`source/{sms,gb,genesis,msx,zx,z80}_writer`, `verify/lanes/{sms,gb,genesis,msx,zx,z80_flat}`, `platforms/{sms,gb,genesis,msx,zx}`; tests `tests/test_z80.py`, `tests/test_m68k.py`.
`platforms.ORDER` is now `java xbox360 nes win32 win16 vxd dos` (`reconstruct.KINDS` and `emulate.RUNNERS` are built from the registry, so they shrink with it; `test_platforms.py` expects the shorter tables).

Left on purpose, and why (this is as far as the tangle went):
- The NES lane (`platforms/nes`, `verify/lanes/nes`, `build/m6502_asm`, `source/nes_writer`): `test_nes.py` also drives the SLEIGH language (`tools/sleigh_verify_6502.py`, kept) and the native decompiler on the cartridge,
  which have no native twin in a test; and `verify/lanes/image_lane` stays because the NES lane imports it.
- `recompile/*` (13 modules) and `test_x360.py` / `test_recompile.py`: the Python tests compile the recompiled C with MSVC and run it; the native tests of `native/cb/recompile` compare against goldens, which is not the same check. Not mapped, not deleted.
- The Windows writers and lanes (`source/{dos,driver,le,ne,pe,image}_writer`, `verify/lanes/{dos,driver}`), `verify/authored/*`, `verify/gate16/*`, `verify/lanes/powerpc`: the boot closure stays until the integrator's all-15 boot on master passes; the others are held by `test_drivers`, `test_module_model`, `test_verification`.
- `translate/*` (the Python front end, C#, Java back ends), `java/projects/minecraft/*` and the `exact/*` plug-ins named by toolchain JSON: other components, not part of this tangle's retro half.

### The tests, case by case

| Python case | native test |
|---|---|
| test_z80 `test_z80_table_round_trip`, `test_sm83_table_round_trip` | `decompile.z80.{table_round_trip,decode_and_text}`, `decompile.sm83.{round_trip,decode_and_text}` (`JP (IX)`, `SET 0,(IX+$05),A`, `LDH`, the missing opcode) |
| `test_z80_flags`, `test_sm83_flags` | `emulate.z80.{add_sub_flags,logic_flags,compare_takes_bits_5_and_3_from_the_operand,daa,accumulator_group}`, `emulate.sm83.{add_adc_flags,sub_sbc_cp_flags,cb_rotates_and_shifts}` (SWAP) |
| `test_platforms_recognise` | `platforms.consoles.registry_identifies_each_by_its_probe`, `platforms.{sms,gb,genesis}.descriptor` |
| `test_sms_source_rebuilds`, `test_gb_source_rebuilds` | `source.sms_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom`, `source.gb_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom`, `..._the_golden_names_what_the_code_touches` |
| `test_sms_lane_judges`, `test_gb_lane_judges` | `engine.lane_kit.machines.the_master_system_demo_cartridge_is_judged`, `the_game_boy_demo_cartridge_is_judged` (new: the repository's own demos, built by our assemblers; the homebrew-sample tests `master_system_edited_program`, `game_boy_edited_program` stay) |
| `test_spectrum_lane` | `formats.zx.*` (tape and snapshot), `source.zx_writer.{a_tape_comes_back_as_source_that_builds_the_same_bytes,a_snapshot_stopped_at_the_start_is_the_same_program}` (new), `engine.lane_kit.machines.the_zx_demo_snapshot_is_judged` (new), `verify.lanes.zx` |
| `test_msx_lane` | `formats.msx.*`, `source.msx_writer.*`, `engine.lane_kit.machines.the_msx_demo_cartridge_is_judged` (new), `msx_edited_program`, `verify.lanes.msx` |
| test_m68k `test_table_round_trip`, `test_assembler_refuses_what_it_used_to_get_wrong` | `decompile.m68k.round_trip_of_every_opcode_word`, `the_shapes_are_the_originals`, `decompile.m68k.assemble.errors` |
| `test_flags` | `emulate.m68k.{add_sets_all_five_flags,sub_and_cmp_flags,logic_clears_v_and_c_and_leaves_x,shifts,...}` |
| `test_platform_recognises`, `test_source_rebuilds`, `test_lane_judges` | `platforms.genesis.descriptor`, `source.genesis_writer.the_cartridge_comes_back_as_source_that_builds_the_same_rom`, `engine.lane_kit.machines.the_genesis_demo_cartridge_is_judged`, `genesis_edited_program` |
| test_platforms (cut: the expected `KINDS` and `RUNNERS` tables lose the deleted kinds; the rest stays) | `platforms.package.*`, `platforms.consoles.*` |
| test_run_record (cut: the sms/gb/genesis runner checks; `checked` needs 2 kinds, not 5, so a machine with no corpus still passes) | `emulate.z80.record`, `emulate.sm83.record_is_the_run_record`, `emulate.m68k.record_is_the_run_record` |
| test_port_fixes `test_sms_writer_game_gear_rom` (cut) | `source.sms_writer.the_game_gears_own_port_is_named_for_a_game_gear_only` |
| run_all.py rows `test_z80.py`, `test_m68k.py` | removed |

New native test inputs: `native/cb/engine/testdata/lane_kit/{sms,gb,msx,zx}_demo.asm` (copies of `tests/data/*/demo.asm`, which stay).
