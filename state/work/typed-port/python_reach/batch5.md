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
