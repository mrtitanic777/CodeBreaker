// codebreaker/analyze/native.hpp -- typed C++ port of codebreaker.analyze.native.
//
// native.py -- codebreaker/analyze/ in C++ (native/analyze: cbanalyze.dll), through ctypes.
//
// A thin shim. An image is handed to the DLL once (its spans, function starts and limits, the
// addresses standing for imports, the pointers the file names) and stays there; questions about it
// come back as JSON, so callers keep the very Python values they had.
//
//     h = native.Handle(img)        the image in the DLL (freed with the handle)
//     h.ask("image.discover")       the answer as Python values
//
// The Python it replaced is deleted, so without the built DLL these questions fail loudly
// (why_not() says why). Build: native/analyze/CMakeLists.txt (tools/build_native.cmd analyze).
//
// What differs from the Python, and why:
//   * ctypes is LoadLibrary / GetProcAddress here (Windows only; elsewhere the DLL is "not built").
//   * The DLL is loaded from its build folder directly, not from the hash-keyed copy under state/dll/
//     that settings.load_native makes so a running process never blocks a rebuild.
//   * `import capstone` (and capstone._cs._name, the very capstone.dll Python decodes with) has no
//     C++ counterpart: the caller names that file with set_capstone_dll() before the first use. Until
//     it does, loading raises the ImportError the Python raises without the module.
//   * The DLL's answers are JSON; Python returns json.loads of them (values of no fixed type), here
//     they are the JSON text itself (ask(), assemble()), for the caller to read with the JSON reader.
//   * The Image the Python reads attributes off is the struct below (the fields this module reads).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::analyze::native {

extern const int64_t _VERSION;
// pointer kind name -> the number the DLL's blob uses
extern const py::dict<py::str, int64_t> _POINTER_KINDS;

// (va, data, executable, writable, name, initialised length)
using Span = std::tuple<int64_t, py::bytes, bool, bool, std::optional<py::str>, int64_t>;
// (kind, target): where the file says the loader writes an address, and which kind of address
using Pointer = std::tuple<py::str, std::optional<int64_t>>;

// The fields of an analyze.image.Image that _blob and _names read.
struct Image {
    py::str kind;                                          // img.kind
    int64_t bits = 32;                                     // img.bits
    bool flat = false;                                     // img.flat (a property of kind in the Python)
    std::optional<py::dict<int64_t, Pointer>> pointers;    // {address: (kind, target)}, None: the file does not say
    py::list<Span> spans;
    py::dict<int64_t, py::str> names;                      // function start -> name
    py::dict<int64_t, int64_t> limits;                     // function start -> where its bytes may run to
    py::dict<int64_t, py::str> imports;                    // address standing for an import -> "MODULE!name"
    std::optional<int64_t> data_segment;                   // 16-bit: base address of the automatic data segment
    py::list<int64_t> roots;                               // where execution can enter from outside
};

py::str dll_path();

// Names the capstone.dll to hand cba_init (the C++ stand-in for capstone._cs._name).
void set_capstone_dll(const py::str& path);

// cbanalyze.dll as loaded: its entry points (defined in the .cpp).
struct Library;

// The loaded library, or nullptr when it could not be loaded (why_not() says why). Loaded once.
Library* _load();

// None if cbanalyze.dll loaded; else why not (loads it on first use).
std::optional<py::str> why_not();

// The loaded library; raises RuntimeError("cbanalyze.dll: <why>") when there is none.
Library& library();

// The image as the DLL reads it (little-endian): kind, bits, flat, has pointers; the spans
// (va, length, executable, writable, initialised length, name, bytes); the function starts with their
// limits; the import addresses; the pointers in the file's order (site, kind, has target, target).
py::bytes _blob(const Image& img);

// The functions' names, then the imports' (count; address, length, UTF-8 each).
py::bytes _names(const Image& img);

// strings' _assemble in the DLL: {(base, offset): (byte, store number)} -> its JSON answer (text).
py::str assemble(const py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>>& stores, int64_t least);

// An image held by the DLL.
class Handle {
public:
    explicit Handle(const Image& img);
    ~Handle();
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    // Hand over the image's names (after discover names the starts it found).
    void names(const Image& img);

    // The DLL's answer to a question as JSON text, or None when it has none (rc 0).
    std::optional<py::str> ask(const py::str& question, int64_t arg = 0);

private:
    void* h_ = nullptr;
};

}  // namespace codebreaker::analyze::native
