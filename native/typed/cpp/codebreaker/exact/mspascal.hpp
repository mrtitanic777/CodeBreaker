// codebreaker/exact/mspascal.hpp -- typed C++ port of codebreaker.exact.mspascal.
//
// mspascal.py -- the Microsoft Pascal compiler (PAS1/PAS2; 1.10 of November 1981 to 3.11), as a toolchain.
//
// A profile names this module (toolchains/mspascal.json). The passes run under the DOS emulator:
// `PAS1 T,T,T,T;` (source, object, listing, code listing), then `PAS2`. The object is OMF. Most of a
// compiland's procedures are not public; the plug-in names them, so that the engine sees them as it
// sees a C file's functions: from the code listing (T.COD: "Procedure/Function : NAME" and the first
// address of its code) - or, for a compiler that writes none (3.04, 3.11), from where the code calls the
// compiler's entry helper (or sets up a frame in line), in the order the source emits its procedures
// (each at the end of its body: after those declared inside it). The names are published in the
// object (a PUBDEF of the code segment): omf.read and the ledger see them like a C file's functions.
//
// Every compiland ends with the compiler's entry helper (a near call to it opens each procedure); it
// is published as "(helper)".
//
// Variants: the kit's version ("version": a folder of the kit, or "1.10": the pre-release kit's zips).
//
// Typing notes:
//   * The Python class MSPascal is a toolchain of its own (it does not subclass exact.toolchain.Toolchain); here it derives
//     from the typed Toolchain only so that toolchain::load() can hand it out as a plugin ("codebreaker.exact.mspascal:MSPascal":
//     register_plugin() below). The base is constructed with the profile's default variant, as load() would; its own compile()
//     is not used. `name`, `variant` (the empty text for None) and `p` are the base's; `key` is this class' own.
//   * The profile entries the typed Profile does not keep (kit110, helper_data, the variants' version and helper_data) are
//     read from the profile's JSON text (Profile::keyed) as the Python reads its dict.
//   * compile() with `headers=None` is the optional; `include` and `switches` are accepted and not used (as in the Python).
//     The cached value is the tuple (object or None, console) under cache kind "exact".
//   * The 1.10 kit's zip files are read with zlib (stored and deflated entries), in place of zipfile.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::mspascal {

extern const py::str CACHE_VERSION;                       // "pas1"
extern const py::list<py::bytes> HELPERS;                 // the compiler's entry helper, by version
extern const py::bytes INLINE;                            // (3.11: the frame set up in line)

// the procedures of a source in the order their code is written: each at the end of its body, after the
// procedures declared inside it -> [name]
py::list<py::str> emit_order(const py::str& text);

// the segment names of the SEGDEFs of an object, in order
py::list<py::str> _segnames(const py::bytes& obj);

// the object with a PUBDEF of the code segment naming procs {name: offset} (before its MODEND)
py::bytes publish(const py::bytes& obj, const py::dict<py::str, int64_t>& procs, const py::str& code_seg);

class MSPascal : public toolchain::Toolchain {
  public:
    MSPascal(const toolchain::Profile& profile, const std::optional<py::str>& variant = std::nullopt);

    py::str version;
    int64_t helper_data;
    py::str key;   // sha1(name + version + CACHE_VERSION)[:12]

    // the folder of the compiler's files (the 1.10 kit's zips are extracted to the state folder once)
    py::str kit() const;

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override;

    // compile(src, headers=None, include=(), switches=None, cache=True, name="T.PAS") -> (the object module or None, the console text)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src,
                                                          const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt, bool cache = true,
                                                          const py::str& name = py::str("T.PAS")) const;

    std::tuple<std::optional<py::bytes>, py::str> _compile(const py::bytes& src, const py::str& name) const;

    // the object with its procedures' names published: from the code listing `cod`, else from the code
    py::bytes _named(const py::bytes& obj, const py::str& cod, const py::str& text) const;

  private:
    struct Raw;
    std::shared_ptr<const Raw> raw_;
};

// puts MSPascal where toolchain::load() finds it: the plugin "codebreaker.exact.mspascal:MSPascal"
void register_plugin();

}  // namespace codebreaker::exact::mspascal
