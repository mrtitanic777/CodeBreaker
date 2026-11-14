| module | lines | held by (tests/tools before this batch; modules importing it) | native replacement |
|---|---:|---|---|
| `analyze/__init__.py` | 23 | test_analysis, test_dos, test_port_fixes, translate; 10 modules | native/cb/java/analyze.hpp (by name) |
| `analyze/call_graph.py` | 110 | test_analysis; 3 modules | native/cb/analyze/call_graph.hpp (by name) |
| `analyze/classes.py` | 533 | test_analysis | native/cb/analyze/classes.hpp (by name) |
| `analyze/conventions.py` | 136 | test_analysis; 4 modules | native/cb/analyze/conventions.hpp (by name) |
| `analyze/dispatch.py` | 404 | test_analysis; 1 modules | native/cb/analyze/dispatch.hpp (by name) |
| `analyze/dos_functions.py` | 516 | test_dos; 2 modules | native/cb/analyze/dos_functions.hpp (by name) |
| `analyze/globals.py` | 105 | test_analysis; 1 modules | native/cb/analyze/globals.hpp (by name) |
| `analyze/image.py` | 524 | test_analysis, translate; 6 modules | native/cb/analyze/image.hpp (by name) |
| `analyze/native.py` | 157 | 2 modules | native/cb/analyze/native.hpp (by name) |
| `analyze/origin.py` | 489 | test_port_fixes; 1 modules | native/cb/analyze/origin.hpp (by name) |
| `analyze/strings.py` | 191 | test_analysis; 3 modules | native/cb/analyze/strings.hpp (by name) |
| `analyze/structures.py` | 421 | test_analysis, translate; 2 modules | native/cb/analyze/structures.hpp (by name) |
| `base/__init__.py` | 0 | frozen_subjects, run_all, test_analysis, test_assembler, test_concolic, test_converted, test_dos, test_drivers, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_run_record, test_symbols, test_ui, test_ui_thin, test_verification, test_versions, test_xex, tools.freeze_tests, tools.golden, tools.l2c; 57 modules | (empty package marker) |
| `base/cache.py` | 591 | test_plumbing, test_verification; 10 modules | native/cb/base/cache.hpp (by name) |
| `base/converted.py` | 188 | test_converted; 2 modules | native/cb/base/converted.hpp (by name) |
| `base/corpus.py` | 292 | frozen_subjects, test_analysis, test_concolic, test_dos, test_drivers, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_run_record, test_symbols, test_ui, test_verification, test_versions, test_xex, tools.freeze_tests, tools.l2c; 24 modules | native/cb/base/corpus.hpp (by name) |
| `base/parallel.py` | 268 | run_all, test_plumbing, test_verification; 4 modules | native/cb/base/parallel.hpp (by name) |
| `base/settings.py` | 286 | test_assembler, test_dos, test_drivers, test_verification, test_versions, tools.golden, tools.l2c; 41 modules | native/cb/base/settings.hpp (by name) |
| `build/__init__.py` | 0 | msc8, test_dos, test_drivers, test_exepack, test_flat_thunk, test_pklite, test_plumbing, test_port_fixes, test_powerpc, test_readable, test_symbols, test_verification, tools.make_api_table; 25 modules | (empty package marker) |
| `build/asm16.py` | 674 | test_verification | native/cb/build/asm16.hpp (by name) |
| `build/data.py` | 530 | 3 modules | native/cb/build/data.hpp (by name) |
| `build/definitions.py` | 208 | 2 modules | native/cb/build/definitions.hpp (by name) |
| `build/dos_sessions.py` | 185 | 1 modules | native/cb/build/dos_sessions.hpp (by name) |
| `build/dos_tools.py` | 77 | test_dos; 2 modules | native/cb/build/dos_tools.hpp (by name) |
| `build/driver.py` | 332 | test_drivers; 4 modules | native/cb/build/driver.hpp (by name) |
| `build/exepack.py` | 107 | test_exepack | native/cb/build/exepack.hpp (by name) |
| `build/flat_thunk.py` | 1085 | test_flat_thunk; 2 modules | native/cb/build/flat_thunk.hpp (by name) |
| `build/functions.py` | 678 | test_powerpc, test_readable, test_verification; 3 modules | native/cb/build/functions.hpp (by name) |
| `build/masm.py` | 442 | test_dos; 2 modules | native/cb/build/masm.hpp (by name) |
| `build/msc6.py` | 924 | msc8, test_dos, test_verification; 1 modules | native/cb/build/msc6.hpp (by name) |
| `build/msvc152.py` | 79 | test_port_fixes; 1 modules | native/cb/build/msvc152.hpp (by name) |
| `build/native_dos.py` | 207 | test_dos; 1 modules | native/cb/build/native_dos.hpp (by name) |
| `build/pklite.py` | 380 | test_pklite | native/cb/build/pklite.hpp (by name) |
| `build/resources.py` | 658 | 1 modules | native/cb/build/resources.hpp (by name) |
| `build/watcom.py` | 160 | msc8, test_plumbing, test_readable, test_symbols, test_verification, tools.make_api_table; 11 modules | native/cb/build/watcom.hpp (by name) |
| `compare/__init__.py` | 0 | test_symbols; 4 modules | native/cb/exact/compare.hpp (by name) |
| `compare/mask.py` | 90 | test_symbols; 4 modules | native/cb/compare/mask.hpp (by name) |
| `decompile/__init__.py` | 0 | frozen_subjects, test_dos, test_frozen, test_plumbing, test_port_fixes, test_verification, test_vmx128, translate, tools.freeze_tests, tools.golden, tools.l2c; 21 modules | (empty package marker) |
| `decompile/asm_blocks.py` | 267 | translate; 2 modules | native/cb/decompile/asm_blocks.hpp (by name) |
| `decompile/c_text.py` | 1428 | frozen_subjects, test_frozen, translate, tools.freeze_tests; 5 modules | native/cb/decompile/c_text.hpp (by name) |
| `decompile/decompiler.py` | 893 | test_dos, test_frozen, test_plumbing, test_verification, translate, tools.freeze_tests, tools.golden, tools.l2c; 5 modules | native/cb/decompile/decompiler.hpp (by name) |
| `decompile/describe.py` | 2104 | frozen_subjects, test_dos, test_port_fixes, test_verification, tools.golden; 14 modules | native/cb/decompile/describe.hpp (by name) |
| `decompile/frame16.py` | 236 | translate; 2 modules | native/cb/decompile/frame16.hpp (by name) |
| `decompile/inline_asm.py` | 191 | 2 modules | native/cb/decompile/inline_asm.hpp (by name) |
| `decompile/jar.py` | 188 | 2 modules | native/cb/decompile/jar.hpp (by name) |
| `decompile/shape16.py` | 111 | translate; 2 modules | native/cb/decompile/shape16.hpp (by name) |
| `decompile/struct_text.py` | 114 | translate; 1 modules | native/cb/decompile/struct_text.hpp (by name) |
| `decompile/vmx128.py` | 374 | test_vmx128 | native/cb/decompile/vmx128.hpp (by name) |
| `emulate/__init__.py` | 45 | test_concolic, test_knowledge, test_native_trace, test_plumbing, test_powerpc, test_run_record, test_verification; 15 modules | native/cb/emulate/ (the facade: its names are the native emulate headers) |
| `emulate/driver.py` | 217 | test_knowledge, test_native_trace; 1 modules | native/cb/emulate/driver.hpp (by name) |
| `emulate/emulator.py` | 616 | test_concolic, test_native_trace, test_run_record; 5 modules | native/cb/emulate/emulator.hpp (by name) |
| `emulate/fast_uc.py` | 69 | test_plumbing; 1 modules | none needed: a Python-binding speed-up; the C++ calls Unicorn directly (emulate/unicorn_uc.hpp) |
| `emulate/native_driver.py` | 204 | 1 modules | native/cb/emulate/native_driver.hpp (by name) |
| `emulate/native_shadow.py` | 239 | 4 modules | native/cb/emulate/native_shadow.hpp (by name) |
| `emulate/native_trace.py` | 390 | test_native_trace; 4 modules | native/cb/emulate/native_trace.hpp (by name) |
| `emulate/native_win16.py` | 209 | 1 modules | native/cb/emulate/native_win16.hpp (by name) |
| `emulate/pcode.py` | 147 | test_concolic, test_native_trace; 2 modules | native/cb/emulate/pcode.hpp (by name) |
| `emulate/ports16.py` | 195 | test_verification; 1 modules | native/cb/emulate/ports16.hpp (by name) |
| `emulate/powerpc.py` | 916 | test_concolic, test_powerpc; 2 modules | native/cb/emulate/powerpc.hpp (by name) |
| `emulate/run.py` | 76 | test_run_record; 4 modules | native/cb/emulate/run.hpp (by name) |
| `emulate/shadow.py` | 681 | test_concolic, test_native_trace; 2 modules | native/cb/emulate/shadow.hpp (by name) |
| `emulate/symbolic.py` | 193 | test_concolic; 2 modules | native/cb/emulate/symbolic.hpp (by name) |
| `emulate/windows16.py` | 1138 | test_concolic, test_knowledge, test_native_trace, test_run_record, test_verification; 4 modules | native/cb/emulate/windows16.hpp (by name) |
| `engine/__init__.py` | 17 | test_engine, test_knowledge, test_knowledge_reads, test_native_engine, test_store, test_ui_thin; 6 modules | native/cb/engine/engine.hpp (by name) |
| `engine/engine.py` | 268 | test_engine, test_knowledge_reads, test_native_engine; 1 modules | native/cb/engine/engine.hpp (by name) |
| `engine/facts.py` | 182 | test_engine, test_knowledge, test_native_engine, test_store; 3 modules | native/cb/engine/facts.hpp (by name) |
| `engine/native_engine.py` | 1048 | test_native_engine; 5 modules | native/cb/engine/{function_store,engine,facts}.hpp (this was the ctypes shell over cbengine.dll) |
| `exact/__init__.py` | 0 | test_dos, test_dpmi, test_port_fixes; 4 modules | (empty package marker) |
| `exact/aout.py` | 82 | 1 modules | native/cb/exact/aout.hpp (by name) |
| `exact/emu/__init__.py` | 0 | test_dos, test_dpmi, test_port_fixes; 3 modules | (empty package marker) |
| `exact/emu/dos.py` | 803 | test_dos; 2 modules | native/cb/exact/emu/dos.hpp (by name) |
| `exact/emu/dpmi.py` | 736 | test_dpmi; 1 modules | native/cb/exact/emu/dpmi.hpp (by name) |
| `exact/emu/native_dosexec.py` | 163 | test_port_fixes; 1 modules | native/cb/exact/emu/native_dosexec.hpp (by name) |
| `formats/__init__.py` | 0 | frozen_subjects, test_concolic, test_dos, test_drivers, test_exepack, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_powerpc, test_run_record, test_ui, test_verification, test_versions, test_vmx128, test_xex, tools.golden; 47 modules | (empty package marker) |
| `formats/cab.py` | 177 | frozen_subjects, test_media; 1 modules | native/cb/formats/cab.hpp (by name) |
| `formats/detect.py` | 88 | frozen_subjects, test_plumbing; 8 modules | native/cb/formats/detect.hpp (by name) |
| `formats/dos.py` | 250 | frozen_subjects, test_dos, test_exepack, test_pklite, test_verification; 6 modules | native/cb/formats/dos.hpp (by name) |
| `formats/le.py` | 375 | frozen_subjects, test_drivers, tools.golden; 6 modules | native/cb/formats/le.hpp (by name) |
| `formats/native.py` | 223 | test_native_formats; 11 modules | native/cb/analyze/native.hpp (by name) |
| `formats/ne.py` | 163 | frozen_subjects, test_concolic, test_native_trace, test_verification, test_versions; 15 modules | native/cb/formats/ne.hpp (by name) |
| `formats/omf.py` | 248 | test_verification; 6 modules | native/cb/formats/omf.hpp (by name) |
| `formats/pe.py` | 176 | frozen_subjects, test_concolic, test_run_record, test_ui; 18 modules | native/cb/formats/pe.hpp (by name) |
| `formats/pe_functions.py` | 366 | frozen_subjects, test_versions; 4 modules | native/cb/formats/pe_functions.hpp (by name) |
| `formats/powerpc_image.py` | 128 | test_powerpc, test_vmx128; 5 modules | native/cb/formats/powerpc_image.hpp (by name) |
| `formats/resources.py` | 196 | frozen_subjects; 1 modules | native/cb/formats/resources.hpp (by name) |
| `formats/xex.py` | 173 | test_vmx128, test_xex; 5 modules | native/cb/formats/xex.hpp (by name) |
| `jobs/__init__.py` | 0 | test_knowledge_reads, test_plumbing; 4 modules | (empty package marker) |
| `jobs/pool.py` | 287 | test_knowledge_reads, test_plumbing; 4 modules | native/cb/jobs/pool.hpp (by name) |
| `knowledge/__init__.py` | 173 | test_knowledge, test_knowledge_reads, test_native_engine; 17 modules | native/cb/knowledge/package.hpp |
| `knowledge/reads.py` | 174 | test_knowledge_reads; 10 modules | native/cb/knowledge/reads.hpp (by name) |
| `knowledge/sources.py` | 116 | test_knowledge; 5 modules | native/cb/knowledge/sources.hpp (by name) |
| `knowledge/store.py` | 72 | test_knowledge, test_knowledge_reads, test_native_engine; 8 modules | native/cb/knowledge/store.hpp (by name) |
| `platforms/__init__.py` | 387 | test_port_fixes, test_run_record; 25 modules | native/cb/java/platforms.hpp (by name) |
| `platforms/dos.py` | 34 | - | native/cb/platforms/dos.hpp (by name) |
| `platforms/java.py` | 27 | - | native/cb/platforms/java.hpp (by name) |
| `platforms/vxd.py` | 43 | - | native/cb/platforms/vxd.hpp (by name) |
| `platforms/win16.py` | 58 | - | native/cb/platforms/win16.hpp (by name) |
| `platforms/win32.py` | 42 | - | native/cb/platforms/package.cpp (`win32()`) |
| `platforms/xbox360.py` | 45 | - | native/cb/platforms/package.cpp (`xbox360()`) |
| `source/__init__.py` | 0 | test_port_fixes, test_readable, translate; 3 modules | native/cb/java/emit/source.hpp (by name) |
| `source/assembly.py` | 185 | 1 modules | native/cb/source/assembly.hpp (by name) |
| `source/ne_asm/__init__.py` | 0 | test_port_fixes | (empty package marker) |
| `source/ne_asm/segdiff.py` | 212 | test_port_fixes | native/cb/source/ne_asm/segdiff.hpp (by name) |
| `source/placement.py` | 121 | 1 modules | native/cb/source/placement.hpp (by name) |
| `source/readable.py` | 644 | test_readable, translate; 1 modules | native/cb/source/readable.hpp (by name) |
| `store/__init__.py` | 0 | test_knowledge, test_native_engine, test_store, test_symbols, test_ui, test_ui_thin, test_verdicts, test_verification; 1 modules | native/cb/knowledge/store.hpp (by name) |
| `store/functions.py` | 126 | test_knowledge, test_native_engine, test_store, test_symbols, test_ui, test_verdicts, test_verification; 1 modules | native/cb/store/functions.hpp (by name) |
| `symbols/__init__.py` | 0 | test_dos, test_drivers, test_knowledge_reads, test_symbols, test_verification; 23 modules | native/cb/exact/symbols.hpp (by name) |
| `symbols/api_arguments.py` | 428 | test_knowledge_reads; 6 modules | native/cb/symbols/api_arguments.hpp (by name) |
| `symbols/demangle.py` | 107 | test_symbols; 2 modules | native/cb/symbols/demangle.hpp (by name) |
| `symbols/dos_interrupts.py` | 101 | test_dos; 3 modules | native/cb/symbols/dos_interrupts.hpp (by name) |
| `symbols/driver_services.py` | 280 | test_drivers; 4 modules | native/cb/symbols/driver_services.hpp (by name) |
| `symbols/libraries/__init__.py` | 104 | test_symbols; 5 modules | native/cb/symbols/libraries/package.hpp |
| `symbols/libraries/coff.py` | 166 | test_symbols; 2 modules | native/cb/symbols/libraries/coff.hpp (by name) |
| `symbols/libraries/omf.py` | 152 | 1 modules | native/cb/symbols/libraries/omf.hpp (by name) |
| `symbols/native.py` | 70 | 2 modules | native/cb/analyze/native.hpp (by name) |
| `symbols/ordinals.py` | 377 | test_symbols; 3 modules | native/cb/symbols/ordinals.hpp (by name) |
| `symbols/prototypes.py` | 124 | test_knowledge_reads, test_symbols; 6 modules | native/cb/symbols/prototypes.hpp (by name) |
| `symbols/runtime_library.py` | 386 | test_symbols; 3 modules | native/cb/symbols/runtime_library.hpp (by name) |
| `symbols/sdk_structs.py` | 265 | 3 modules | native/cb/symbols/sdk_structs.hpp (by name) |
| `symbols/wine16.py` | 89 | test_symbols, test_verification; 1 modules | native/cb/symbols/wine16.hpp (by name) |
| `symbols/xdk_libraries.py` | 151 | 2 modules | native/cb/symbols/xdk_libraries.hpp (by name) |
| `translate/__init__.py` | 6 | test_readable, test_verdicts, translate, tools.l2c; 11 modules | (package marker) |
| `translate/backends/__init__.py` | 1 | translate, tools.l2c; 5 modules | (package marker) |
| `translate/backends/asm.py` | 71 | translate; 1 modules | native/cb/translate/backends/asm.hpp (by name) |
| `translate/backends/c.py` | 96 | translate, tools.l2c; 4 modules | native/cb/translate/backends/c.hpp (by name) |
| `translate/backends/cpp17.py` | 53 | 1 modules | native/cb/translate/backends/cpp17.hpp (by name) |
| `translate/backends/module_file.py` | 127 | translate | native/cb/translate/backends/module_file.hpp (by name) |
| `translate/desugar.py` | 490 | translate; 1 modules | native/cb/translate/desugar.hpp (by name) |
| `translate/frontends/__init__.py` | 2 | translate | (package marker) |
| `translate/frontends/java/__init__.py` | 118 | translate | native/cb/platforms/java.hpp (by name) |
| `translate/frontends/java/lower.py` | 543 | translate | native/cb/translate/frontends/clang/lower.hpp (by name) |
| `translate/ir.py` | 320 | translate; 3 modules | native/cb/translate/ir.hpp (by name) |
| `translate/l2.py` | 431 | test_readable, test_verdicts, translate; 5 modules | native/cb/translate/l2.hpp (by name) |
| `translate/native_l2.py` | 340 | translate; 4 modules | native/cb/translate/native_l2.hpp (by name) |
| `verify/__init__.py` | 12 | frozen_subjects, test_concolic, test_native_engine, test_native_trace, test_plumbing, test_run_record, test_ui_thin, test_verdicts, test_verification; 12 modules | native/cb/port/verify.hpp (by name) |
| `verify/compare.py` | 228 | test_plumbing, test_verdicts, test_verification; 4 modules | native/cb/verify/compare.hpp (by name) |
| `verify/concolic.py` | 229 | test_concolic; 1 modules | native/cb/verify/concolic.hpp (by name) |
| `verify/guided.py` | 222 | 3 modules | native/cb/verify/guided.hpp (by name) |
| `verify/lane_common.py` | 264 | test_run_record, test_verdicts, test_verification; 2 modules | native/cb/verify/lane_common.hpp (by name) |
| `verify/lanes/__init__.py` | 4 | frozen_subjects, test_concolic, test_native_trace, test_run_record, test_verdicts, test_verification; 5 modules | native/cb/engine/lanes.hpp (by name) |
| `verify/lanes/win16.py` | 727 | test_run_record, test_verdicts, test_verification; 1 modules | native/cb/verify/lanes/win16.hpp (by name) |
| `verify/lanes/win32.py` | 917 | frozen_subjects, test_concolic, test_native_trace, test_run_record, test_verdicts; 3 modules | native/cb/verify/lanes/win32.hpp (by name) |
| `verify/verdicts.py` | 215 | test_native_engine, test_verdicts; 6 modules | native/cb/verify/verdicts.hpp (by name) |
