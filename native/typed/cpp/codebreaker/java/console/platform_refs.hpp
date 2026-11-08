// codebreaker/java/console/platform_refs.hpp -- typed C++ port of codebreaker.java.console.platform_refs.
//
// platform_refs.py -- emit the hand-written platform layer instead of no-op stubs.
//
// The converter normally emits SYMBOL-ONLY stubs for the platform subsystems (GL11, Audio, Input,
// Socket, ...). When `--platform-refs` is on, this module emits the REAL, hand-written implementations
// from `docs/platform/reference/*.reference.{h,cpp}` in their place -- the GL->D3D9 emulator, XACT audio,
// XInput, XNet socket, etc.
//
// Notes on the port:
//   * the Java IR objects are the real codebreaker::java::ir types (py::ref<JClass> and so on);
//   * `emit_references.last_skipped` (a function attribute in Python) is the variable
//     emit_references_last_skipped;
//   * audit_references returns a record (a dict with two fixed keys in Python);
//   * runnable_game is a null py::ref for Python's None.
#pragma once

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/java/ir.hpp"

namespace codebreaker::java::console::platform_refs {

// A reference file often provides MORE than one header's worth of symbols. The game #includes the
// per-type stub header (Mouse.h / Keyboard.h / SoundSystem.h), so emitting only `Input.h` / `Audio.h`
// would leave the no-op stub shadowing the real impl. For each such reference we ALSO overwrite the
// stubs it replaces with a thin `#include` shim.
extern py::dict<py::str, py::list<py::str>> REFERENCE_REPLACES;

// References that lack a Beta-specific method the converter's STUB already provides (and would otherwise
// shadow it), or that need companion types not yet bundled. SKIP emitting them -> the converter's stub
// stands.
extern py::set<py::str> REFERENCE_SKIP;

// Base names X that have an X.reference.h in reference_dir (e.g. {'GL11','Audio','Socket',...}).
py::set<py::str> available_references(const py::str& reference_dir);

// The game's entry class: the one declaring `public static void main(String[])`. Returns its
// name, or None.
std::optional<py::str> detect_main_class(const py::list<py::ref<ir::JClass>>& classes);

py::str generate_entry_point(const py::str& main_class);

// Java primitive type names, for generating default entry-point constructor args.
extern py::set<py::str> ENTRY_PRIMS_;

py::str generate_pc_entry_point(const py::str& main_class);

// The game's shell class when it has NO `static main` but IS a Runnable started on a thread
// (Indev/Infdev). Returns the JClass, or null (None).
py::ref<ir::JClass> detect_runnable_game_class(const py::list<py::ref<ir::JClass>>& classes);

py::str _runnable_ctor_args(const py::ref<ir::JClass>& cls);
py::str generate_runnable_entry_point(const py::ref<ir::JClass>& cls);

// References that are legitimately .cpp-only (no stub .h to replace): Main is the standalone entry point.
extern py::set<py::str> SPECIAL_CPP_ONLY_;

// What audit_references returns (a dict with these two keys in Python).
struct AuditResult {
    py::set<py::str> complete;   // bases with a .reference.h
    py::list<py::str> orphans;   // sorted .cpp-only bases, excluding Main and *.ext
};
AuditResult audit_references(const py::str& reference_dir);

// After emission, confirm each reference base's tree X.h actually carries the PLATFORM REFERENCE banner.
// Returns the bases that did NOT integrate.
py::list<py::str> verify_integrated(const py::str& out_dir, const py::set<py::str>& bases);

extern py::dict<py::str, std::tuple<py::str, py::str>> LAYER_;

py::str _banner(const py::str& base, const py::str& ext, const py::str& platform = py::str("xbox360"));

// emit_references.last_skipped
extern py::list<py::str> emit_references_last_skipped;

// Write each X.reference.{h,cpp} to out_dir/X.{h,cpp} (with a provenance banner), replacing the
// emitted stub. Returns the set of base names emitted. Also writes PLATFORM_LAYER.md.
py::set<py::str> emit_references(const py::str& out_dir, const py::str& reference_dir,
                                 const py::set<py::str>& game_classes = py::set<py::str>(),
                                 const std::optional<py::str>& main_class = std::nullopt,
                                 const py::ref<ir::JClass>& runnable_game = nullptr,
                                 const py::str& platform = py::str("xbox360"));

py::str _companion_banner(const py::str& fn);
void _write_platform_layer_md(const py::str& out_dir, const py::list<py::str>& emitted,
                              const py::list<py::str>& files_written);

}  // namespace codebreaker::java::console::platform_refs
