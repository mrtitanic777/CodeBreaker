// codebreaker/java/runtime.hpp -- typed C++ port of codebreaker.java.runtime.
//
// runtime.py -- support files the converter emits alongside the translated classes.
//
// These stand in for Java runtime types that aren't part of the decompiled corpus
// (java.util.Random, ...). They are emitted into the output by the pipeline and
// #included by any class that references them (see typemap.RUNTIME_HEADERS).
//
// Notes on the port:
//   * the Python module runs its body at import (loads the canonical headers from disk, builds the
//     _GENERATED and FILES dicts, registers STUB_MEMBERS). C++ has no import step, so the module's
//     variables are accessor functions with the Python names (RANDOM_H(), FILES(), STUB_MEMBERS(), ...)
//     that build the module's state on first use; the canonical folder is found through
//     java::_paths::data_path, so a host sets _paths::set_here before the first use;
//   * STUB_MEMBERS is {class: {'parent': base_or_None, 'methods': {method: cpp_ret_type}}}: a
//     StubMembers record per class;
//   * the (name, (type, value)) pairs of ret_methods / ret_fns are std::tuple<str, tuple<str, str>>,
//     the (name, value) pairs of consts are std::tuple<str, int64_t> (every const in the table is an int);
//   * cpp_std is codebreaker.java.transpile.CppStandard (no typed port of that module): the CppStd record of
//     emit/header.hpp (its year and label) stands in for it; None -> std::nullopt;
//   * the first-use build is not thread-safe (the Python's import lock has no counterpart here).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/java/emit/header.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::runtime {

using StrList = py::list<py::str>;
using Ret = std::tuple<py::str, std::tuple<py::str, py::str>>;   // (method, (return type, default value))
using RetList = py::list<Ret>;
using Const = std::tuple<py::str, int64_t>;                       // (name, value)
using ConstList = py::list<Const>;

// One STUB_MEMBERS entry: {'parent': base_or_None, 'methods': {method: cpp_ret_type}}.
struct StubMembers {
    std::optional<py::str> parent;
    py::dict<py::str, py::str> methods;
};

// Canonical hand-written java.io byte-stream headers (real, not stubs). Stored verbatim in
// docs/platform/canonical/ and loaded here so the netcode/save byte streams are REAL and there is exactly
// ONE definition of each (the C4743 vtable-ODR fix). [360] NO XDK EQUIVALENT -- hand-implemented: pure C++
// big-endian byte codecs (DataInput/Output) + passthrough/streaming wrappers (Buffered/Data*Stream).
py::str _load_canonical(const py::str& header);

// Faithful java.util.Random: 48-bit LCG with Java's exact constants, so generated
// worlds/sequences match the Java original bit-for-bit.
const py::str& RANDOM_H();
// --- external Java base types our corpus extends/implements but that aren't decompiled.
// Minimal but faithful stubs so inheritance + overrides compile (a missing base header is
// a fatal include error that blocks the whole translation unit). ---
const py::str& RUNNABLE_H();
const py::str& THREAD_H();
const py::str& COMPARATOR_H();
extern const py::str FILE_H;
const py::str& FILENAMEFILTER_H();
const py::str& CANVAS_H();
const py::str& APPLET_H();
// Universal runtime prelude: free-function shims for java.lang.* statics that have no
// clean inline mapping. #included by every emitted .cpp (named JavaRT.h, NOT Math.h, to
// avoid colliding with system <math.h> on a case-insensitive filesystem).
// java.lang.String -> a thin std::string subclass that adds Java's String methods.
const py::str& JAVASTRING_H();
const py::str& JARRAY_H();
const py::str& JITERATOR_H();
const py::str& JLIST_H();
// HashSet / TreeSet -> JSet<T>: a std::set subclass with the Java Set API.
const py::str& JSET_H();
// HashMap / TreeMap / LinkedHashMap / Map -> JMap<K,V>: a std::map subclass with the Java Map API.
const py::str& JMAP_H();
const py::str& JAVART_H();
extern const py::str GL11_H;
const py::str& COLLECTIONS_H();

// Best-effort Java package for the external types we stub, so the banner names the real library.
const py::dict<py::str, py::str>& _EXT_PKG();
// common java.awt Component/Container surface, shared by Canvas/Panel/Component/Applet stubs
const py::list<py::str>& _AWT_VOID();
// LWJGL 2's Keyboard.getKeyName table: key code (as text) -> constant name without KEY_.
const py::dict<py::str, py::str>& _LWJGL_KEY_NAMES();
// java.awt.Color constants: (name, ARGB).
const py::list<std::tuple<py::str, int64_t>>& _COLOR_RGB();

