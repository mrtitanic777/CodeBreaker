// codebreaker/exact/plm86.hpp -- typed C++ port of codebreaker.exact.plm86.
//
// plm86.py -- Intel's ISIS-II tools for the 8086 (PL/M-86 V1.1-2.1, ASM86 2.0/2.1; 8080 programs), as toolchains.
//
// A profile names this module (toolchains/plm86.json, asm86.json). The tool runs under the ISIS-II
// emulator (emu/isis.py); the object is Intel's OMF-86 (omf.read reads it). The profile says which:
//
//     "tool": folder of the kit (plm86, asm86)   "program": its file (PLM86, ASM86)
//     "source": the name the source is given (T.PLM, T.A86)
//     "tail": the command line after the program, {controls} the variant's
//     "ok": a regular expression the console and listing must match (group 1 "0": no errors), or none
//
// and the variants the version and the controls: {"version": "2.1", "controls": "MEDIUM OPTIMIZE(2) CODE"}.
// A source's $INCLUDEd files are the project's headers (the profile's "headers" extensions: .LIT).
//
// Typing notes:
//   * The Python class PLM86 is a toolchain of its own (it does not subclass exact.toolchain.Toolchain, and never
//     calls its __init__); here the class derives from the typed Toolchain only so that toolchain::load() can hand
//     it out as a plugin ("codebreaker.exact.plm86:PLM86": register_plugin() below puts the factory there).
//     The base is constructed with the profile's default variant, as load() would; its own compile() is not used.
//     `name`, `variant` (the empty text for None), and `p` are the base's; `key` is this class' own (the base's key
//     is shadowed on purpose: the Python attribute of that name is the one computed here).
//   * The profile is the typed Profile (JSON parsed by the toolchain port); the entries that port does not keep
//     (tool, program, source, tail, ok, limit, controls, the variants' version and controls) are read from the
//     profile's own JSON text (Profile::keyed) as the Python reads the dict.
//   * compile() with `headers=None` is the optional; `include` is accepted and not used (as in the Python).
//     The cached value is the tuple (object or None, console) under cache kind "exact".
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::plm86 {

extern const py::str CACHE_VERSION;   // "p2"

class PLM86 : public toolchain::Toolchain {
  public:
    PLM86(const toolchain::Profile& profile, const std::optional<py::str>& variant = std::nullopt);

    py::str version;
    py::str controls;
    py::str key;   // sha1(name + version + controls + CACHE_VERSION)[:12]

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override;

    // compile(src, headers=None, include=(), switches=None, cache=True) -> (the object module or None, the console text)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src,
                                                          const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt,
                                                          bool cache = true) const;

    // the object, with the headers (name upper-cased, sorted) as given
    std::tuple<std::optional<py::bytes>, py::str> _compile(const py::bytes& src,
                                                           const py::list<std::tuple<py::str, py::bytes>>& headers,
                                                           const py::str& controls) const;

  private:
    struct Raw;                      // the profile's JSON, as the Python's dict
    std::shared_ptr<const Raw> raw_;
};

// puts PLM86 where toolchain::load() finds it: the plugin "codebreaker.exact.plm86:PLM86"
void register_plugin();

}  // namespace codebreaker::exact::plm86