py::str _guard(const py::str& name);

// Header banner marking a SYMBOL-ONLY stub for an external Java library type (the user asked
// every such stub to say so). Names the real package when known. These are NOT decompiled game
// source; their behaviour is the Xbox 360 platform layer's job.
py::list<py::str> _ext_banner(const py::str& name);

// A namespace of static constants + functions (e.g. GL15, Keyboard) -- accessed Name::X.
py::str _ns_stub(const py::str& name, const ConstList& consts = {}, const StrList& void_fns = {},
                 const RetList& ret_fns = {}, const StrList& includes = {}, const StrList& fwds = {});

// Return-type registry for the emitted stub classes, so expression-type inference (the `.` vs `->`
// chain resolver of the old text translator) can type a chain THROUGH a stub method result -- e.g.
// `Runtime::getRuntime()->maxMemory()`, `getDocumentBase()->getHost()`, `var->getParentFile()->mkdirs()`.
// Populated as each stub is built. Last definition wins (matches _GENERATED overwrite for classes
// redefined, e.g. File). The pipeline converts the C++ return spellings to JTypes and merges this into
// the member environment.
py::dict<py::str, StubMembers>& STUB_MEMBERS();

// A class with variadic stub methods (e.g. SoundSystem, DataInput) -- accessed var->X.
// `extra` = verbatim extra member lines (e.g. conversion operators).
py::str _class_stub(const py::str& name, const StrList& void_methods = {}, const RetList& ret_methods = {},
                    bool ctors = true, const StrList& bases = {}, const StrList& includes = {},
                    const StrList& fwds = {}, const StrList& extra = {});

// NIO buffers: real classes (not raw pointers) so the .clear()/.put()/.flip() calls compile.
// Cross-return by pointer (forward-declared) to avoid include cycles.
py::str _buffer_stub(const py::str& name);

// header name -> text of every generated stub (the module's _GENERATED)
py::dict<py::str, py::str>& _GENERATED();

// filename -> content
py::dict<py::str, py::str>& FILES();

// Every docs/platform/canonical/*.h must be _load_canonical'd into a real emitted file; one that isn't
// silently falls back to its no-op stub (same class as the GLU flat-world bug). Returns the canonical
// headers that are NOT present in the emitted FILES map -- empty = all wired.
py::list<py::str> unwired_canonicals();
const py::list<py::str>& _UNWIRED();

// Inject a platform-specific SDK hint comment after the EXTERNAL-LIBRARY STUB banner.
// If the file isn't a stub or no platform is set, returns the text unchanged.
py::str annotate_stub(const py::str& text, const py::str& class_name, const std::optional<py::str>& platform_id);

extern const py::str _PLATFORM_BANNER_OLD;
extern const py::str _PLATFORM_BANNER_SDK_OLD;

// The file layer's out-of-line definitions for a conversion with no platform (see generate_files): what
// FileIO.reference.cpp gives the 360 through CreateFile, here through stdio.
extern const py::str HOST_IO_CPP;

// Dynamically generate the full runtime file set, applying C++ standard
// transforms and platform-specific stub annotations on the fly.
//
// This is the dynamic replacement for the static FILES dict. Every call
// produces a fresh dict so the same converter session can re-run with
// different settings without stale cached output.
py::dict<py::str, py::str> generate_files(const std::optional<emit::header::CppStd>& cpp_std = std::nullopt,
                                          const std::optional<py::str>& platform_id = std::nullopt);

// platform -> {'GetTickCount()' | 'currentTimeMillis' | 'nanoTime': replacement text}
const py::dict<py::str, py::dict<py::str, py::str>>& _TIMER_PATCHES();

// (The module-level re.compile(...) patterns, made on first use.)
const py::re::Pattern& _EXTRA_PTR_METHOD();
const py::re::Pattern& _CURRENT_TIME_RE();
const py::re::Pattern& _NANO_TIME_RE();
const py::re::Pattern& _GET_TICK_COUNT();

// Replace Windows timer calls in JavaRT.h / Random.h with platform equivalents.
py::str _patch_timer_for_platform(const py::str& content, const py::str& platform_id);

}  // namespace codebreaker::java::runtime
